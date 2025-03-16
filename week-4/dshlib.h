#ifndef __DSHLIB_H__
 #define __DSHLIB_H__
 
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 
 // Constants for command structure sizes
 #define EXE_MAX 64
 #define ARG_MAX 256
 #define CMD_MAX 8
 #define CMD_ARGV_MAX (CMD_MAX + 1)
 // Longest command that can be read from the shell
 #define SH_CMD_MAX EXE_MAX + ARG_MAX
 
 // Command buffer structure
 typedef struct cmd_buff {
     int argc;
     char *argv[CMD_ARGV_MAX];
     char *_cmd_buffer;          // (Optional) store original input if needed
     struct cmd_buff *next;      // For linked list use if needed (not used here)
 } cmd_buff_t;
 
 // Updated function prototype for a quote-aware parse_input function:
 void parse_input(char *input, cmd_buff_t *cmd);
 
 // Special character #defines
 #define SPACE_CHAR  ' '
 #define PIPE_CHAR   '|'
 #define PIPE_STRING "|"
 
 #define SH_PROMPT "dsh2> "
 #define EXIT_CMD "exit"
 
 // Standard Return Codes
 #define OK                       0
 #define WARN_NO_CMDS            -1
 #define ERR_TOO_MANY_COMMANDS   -2
 #define ERR_CMD_OR_ARGS_TOO_BIG -3
 #define ERR_CMD_ARGS_BAD        -4      // for extra credit
 #define ERR_MEMORY              -5
 #define ERR_EXEC_CMD            -6
 #define OK_EXIT                 -7
 
 // Prototypes for command buffer functions (if needed)
 int alloc_cmd_buff(cmd_buff_t *cmd_buff);
 int free_cmd_buff(cmd_buff_t *cmd_buff);
 int clear_cmd_buff(cmd_buff_t *cmd_buff);
 int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff);
 
 // Built-in command stuff
 typedef enum {
     BI_CMD_EXIT,
     BI_CMD_DRAGON,
     BI_CMD_CD,
     BI_NOT_BI,
     BI_EXECUTED,
     BI_RC,
 } Built_In_Cmds;
 
 Built_In_Cmds match_command(const char *input); 
 Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd);
 
 // Main execution context
 int exec_local_cmd_loop();
 int exec_cmd(cmd_buff_t *cmd);
 
 // Output constants
 #define CMD_OK_HEADER       "PARSED COMMAND LINE - TOTAL COMMANDS %d\n"
 #define CMD_WARN_NO_CMD     "warning: no commands provided\n"
 #define CMD_ERR_PIPE_LIMIT  "error: piping limited to %d commands\n"
 
 #endif // __DSHLIB_H__ 