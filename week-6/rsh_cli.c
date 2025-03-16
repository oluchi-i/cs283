#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <fcntl.h>

#include "dshlib.h"
#include "rshlib.h"

#define MAX_CMD_LENGTH 1024   // Maximum command length buffer size
#define MAX_RSP_LENGTH 2048   // Maximum response length buffer size


/*
 * exec_remote_cmd_loop(server_ip, port)
 *      server_ip:  a string in ip address format, indicating the server's IP
 *                  address. By default, 127.0.0.1 means the server is on the
 *                  same machine as the client.
 *              
 *      port:   The port the server will use. The constant 
 *              RDSH_DEF_PORT is defined in rshlib.h (usually 1234), or 
 *              you can override it via the command line (e.g., `-c` option).
 * 
 *      This function executes a networked command loop that sends commands
 *      to the server and receives responses.
 *  
 *      1. Allocates buffers for sending and receiving data over the network
 *      2. Establishes a network connection to the server via start_client()
 *      3. Continuously reads commands from the user, sends them to the server,
 *         and prints the responses.
 *
 *   returns:
 *          OK:      Successfully executed commands and exiting the loop
 *          ERR_MEMORY:  Failed to allocate memory for buffers
 *          ERR_RDSH_CLIENT:  Could not connect to the server
 *          ERR_RDSH_COMMUNICATION:  Communication error during sending/receiving
 */
int exec_remote_cmd_loop(char *server_ip, int port) {
    // Initialize the client socket by establishing a connection with the server
    int cli_socket = start_client(server_ip, port);
    if (cli_socket == -1) return ERR_RDSH_CLIENT;  // Error in connecting to server

    // Buffers to hold the command to send and the response to receive
    char cmd_buff[MAX_CMD_LENGTH];   // Command buffer
    char rsp_buff[MAX_RSP_LENGTH];   // Response buffer

    while (1) {
        // Prompt the user for a command input
        printf("rsh> ");
        if (fgets(cmd_buff, MAX_CMD_LENGTH, stdin) == NULL) {
            break; // Exit on EOF or error from fgets()
        }

        // Send the command to the server
        if (send(cli_socket, cmd_buff, strlen(cmd_buff), 0) == -1) {
            perror("Send failed");
            break; // Exit if sending the command fails
        }

        // Initialize the response buffer and variables for chunked data handling
        int total_bytes_received = 0;
        int bytes_received = 0;

        // Receive the response from the server in chunks
        while (1) {
            bytes_received = recv(cli_socket, rsp_buff + total_bytes_received, MAX_RSP_LENGTH - total_bytes_received, 0);
            if (bytes_received <= 0) {
                perror("Recv failed");
                break; // Exit if receiving the response fails or no data is received
            }

            total_bytes_received += bytes_received;

            // Null-terminate the received response and check if it's EOF
            rsp_buff[total_bytes_received] = '\0';  
            if (rsp_buff[total_bytes_received - 1] == RDSH_EOF_CHAR) {
                break; // End of response from server
            }
        }

        printf("Server Response: %s\n", rsp_buff);

        // Check if the server indicates EOF (end of communication)
        if (rsp_buff[total_bytes_received - 1] == RDSH_EOF_CHAR) {
            // Server has finished sending data, we can continue to send another command
            continue;
        }
    }

    // Clean up by closing the socket after communication is done
    close(cli_socket);
    return OK;
}

/*
 * start_client(server_ip, port)
 *      server_ip:  the server's IP address in string format
 *      port:   The port the client will connect to on the server
 * 
 *      This function creates a client socket, sets up the server address, and 
 *      establishes a connection to the server. It returns the client socket 
 *      for further communication.
 * 
 *   returns:
 *          cli_socket:      The file descriptor of the client socket
 *          ERR_RDSH_CLIENT: If socket creation or connection fails
 */
int start_client(char *server_ip, int port) {
    struct sockaddr_in addr;
    int cli_socket;

    // Create the socket using the TCP/IP protocol (AF_INET) and SOCK_STREAM type
    cli_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (cli_socket < 0) {
        perror("socket creation failed");
        return ERR_RDSH_CLIENT; // Return error if socket creation fails
    }

    // Prepare the server address struct (AF_INET: IPv4)
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);  // Convert port to network byte order
    addr.sin_addr.s_addr = inet_addr(server_ip);  // Convert IP address to binary format

    // Establish a connection to the server
    if (connect(cli_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect failed");
        close(cli_socket);  // Close socket if connection fails
        return ERR_RDSH_CLIENT;
    }

    return cli_socket;  // Return the socket if connection is successful
}

/*
 * client_cleanup(int cli_socket, char *cmd_buff, char *rsp_buff, int rc)
 *      cli_socket:   The client socket file descriptor
 *      cmd_buff:     Buffer holding the command to send to the server
 *      rsp_buff:     Buffer holding the response received from the server
 * 
 *   This function is responsible for cleaning up the resources, such as:
 *      - Closing the socket if valid
 *      - Freeing the allocated memory for command and response buffers
 *      - Returning the error code passed as rc
 * 
 *   returns:
 *          rc:   The error code passed as parameter, to return the cleanup status
 */
int client_cleanup(int cli_socket, char *cmd_buff, char *rsp_buff, int rc) {
    // Close the socket if it's valid (opened)
    if (cli_socket > 0) {
        close(cli_socket);
    }

    // Free the allocated memory for command and response buffers
    free(cmd_buff);
    free(rsp_buff);

    // Return the provided error code
    return rc;
}
