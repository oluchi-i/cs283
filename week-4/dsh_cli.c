#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dshlib.h"

#define INPUT_SIZE 1024

int main() {
    char input[INPUT_SIZE];

    while (1) {
        printf("dsh> ");
        if (!fgets(input, INPUT_SIZE, stdin)) {
            break;  // Exit on EOF (CTRL+D)
        }

        input[strcspn(input, "\n")] = '\0';  // Remove newline
        if (strcmp(input, "exit") == 0) {
            break;
        }

        exec_local_cmd_loop(input);
    }

    return 0;
}
