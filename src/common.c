#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#ifdef Linux
#include <bsd/stdlib.h> 
#endif


void printstderr(char *msg)
{
    fprintf(stderr, "%s", msg);
}

int randomint(int max_value){
    // straight out of 'arc4random' manpage
    return arc4random() % ((unsigned)max_value +1 );
}


int randomport(){
    return randomint(65536);
}

void randomip(char* buf){
    //generate 4 octets and string them together 
    char oct1[4];
    char oct2[4];
    char oct3[4];
    char oct4[4];
    random_octet(oct1);
    random_octet(oct2);
    random_octet(oct3);
    random_octet(oct4);
    char ip[20];
    snprintf(ip, 20, "%s.%s.%s.%s", oct1, oct2, oct3, oct4);
    printf("IP will be: %s\n", ip);
    strncpy(buf, ip, 20);
    printf("Buffer is now: %s\n", buf);
}

    
/*
 * Convert integer to a string.
 * Pass in a buffer to be used for the string and the length of the buffer.
 * String will be truncated to fit in the buffer.
 * */
void int_to_str(int int_value, char* str_buf, int buf_len){
    char stri[buf_len];
    snprintf(stri, buf_len, "%d", int_value);
    strncpy(str_buf, stri, buf_len);
}

// copy random octet (char*) into the provided buffer
void random_octet(char* oct_buf){
    int rand_int = randomint(254);
    int_to_str(rand_int, oct_buf, 4); // max size always will be 4 (3 +1)
}

void die(char *msg)
{
    printstderr(msg);
    exit(EXIT_FAILURE);
}

