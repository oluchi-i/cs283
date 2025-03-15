#ifndef DSHLIB_H
#define DSHLIB_H

#include <stddef.h>

// Constants
#define MAX_ARGS 64      // Maximum arguments per command
#define MAX_CMDS 10      // Maximum commands in a pipeline
#define ERR_COMMAND_NOT_FOUND 127

// Structure to hold command information
typedef struct {
    char *argv[MAX_ARGS];   // Arguments
    int argc;               // Argument count
    int is_pipeline;        // Flag for pipeline
    char *cmds[MAX_CMDS];   // Commands in the pipeline
    int cmd_count;          // Number of commands in the pipeline
} cmd_buff_t;

// Function Declarations
void parse_input(char *input, cmd_buff_t *cmd);           // Parse input to cmd_buff
void exec_local_cmd_loop(char *input);                    // Main loop to handle user input
void exec_command(cmd_buff_t *cmd);                       // Execute a command
void exec_pipeline(cmd_buff_t *cmd);                      // Execute pipelined commands
void execute_cd(cmd_buff_t *cmd);                         // Handle 'cd' command
void execute_rc(int last_rc);                             // Handle 'rc' command

#endif
