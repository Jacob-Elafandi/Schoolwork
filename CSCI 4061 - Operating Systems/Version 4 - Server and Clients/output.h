#define _BSD_SOURCE

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>

#include <stdio.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

/************************************************
 * Turns a message sent from the client into a  *
 * string to be output to the terminal.         *
 ************************************************/
char* get_client_terminal_output(char* message) {
    char* output = malloc(256);
    output[0] = message[0];
    output[1] = message[1];
    output[2] = ' ';
    if (message[3] == '\0') {
        strcpy(output + 3, "(null) (null)");
    }
    if (message[3] == ' ') {
        strcpy(output + 3, "(null) ");  // Shouldn't happen, but safety first
    }
    else {
        int i;
        for (i = 17; i >= 3; i--) {
            if (message[i] != ' ') {
                output[i+1] = ' ';
                output[i+2] = '\0';
                break;
            }
        }
        for ( ; i >= 3; i--) {
            output[i] = message[i];
        }
    }
    if (message[19] == '\0') {
        strcat(output, "(null)");
    }
    else {
        strcat(output, message + 19);
    }
    return output;
}

/************************************************
 * Turns a message sent from the server into a  *
 * string to be output to the terminal.         *
 ************************************************/
char* get_server_terminal_output(char* message) {
    char* output = malloc(256);
    output[0] = message[0];
    output[1] = message[1];
    output[2] = ' ';
    if (message[3] == '\0') {
        strcpy(output + 3, "(null)");
    }
    else {
        strcpy(output + 3, message + 3);
    }
    return output;
}
