#define __DSHLIB_H__

// Constants for command structure sizes
#define EXE_MAX 64            // Maximum length for command executable name
#define ARG_MAX 256           // Maximum length for command arguments
#define CMD_MAX 8             // Maximum number of commands in a pipeline
#define CMD_ARGV_MAX (CMD_MAX + 1)  // Maximum arguments per command (including NULL terminator)
#define SH_CMD_MAX (EXE_MAX + ARG_MAX)  // Maximum size for a command (exe + args)

// Command structure: stores executable and arguments
typedef struct command {
    char exe[EXE_MAX];    // Command executable (e.g., 'ls')
    char *args[ARG_MAX];  // Command arguments (e.g., '-l', '/home') - use an array of strings
} command_t;

// Command buffer structure: stores parsed command arguments
typedef struct cmd_buff {
    int argc;             // Argument count
    char *argv[CMD_ARGV_MAX];  // Argument vector (array of strings)
    char *_cmd_buffer;    // Pointer to the entire command string
} cmd_buff_t;

// Command list structure: stores multiple commands in case of pipelines
typedef struct command_list {
    int num;               // Number of commands
    cmd_buff_t commands[CMD_MAX];  // Array of commands in the pipeline
} command_list_t;

// Special character definitions
#define SPACE_CHAR  " "     // Space character
#define PIPE_CHAR   '|'      // Pipe character
#define PIPE_STRING "|"      // Pipe string (to handle piping in shell)

// Shell prompt and exit command
#define SH_PROMPT "dsh3> "   // Prompt displayed to the user
#define EXIT_CMD "exit"      // Exit command to terminate the shell
#define EXIT_SC 99           // Exit status code

// Standard return codes for shell operation
#define OK                       0  // Command executed successfully
#define WARN_NO_CMDS            -1 // Warning: No commands provided
#define ERR_TOO_MANY_COMMANDS   -2 // Error: Too many commands in pipeline
#define ERR_CMD_OR_ARGS_TOO_BIG -3 // Error: Command or arguments too large
#define ERR_CMD_ARGS_BAD        -4 // Error: Invalid arguments for a command (extra credit)
#define ERR_MEMORY              -5 // Error: Memory allocation failure
#define ERR_EXEC_CMD            -6 // Error: Command execution failure
#define OK_EXIT                 -7 // Successful exit from shell

// Function prototypes for memory management and command processing
int alloc_cmd_buff(cmd_buff_t *cmd_buff);                // Allocate memory for command buffer
int free_cmd_buff(cmd_buff_t *cmd_buff);                 // Free memory of command buffer
int clear_cmd_buff(cmd_buff_t *cmd_buff);                // Clear contents of command buffer
int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff); // Build command buffer from input line
int close_cmd_buff(cmd_buff_t *cmd_buff);                // Close and clean up command buffer
int build_cmd_list(char *cmd_line, command_list_t *clist); // Build list of piped commands
int free_cmd_list(command_list_t *cmd_lst);              // Free memory of command list

// Built-in command handling
typedef enum {
    BI_CMD_EXIT,          // Built-in command: exit
    BI_CMD_DRAGON,        // Built-in command: dragon (example placeholder)
    BI_CMD_CD,            // Built-in command: cd (change directory)
    BI_NOT_BI,            // Not a built-in command
    BI_EXECUTED,          // Command executed successfully
} Built_In_Cmds;

Built_In_Cmds match_command(const char *input);           // Match command with built-in commands
Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd);         // Execute built-in command

// Main execution context
int exec_local_cmd_loop();                               // Main command loop for interactive shell
int exec_cmd(cmd_buff_t *cmd);                           // Execute a single command
int execute_pipeline(command_list_t *clist);             // Execute pipelined commands

// Output constants for command execution
#define CMD_OK_HEADER       "PARSED COMMAND LINE - TOTAL COMMANDS %d\n"
#define CMD_WARN_NO_CMD     "warning: no commands provided\n"
#define CMD_ERR_PIPE_LIMIT  "error: piping limited to %d commands\n"
#ifndef DSHLIB_H
#define DSHLIB_H

#endif // __DSHLIB_H__
