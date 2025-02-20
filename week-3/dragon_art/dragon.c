#include <stdio.h>
#include <string.h>


void print_dragon() {
    const char *dragon =
        "                                                                        @%%%%                        \n"
        "                                                                     %%%%%%                          \n"
        "                                                                    %%%%%%                           \n"
        "                                                                 % %%%%%%%           @               \n"
        "                                                                %%%%%%%%%%        %%%%%%%           \n"
        "                                       %%%%%%%  %%%%@         %%%%%%%%%%%%@    %%%%%%  @%%%%        \n"
        "                                  %%%%%%%%%%%%%%%%%%%%%%      %%%%%%%%%%%%%%%%%%%%%%%%%%%%          \n"
        "                                %%%%%%%%%%%%%%%%%%%%%%%%%%   %%%%%%%%%%%% %%%%%%%%%%%%%%%           \n"
        "                               %%%%%%%%%%%%%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%     %%%            \n"
        "                             %%%%%%%%%%%%%%%%%%%%%%%%%%%%@ @%%%%%%%%%%%%%%%%%%        %%            \n"
        "                            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%%%%                \n"
        "                            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%              \n"
        "                            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%@              \n"
        "      %%%%%%%%@           %%%%%%%%%%%%%%%%        %%%%%%%%%%%%%%%%%%%%%%%%%%      %%                \n"
        "    %%%%%%%%%%%%%         %%@%%%%%%%%%%%%           %%%%%%%%%%% %%%%%%%%%%%%      @%                \n"
        "  %%%%%%%%%%   %%%        %%%%%%%%%%%%%%            %%%%%%%%%%%%%%%%%%%%%%%%                        \n"
        " %%%%%%%%%       %         %%%%%%%%%%%%%             %%%%%%%%%%%%@%%%%%%%%%%%                       \n"
        "%%%%%%%%%@                % %%%%%%%%%%%%%            @%%%%%%%%%%%%%%%%%%%%%%%%%                     \n"
        "%%%%%%%%@                 %%@%%%%%%%%%%%%            @%%%%%%%%%%%%%%%%%%%%%%%%%%%%                  \n"
        "%%%%%%%@                   %%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%              \n"
        "%%%%%%%%%%                  %%%%%%%%%%%%%%%          %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%      %%%%  \n"
        "%%%%%%%%%@                   @%%%%%%%%%%%%%%         %%%%%%%%%%%%@ %%%% %%%%%%%%%%%%%%%%%   %%%%%%%%\n"
        "%%%%%%%%%%                  %%%%%%%%%%%%%%%%%        %%%%%%%%%%%%%      %%%%%%%%%%%%%%%%%% %%%%%%%%%\n"
        "%%%%%%%%%@%%@                %%%%%%%%%%%%%%%%@       %%%%%%%%%%%%%%     %%%%%%%%%%%%%%%%%%%%%%%%  %%\n"
        " %%%%%%%%%%                  % %%%%%%%%%%%%%%@        %%%%%%%%%%%%%%   %%%%%%%%%%%%%%%%%%%%%%%%%% %%\n"
        "  %%%%%%%%%%%%  @           %%%%%%%%%%%%%%%%%%        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  %%% \n"
        "   %%%%%%%%%%%%% %%  %  %@ %%%%%%%%%%%%%%%%%%          %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    %%% \n"
        "    %%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%%%%           @%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    %%%%%%% \n"
        "     %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%              %%%%%%%%%%%%%%%%%%%%%%%%%%%%        %%%   \n"
        "      @%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                  %%%%%%%%%%%%%%%%%%%%%%%%%               \n"
        "        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                      %%%%%%%%%%%%%%%%%%%  %%%%%%%          \n"
        "           %%%%%%%%%%%%%%%%%%%%%%%%%%                           %%%%%%%%%%%%%%%  @%%%%%%%%%         \n"
        "              %%%%%%%%%%%%%%%%%%%%           @%@%                  @%%%%%%%%%%%%%%%%%%   %%%        \n"
        "                  %%%%%%%%%%%%%%%        %%%%%%%%%%                    %%%%%%%%%%%%%%%    %         \n"
        "                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                      %%%%%%%%%%%%%%            \n"
        "                %%%%%%%%%%%%%%%%%%%%%%%%%%  %%%% %%%                      %%%%%%%%%%  %%%@          \n"
        "                     %%%%%%%%%%%%%%%%%%% %%%%%% %%                          %%%%%%%%%%%%%@          \n"
        "                                                                                 %%%%%%%@       \n";
    
    printf("%s\n", dragon);
}


const char compressed_dragon[] = {
    72, '@', 24, '%', 24, ' ', 1, '%', 6, ' ', 7, '%', 7, ' ', 8, '%', 7, ' ', 9, '%', 
    1, ' ', 8, '%', 1, ' ', 6, '@', 1, ' ', 7, '%', 1, ' ', 6, '@', 1, ' ', 5, '%', 
    1, ' ', 9, '%', 1, ' ', 5, '%', 5, '@', 1, ' ', 8, '%', 1, ' ', 6, '@', 1, ' ', 5, '%',
    0 // Null terminator for safety
};


void print_compressed_dragon() {
    const char *p = compressed_dragon;
    while (*p) {
        int count = *p++;
        char c = *p++;
        for (int i = 0; i < count; i++) putchar(c);
        putchar('\n');
    }
}

int main() {
    printf("Full Dragon:\n");
    print_dragon();  // Print the full dragon art

    printf("\nCompressed Dragon:\n");
    print_compressed_dragon();  // Print the compressed dragon art

    return 0;
}
