#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dshlib.h"

// Main entry point for the shell
int main() {
    printf("dsh3> ");

    // Main command loop
    while (1) {
        char input[1024];

        // Read user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\nexiting...\n");
            break;
        }

        // Remove trailing newline
        input[strcspn(input, "\n")] = '\0';

        // Exit command
        if (strcmp(input, "exit") == 0) {
            printf("exiting...\n");
            break;
        }

        // Execute the command loop
        exec_local_cmd_loop(input);
        
        printf("dsh3> ");
    }

    return 0;
}
