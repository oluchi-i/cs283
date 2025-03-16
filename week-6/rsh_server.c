#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_CMD_LENGTH 256
#define MAX_RSP_LENGTH 1024

// Global variable to store the last return code (for extra credit)
int last_return_code = 0;

// Structure to store client data for multi-threaded handling
typedef struct {
    int client_socket;
    struct sockaddr_in client_addr;
} client_data_t;

/*
 * execute_command(cmd, client_socket)
 *   Forks a child process to execute the given command.
 *   Child's stdout and stderr are redirected to a pipe, whose output is then
 *   read by the parent and sent back to the client.
 *   Also updates the global last_return_code.
 */
void execute_command(char *cmd, int client_socket) {
    char rsp_buff[MAX_RSP_LENGTH] = "";
    FILE *fp;
    char output[MAX_RSP_LENGTH];
    int pipe_fd[2];  // Declare pipe_fd here so it is available to both parent and child

    if (pipe(pipe_fd) < 0) {
        perror("Pipe failed");
        snprintf(rsp_buff, MAX_RSP_LENGTH, "Error: Unable to create pipe\n");
        send(client_socket, rsp_buff, strlen(rsp_buff), 0);
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        snprintf(rsp_buff, MAX_RSP_LENGTH, "Error: Unable to fork process\n");
        send(client_socket, rsp_buff, strlen(rsp_buff), 0);
        return;
    }

    if (pid == 0) {
        // Child process: execute the command
        close(pipe_fd[0]);  // Close read end; child writes
        if (dup2(pipe_fd[1], STDOUT_FILENO) < 0) {
            perror("Failed to redirect stdout");
            exit(1);
        }
        if (dup2(pipe_fd[1], STDERR_FILENO) < 0) {
            perror("Failed to redirect stderr");
            exit(1);
        }
        close(pipe_fd[1]); // Close duplicate fd after redirection

        // Execute the command
        execlp(cmd, cmd, (char *)NULL);
        // If execlp fails
        perror("Exec failed");
        exit(1);
    } else {
        // Parent process: wait for child to complete
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            last_return_code = WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            last_return_code = 128 + WTERMSIG(status);
        }

        close(pipe_fd[1]);  // Close write end in parent

        fp = fdopen(pipe_fd[0], "r");
        if (fp == NULL) {
            perror("Failed to open pipe for reading");
            snprintf(rsp_buff, MAX_RSP_LENGTH, "Error: Failed to read output\n");
            send(client_socket, rsp_buff, strlen(rsp_buff), 0);
            return;
        }

        // Read command output into the buffer
        while (fgets(output, sizeof(output), fp) != NULL) {
            strncat(rsp_buff, output, MAX_RSP_LENGTH - strlen(rsp_buff) - 1);
        }
        fclose(fp);

        // Send the command output back to the client
        send(client_socket, rsp_buff, strlen(rsp_buff), 0);
    }
}

/*
 * handle_client(client_socket)
 *   Receives commands from the connected client, processes them, and sends back responses.
 *   Implements built-in commands such as "exit", "rc", and "ls".
 */
