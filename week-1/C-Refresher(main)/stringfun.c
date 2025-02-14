#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFFER_SZ 50
#define SPACE_CHAR ' '

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here
//DONE!
void reverse_string(char *buff, int len);
void  word_print(char *str);
void replace_string(char *buff, int len, const char *old_str, const char *new_str);


int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    //DONE!
    int user_str_len = 0;
    char *ptr = user_str;
    
    // get the length of user_str without using strlen()
    while (*ptr != '\0') {
        user_str_len++;
        ptr++;
    }

    // if the string length exceeds buffer size, return error code -1
    if (user_str_len > len) {
        return -1;
    }

    // Process the user string to copy into the buffer
    char *buff_ptr = buff;
    bool last_was_space = false;  
    ptr = user_str;

    while (*ptr != '\0') {
        // Skip consecutive spaces and replace other whitespace with space
        if (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r') {
            if (!last_was_space) {
                *buff_ptr = ' ';  // Replace with a single space
                buff_ptr++;
                last_was_space = true;
            }
        } else {
            *buff_ptr = *ptr;  // Copy the non-whitespace character
            buff_ptr++;
            last_was_space = false;
        }
        ptr++;
    }

    // Fill the rest of the buffer with '.' characters
    while (buff_ptr < buff + len) {
        *buff_ptr = '.';  // Fill remaining space with dots
        buff_ptr++;
    }

    return user_str_len;  // Return the length of the processed string
}
    //return 0; //for now just so the code compiles. 


void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len) {
    int wc = 0;
    bool word_start = false;
    char *ptr = buff;

    while (*ptr != '\0' && ptr < buff + len) {
        if (word_start == false) {
            if (*ptr == SPACE_CHAR) {
                ptr++;
                continue;
            }
            wc++;
            word_start = true;

        } else {
            if (*ptr == SPACE_CHAR) {
                word_start = false;
            }
        }
        ptr++;
    }
    return wc;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS
//DONE!

void reverse_string(char *buff, int len) {
    char *start = buff;
    char *end = buff + len - 1;
    char tmp_char;

    while (end >= start && (*end == '.' || *end == '\0')) {
        end--;
    }

    while (start < end) {
        tmp_char = *start;
        *start = *end;
        *end = tmp_char;

        start++;
        end--;
    }
    *(buff + len) = '\0';
}

void word_print(char *str) {   // suggested local variables
    int wc = 0;         // counts words
    int wlen = 0;       // length of the current word
    bool word_start = false;    // am I at the start of a new word
    char curr;    // current character in the string

    // Pointer to iterate through the string
    char *ptr = str;

    while (*ptr != '\0' && *ptr != '.') {
        if (word_start == false) {  
            if (*ptr != SPACE_CHAR) {
                wc++;  
                word_start = true;
                wlen = 1;  
                printf("%d. %c", wc, *ptr);  
            }

        } else {  
            if (*ptr != SPACE_CHAR) {
                curr = *ptr;
                printf("%c", curr);  
                wlen++;  

            } else {  
                printf(" (%d)\n", wlen);  
                word_start = false;
                wlen = 0; 
            }
        }
        ptr++;  
    }

    if (word_start == true) {
        printf(" (%d)\n", wlen);
    }
}

void replace_string(char *buff, int len, const char *old_str, const char *new_str) {
    char *pos = buff;
    const char *old_ptr;
    int old_len = 0, new_len = 0;

    // Calculate the lengths of old_str and new_str
    while (old_str[old_len] != '\0') old_len++;
    while (new_str[new_len] != '\0') new_len++;

    // Find the substring in the buffer
    while (*pos != '\0') {
        old_ptr = old_str;

        // Check for a match
        char *match_start = pos;
        while (*pos == *old_ptr && *old_ptr != '\0') {
            pos++;
            old_ptr++;
        }

        // If the whole old_str was found
        if (*old_ptr == '\0') {
            int diff = new_len - old_len;

            // If new_str is longer, make room
            if (diff > 0) {
                char *end = buff + len - 1;
                while (end >= pos) {
                    *(end + diff) = *end;
                    end--;
                }
            }

            // Insert new_str
            for (int i = 0; i < new_len; i++) {
                match_start[i] = new_str[i];
            }

            // If new_str is shorter, shift remaining characters left
            if (diff < 0) {
                char *shift_start = match_start + new_len;
                char *shift_end = pos;
                while (*shift_end != '\0') {
                    *shift_start = *shift_end;
                    shift_start++;
                    shift_end++;
                }
                while (shift_start < buff + len) {
                    *shift_start = '.';
                    shift_start++;
                }
            }
            return;  // Only replace the first occurrence
        }

        pos = match_start + 1;  // Move to the next character
    }

    // If old_str was not found, print error
    printf("Error: Word '%s' not found in buffer.\n", old_str);
}


int main(int argc, char *argv[]) {

    char *buff;             //placeholder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    // It is safe because the program expects an option flag, to specify the functionality.
    //If argv[1] were missing, the program will print the usage message, then exit with an error code.
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //This check makes sure the user has given both an option 
    //(like -r or -c) and a string to work with. Without both, 
    //the program cannot perform any action, so it stops and 
    //shows the usage message.

    //It ensures that the program has all the information 
    //it needs to do its job. If the user forgets to provide 
    //one of the required things, the program won’t try to 
    //do something incomplete.
    //
    if ((opt == 'c' || opt == 'r' || opt == 'w') && argc < 3) {
        usage(argv[0]);
        exit(1);
    }
    if (opt == 'x' && argc < 5) {
        usage(argv[0]);
        exit(1);
    }
    
    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    //DONE!
    buff = (char *)malloc(BUFFER_SZ * sizeof(char));  // Allocate memory for the buffer
    if (buff == NULL) {
        printf("Memory allocation failed\n");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        free(buff);
        exit(2);
    }

    char *start = buff; 

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                free(buff);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        case 'r':
        printf("Reversed String: ");
        reverse_string(buff, user_str_len);  // Pass the actual string length, not the buffer size.
    
        // Print only the valid characters (ignore the filler dots)
        char *start = buff;
        while (*start != '\0') {
            if (*start != '.') {  // Skip the filler dots
                putchar(*start);
            }
            start++;
        }
        putchar('\n');
        break;
        
        case 'w':
        printf("Word Print\n");
        printf("----------\n");
        word_print(buff);
        break; 

        case 'x':
        if (argc < 5) {
            printf("Error: You need to provide both the old and new strings for replacement.\n");
            usage(argv[0]);
            free(buff);
            exit(1);
        }

        replace_string(buff, BUFFER_SZ, argv[3], argv[4]);  // Replace the first occurrence
        printf("Modified String: ");
        start = buff;
        while (*start != '\0') {
            if (*start != '.') {  // Skip the filler dots
                putchar(*start);
            }
            start++;
        }
        putchar('\n');
        break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        //DONE!

    default:
        usage(argv[0]);
        free(buff);
        exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    //DONE!
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
//
//          Providing both helps the program know exactly what part of 
//          the memory to use and how much space is available. This prevents
//          the program from accidentally going over the memory limit or working
//          with random junk data in unused parts of memory.
//
//          Knowing where the data is and where it ends, prevents crashes and bugs
//          from trying to use too much space or incorrect data.
//