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
void  reverse_string(char *);
void  word_print(char *);


int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    return 0; //for now just so the code compiles. 
}

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

int count_words(char *buff, int len, int str_len){
    int wc = 0;
    bool word_start = false;

    char *ptr = buff;
    while (*ptr != '\0' && ptr < buff + len) {
        if (word_start == false) {
            if(*ptr == SPACE_CHAR){
                ptr++;
                continue;
            }
            wc++;
            word_start = true;
        }
        else {
            if (*ptr == SPACE_CHAR) {
                word_start = false;
            }
        } 
        ptr++;
    }
return wc;
}
 
  switch(opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        default:
            usage(argv[0]);
            exit(1);
    }