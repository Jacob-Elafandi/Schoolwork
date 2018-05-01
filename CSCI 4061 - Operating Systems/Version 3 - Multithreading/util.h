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

#include "dag.h"
#include "pqueue.h"

/* The maximum amount of bytes for a file name */
#define MAX_FILE_NAME_SIZE 255

// Initial size for a buffer (gets doubled every time it isn't long enough)
#define BUF_SIZE_START 1024

// Structure containing information needed to read in leaf nodes
typedef struct {
    dag_node_t* dag_root;
    pqueue_t* queue;
    char* input_dir;
    char* output_dir;
    char* log_txt_path;
    pthread_mutex_t* log_mutex;
} leaf_info_t;

/************************************************************
 * Function: read_line                  					*
 * ---------------------------------------------------------*
 * This function takes a file pointer and reads a line into	*
 *	memory and places the line read into buf.				*
 * ---------------------------------------------------------*
 * inputs: fp, buf, buf_size								*
 * outputs: int												*
 * ---------------------------------------------------------*
 * fp: file pointer											*
 * buf: string pointer destination for read_line			*
 * buf_size: size of string pointer ^^						*
 * int: 0 on success, -1 on failure							*
 ************************************************************/
int read_line(FILE* fp, char* buf, int* buf_size) {
    if (buf == NULL) {
        buf = malloc(BUF_SIZE_START);
        *buf_size = BUF_SIZE_START;
    }
    int length = 0;
    int ret_val = 0;
    char c;
    do {
        c = fgetc(fp);
        if (c == EOF && length == 0) {
            ret_val = -1;
        }
        if (c == EOF || c == '\n') {
            c = '\0';
        }
        if (length >= *buf_size) {
            *buf_size *= 2;
            buf = realloc(buf, *buf_size);
        }
        buf[length] = c;
        length++;
    }
    while (c != '\0');
    return ret_val;
}

/************************************************************
 * Function: decrypt										*
 * ---------------------------------------------------------*
 * This function decrypts the string pointer "line" by		*
 *	shifting all alphabetic characters 2 forward.			*
 * ---------------------------------------------------------*
 * inputs: line												*
 * ---------------------------------------------------------*
 * line: pointer to line being decrypted		    		*
 ************************************************************/
void decrypt(char* line) {
    if (line == NULL) {
        return;
    }
    for (int i = 0; line[i] != '\0'; i++) {
        if ((line[i] >= 'a' && line[i] <= 'x') || (line[i] >= 'A' && line[i] <= 'X')) {
            line[i] += 2;
        }
        else if (line[i] == 'y' || line[i] == 'z' || line[i] == 'Y' || line[i] == 'Z') {
            line[i] -= 24;
        }
    }
}

/************************************************************
 * Function: remove_dir										*
 * ---------------------------------------------------------*
 * This function deletes a directory given by path string.	*
 *	If no directory exists there, nothing happens.			*
 * ---------------------------------------------------------*
 * inputs: path												*
 * ---------------------------------------------------------*
 * path: string pointer to directory being deleted			*
 ************************************************************/
void remove_dir(const char* path) {
    DIR* this_dir = opendir(path);
    if (this_dir == NULL) {
        return;  // No directory to remove
    }
    for (struct dirent* entry = readdir(this_dir); entry != NULL; entry = readdir(this_dir)) {
        if (entry->d_name[0] == '.') {
            // don't need to remove current or parent directory entries
            continue;
        }
        char* file_path = malloc(strlen(path) + strlen(entry->d_name) + 2);
        strcpy(file_path, path);
        strcat(file_path, "/");
        strcat(file_path, entry->d_name);
        if (entry->d_type == DT_DIR) {
            remove_dir(file_path);
        }
        else {
            remove(file_path);
        }
        free(file_path);
    }
    closedir(this_dir);
    remove(path);
}

/**********************************
* 
* Taken from Unix Systems Programming, Robbins & Robbins, p37
* 
*********************************/
int makeargv(const char *s, const char *delimiters, char ***argvp) {
   int error;
   int i;
   int numtokens;
   const char *snew;
   char *t;

   if ((s == NULL) || (delimiters == NULL) || (argvp == NULL)) {
      errno = EINVAL;
      return -1;
   }
   *argvp = NULL;
   snew = s + strspn(s, delimiters);
   if ((t = malloc(strlen(snew) + 1)) == NULL)
      return -1;
   strcpy(t,snew);
   numtokens = 0;
   if (strtok(t, delimiters) != NULL)
      for (numtokens = 1; strtok(NULL, delimiters) != NULL; numtokens++) ;

   if ((*argvp = malloc((numtokens + 1)*sizeof(char *))) == NULL) {
      error = errno;
      free(t);
      errno = error;
      return -1;
   }

   if (numtokens == 0)
      free(t);
   else {
      strcpy(t,snew);
      **argvp = strtok(t,delimiters);
      for (i=1; i<numtokens; i++)
         *((*argvp) +i) = strtok(NULL,delimiters);
   }
   *((*argvp) + numtokens) = NULL;
   return numtokens;
}

/**********************************
* 
* Taken from Unix Systems Programming, Robbins & Robbins, p38
* 
*********************************/
void freemakeargv(char **argv) {
   if (argv == NULL)
      return;
   if (*argv != NULL)
      free(*argv);
   free(argv);
}

char *trimwhitespace(char *str) {
  char *end;
  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;

  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}
