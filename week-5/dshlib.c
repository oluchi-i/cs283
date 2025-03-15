#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "dshlib.h"

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    cmd_buff->_cmd_buffer = strdup(cmd_line);
    if (!cmd_buff->_cmd_buffer) {
        perror("strdup failed");
        return ERR_MEMORY;
    }
    cmd_buff->argc = 0;

    char *start = cmd_buff->_cmd_buffer;
    while (*start) {
        while (*start == ' ') start++;
        if (*start == '\0') break;

        char *end = start;
        if (*start == '"' || *start == '\'') {
            char quote = *start++;
            end = start;
            while (*end && *end != quote) end++;
        } else {
            while (*end && *end != ' ') end++;
        }

        if (*end) *end++ = '\0';

        cmd_buff->argv[cmd_buff->argc] = strdup(start);
        if (!cmd_buff->argv[cmd_buff->argc]) {
            perror("strdup failed");
            return ERR_MEMORY;
        }
        cmd_buff->argc++;
        start = end;

        if (cmd_buff->argc >= CMD_ARGV_MAX - 1) break;
    }

    cmd_buff->argv[cmd_buff->argc] = NULL;
    return OK;
}

int build_cmd_list(char *cmd_line, command_list_t *clist) {
    clist->num = 0;
    char *saveptr;
    char *token = strtok_r(cmd_line, PIPE_STRING, &saveptr);
    while (token != NULL) {
        while (*token == ' ') token++;
        char *end = token + strlen(token) - 1;
        while (end > token && (*end == ' ')) {
            *end = '\0';
            end--;
        }
        if (strlen(token) > 0) {
            if (clist->num >= CMD_MAX) {
                fprintf(stderr, CMD_ERR_PIPE_LIMIT, CMD_MAX);
                return ERR_TOO_MANY_COMMANDS;
            }
            if (build_cmd_buff(token, &clist->commands[clist->num]) != OK) {
                return ERR_MEMORY;
            }
            clist->num++;
        }
        token = strtok_r(NULL, PIPE_STRING, &saveptr);
    }
    if (clist->num == 0) {
        fprintf(stderr, CMD_WARN_NO_CMD);
        return WARN_NO_CMDS;
    }
    return OK;
}

int exec_cmd(cmd_buff_t *cmd) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return ERR_EXEC_CMD;
    } else if (pid == 0) {
        for (int i = 0; i < cmd->argc; i++) {
            if (strcmp(cmd->argv[i], ">") == 0 && i + 1 < cmd->argc) {
                int fd = open(cmd->argv[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd == -1) {
                    perror("open failed");
                    exit(ERR_EXEC_CMD);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
                cmd->argv[i] = NULL;
                break;
            } else if (strcmp(cmd->argv[i], ">>") == 0 && i + 1 < cmd->argc) {
                int fd = open(cmd->argv[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (fd == -1) {
                    perror("open failed");
                    exit(ERR_EXEC_CMD);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
                cmd->argv[i] = NULL;
                break;
            }
        }
        execvp(cmd->argv[0], cmd->argv);
        perror("execvp failed");
        exit(ERR_EXEC_CMD);
    } else {
        int status;
        waitpid(pid, &status, 0);
        return status;
    }
}

int execute_pipeline(command_list_t *clist) {
    int num = clist->num;
    int prev_fd = -1;
    int pipe_fd[2];
    pid_t pids[CMD_MAX];

    for (int i = 0; i < num; i++) {
        if (i < num - 1) {
            if (pipe(pipe_fd) == -1) {
                perror("pipe failed");
                return ERR_EXEC_CMD;
            }
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            return ERR_EXEC_CMD;
        }

        if (pid == 0) {
            if (prev_fd != -1) {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (i < num - 1) {
                close(pipe_fd[0]);
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }

            for (int j = 0; j < num - 1; j++) {
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }

            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            perror("execvp failed");
            exit(ERR_EXEC_CMD);
        } else {
            pids[i] = pid;
            if (prev_fd != -1) close(prev_fd);
            if (i < num - 1) {
                close(pipe_fd[1]);
                prev_fd = pipe_fd[0];
            }
        }
    }

    for (int i = 0; i < num; i++) {
        int status;
        waitpid(pids[i], &status, 0);
    }
    return OK;
}

int exec_local_cmd_loop(char *input) {
    char *input_dup = strdup(input);
    if (!input_dup) {
        perror("strdup failed");
        return ERR_MEMORY;
    }

    command_list_t clist;
    int ret = build_cmd_list(input_dup, &clist);
    free(input_dup);
    if (ret != OK) {
        return ret;
    }

    if (clist.num == 1) {
        ret = exec_cmd(&clist.commands[0]);
    } else {
        ret = execute_pipeline(&clist);
    }

    for (int i = 0; i < clist.num; i++) {
        for (int j = 0; j < clist.commands[i].argc; j++) {
            free(clist.commands[i].argv[j]);
        }
        free(clist.commands[i]._cmd_buffer);
    }
    return ret;
}