void handle_client(int client_socket) {
    char cmd_buff[MAX_CMD_LENGTH];
    char rsp_buff[MAX_RSP_LENGTH];
    int bytes_received;

    // Receive command from the client
    bytes_received = recv(client_socket, cmd_buff, sizeof(cmd_buff), 0);
    if (bytes_received <= 0) {
        perror("recv failed");
        snprintf(rsp_buff, MAX_RSP_LENGTH, "Error receiving command\n");
        send(client_socket, rsp_buff, strlen(rsp_buff), 0);
        return;
    }

    cmd_buff[bytes_received] = '\0';  // Null-terminate the received string

    // Handle "cd" command
    if (strncmp(cmd_buff, "cd", 2) == 0) {
        // Remove the "cd" part and any leading spaces
        char *dir = cmd_buff + 2;
        while (*dir == ' ') dir++;  // Skip spaces after "cd"

        if (chdir(dir) == -1) {
            // If chdir fails, send an error message with the reason
            snprintf(rsp_buff, MAX_RSP_LENGTH, "cd failed: %s\n", strerror(errno));
        } else {
            // Success, respond with a success message
            snprintf(rsp_buff, MAX_RSP_LENGTH, "Changed directory to: %s\n", dir);
        }
    }
    // Handle "exit" command
    else if (strncmp(cmd_buff, "exit", 4) == 0) {
        snprintf(rsp_buff, MAX_RSP_LENGTH, "Exiting...\n");
        send(client_socket, rsp_buff, strlen(rsp_buff), 0);
        close(client_socket); // Close the socket
        return; // Exit the client handler
    }
    // Handle "rc" command (return code)
    else if (strncmp(cmd_buff, "rc", 2) == 0) {
        snprintf(rsp_buff, MAX_RSP_LENGTH, "Last return code: %d\n", last_return_code);
    }
    // Handle "ls" command (list directory contents)
    else if (strncmp(cmd_buff, "ls", 2) == 0) {
        FILE *fp;
        char output[MAX_RSP_LENGTH];
        fp = popen("ls -l", "r");  // Use popen to execute "ls" and capture output
        if (fp == NULL) {
            perror("Failed to execute ls");
            snprintf(rsp_buff, MAX_RSP_LENGTH, "Error: Failed to run 'ls'\n");
        } else {
        snprintf(rsp_buff, MAX_RSP_LENGTH, "Directory contents:\n");
        while (fgets(output, sizeof(output), fp) != NULL) {
            strncat(rsp_buff, output, MAX_RSP_LENGTH - strlen(rsp_buff) - 1);
    }
    pclose(fp);  // Close the stream properly
}

    }
    // Handle unknown commands
    else {
        snprintf(rsp_buff, MAX_RSP_LENGTH, "Unknown command: %s\n", cmd_buff);
    }

    // Send the response back to the client
    int bytes_sent = send(client_socket, rsp_buff, strlen(rsp_buff), 0);
    if (bytes_sent == -1) {
        perror("send failed");
    }
}

/*
 * client_thread_func(client_data)
 *   This function is used by a thread to handle each client connection.
 *   It calls `handle_client()` and closes the connection once done.
 */
void *client_thread_func(void *arg) {
    client_data_t *client_data = (client_data_t *)arg;
    handle_client(client_data->client_socket);
    close(client_data->client_socket);
    free(client_data);
    return NULL;
}

/*
 * start_server(server_ip, port, is_threaded)
 *   Starts the server:
 *     - Creates and binds the server socket
 *     - Listens for incoming client connections
 *     - For each client, forks a process or creates a thread to handle the client
 *       (Multi-threaded mode is optional and controlled by the `is_threaded` parameter)
 */
void start_server(char *server_ip, int port, bool is_threaded) {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(1);
    }

    // Start listening for incoming connections
    if (listen(server_socket, 10) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(1);
    }

    printf("Server listening on %s:%d...\n", server_ip, port);

    while (1) {
        // Accept a client connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;  // Continue accepting new connections
        }

        printf("New connection from %s:%d\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        if (is_threaded) {
            // Allocate memory for client data
            client_data_t *client_data = malloc(sizeof(client_data_t));
            if (client_data == NULL) {
                perror("Memory allocation failed");
                close(client_socket);
                continue;
            }

            client_data->client_socket = client_socket;
            client_data->client_addr = client_addr;

            // Create a new thread for the client
            pthread_t client_thread;
            if (pthread_create(&client_thread, NULL, client_thread_func, (void *)client_data) != 0) {
                perror("Thread creation failed");
                close(client_socket);
                free(client_data);
                continue;
            }

            // Detach the thread so resources are automatically cleaned up when it exits
            pthread_detach(client_thread);
        } else {
            // Handle single-threaded client connection (blocking)
            handle_client(client_socket);  // This could block until the client disconnects
        }
    }

    // Close the server socket (this will never be reached in this code)
    close(server_socket);
}
