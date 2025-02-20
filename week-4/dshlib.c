#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <errno.h>
#include "dshlib.h"

// Global variable to store the last return code of executed commands
int last_return_code = 0;

/*
 * parse_input:
 * This parser processes the input string and splits it into tokens.
 * It respects quoted strings so that text inside double quotes is kept intact.
 * It allocates no new memory; tokens are pointers into the original input.
 */
void parse_input(char *input, cmd_buff_t *cmd) {
    if (input == NULL) return;

    cmd->argc = 0; // Initialize argument count

    char *p = input;
    char *token_start = NULL;
    int in_quote = 0;

    while (*p) {
        if (*p == '"') {
            in_quote = !in_quote;  // Toggle quote flag
            if (in_quote) {
                // Start of a quoted token; set token_start to character after quote
                token_start = p + 1;
            } else {
                // End of quoted token; replace quote with null terminator
                *p = '\0';
                if (cmd->argc < CMD_ARGV_MAX)
                    cmd->argv[cmd->argc++] = token_start;
                token_start = NULL;
            }
        } else if (!in_quote && isspace(*p)) {
            // Outside of quotes: token boundary
            if (token_start != NULL) {
                *p = '\0';  // Terminate the token
                if (cmd->argc < CMD_ARGV_MAX)
                    cmd->argv[cmd->argc++] = token_start;
                token_start = NULL;
            }
        } else {
            // Non-space character
            if (!in_quote && token_start == NULL) {
                // Start of a new token outside quotes
                token_start = p;
            }
        }
        p++;
    }
    // If we reached end of input while a token is in progress
    if (token_start != NULL && cmd->argc < CMD_ARGV_MAX) {
        cmd->argv[cmd->argc++] = token_start;
    }
    // Ensure argv array is null-terminated
    if (cmd->argc < CMD_ARGV_MAX)
        cmd->argv[cmd->argc] = NULL;
}

// Built-in cd command handler
int execute_cd(char *path) {
    if (path == NULL || *path == '\0') {
        // No path provided; do nothing.
        return OK;
    }
    if (chdir(path) == -1) {
        perror("cd");
        return errno;  // Return error code from chdir
    }
    return OK;
}

// Built-in rc command: prints the last return code
void execute_rc() {
    printf("%d\n", last_return_code);
}

// Execute external commands using fork() and execvp()
int exec_cmd(cmd_buff_t *cmd) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return ERR_MEMORY;
    } else if (pid == 0) {
        // Child process
        execvp(cmd->argv[0], cmd->argv);
        // If execvp returns, an error occurred:
        switch (errno) {
            case ENOENT:
                fprintf(stderr, "Command '%s' not found in PATH\n", cmd->argv[0]);
                break;
            case EACCES:
                fprintf(stderr, "Permission denied for command '%s'\n", cmd->argv[0]);
                break;
            default:
                fprintf(stderr, "Unknown error: %s\n", strerror(errno));
                break;
        }
        exit(errno);  // Exit child process with error code
    } else {
        // Parent process: wait for child process to finish
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            last_return_code = WEXITSTATUS(status);
        } else {
            last_return_code = -1;
        }
    }
    return OK;
}

// Main shell loop to execute commands
int exec_local_cmd_loop() {
    char input[CMD_MAX];
    cmd_buff_t cmd;
    
    while (1) {
        // Display the prompt
        printf("%s", SH_PROMPT);
        fflush(stdout);
        
        // Read input from the user
        if (fgets(input, CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }
        // Remove trailing newline
        input[strcspn(input, "\n")] = '\0';

        // Skip empty input
        if (strlen(input) == 0)
            continue;

        // Parse the input into tokens
        parse_input(input, &cmd);
        if (cmd.argc == 0)
            continue;

        // Handle built-in commands
        if (strcmp(cmd.argv[0], "exit") == 0) {
            break;  // Exit shell
        } else if (strcmp(cmd.argv[0], "cd") == 0) {
            if (cmd.argc == 1)
                execute_cd(NULL);
            else
                execute_cd(cmd.argv[1]);
        } else if (strcmp(cmd.argv[0], "rc") == 0) {
            execute_rc();
        } else {
            // External command
            exec_cmd(&cmd);
        }
    }
    return OK;
}
