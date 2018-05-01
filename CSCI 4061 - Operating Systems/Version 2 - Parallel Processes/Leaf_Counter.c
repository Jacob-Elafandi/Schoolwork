#define _BSD_SOURCE

#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./Leaf_Counter <path>\n");
        exit(-1);
    }
    // Open the votes.txt in this folder
    char input_path[MAX_FILE_NAME_SIZE];
    strcpy(input_path, argv[1]);
    strcat(input_path, "/");
    strcat(input_path, "votes.txt");
    FILE* input_file = fopen(input_path, "r");
    if (input_file == NULL) {
        printf("Not a leaf node.\n");
        exit(-2);
    }
    // Make a linked list to store the vote totals in
    vote_node_t* start_node = NULL;
    // Make a buffer and read each line of the file into it
    char buf[MAX_IO_BUFFER_SIZE];
    while (fgets(buf, MAX_IO_BUFFER_SIZE, input_file) != NULL) {
        if (buf[0] == '\0' || buf[0] == '\n') {
            // Skip blank line
            continue;
        }
        if (buf[strlen(buf) - 1] == '\n') {
            // Get rid of trailing new line character
            buf[strlen(buf) - 1] = '\0';
        }
        if (start_node == NULL) {
            // first line. need to make the first node
            start_node = malloc(sizeof(vote_node_t));
            start_node->name = malloc(strlen(buf) + 1);
            strcpy(start_node->name, buf);
            start_node->count = 1;
            start_node->next = NULL;
        }
        else {
             for (vote_node_t* this_node = start_node; ; this_node = this_node->next) {
                if (this_node == NULL) {
                    // found a new candidate; need to make a new node
                    vote_node_t* new_start_node = malloc(sizeof(vote_node_t));
                    new_start_node->next = start_node;
                    new_start_node->name = malloc(strlen(buf) + 1);
                    strcpy(new_start_node->name, buf);
                    new_start_node->count = 1;
                    start_node = new_start_node;
                    break;
                }
                else if (strcmp(this_node->name, buf) == 0) {
                    // this is the node that tallies this candidate's votes
                    this_node->count += 1;
                    break;
                }
            }
        }
    }
    fclose(input_file);
    
    // Construct name of output file from path
    char* current_dir_name = get_current_dir_name(argv[1]);
    char new_txt_name[MAX_FILE_NAME_SIZE];
    strcpy(new_txt_name, argv[1]);
    strcat(new_txt_name, "/");
    strcat(new_txt_name, current_dir_name);
    strcat(new_txt_name, ".txt");
    
    // Create output file
    FILE* new_txt = fopen(new_txt_name, "w");
    if (new_txt == NULL) {
        fprintf(stderr, "Error: Couldn't create txt in directory \"%s\"\n", argv[1]);
        exit(-9);
    }
    // Print name of output file, followed by new line character
    // (prints to /dev/null if this process was spawned recursively or by Who_Won)
    printf("%s\n", new_txt_name);
    
    // Write contents of nodes to new txt. Free nodes along the way
    for (vote_node_t* this_node = start_node; ; ) {
        fprintf(new_txt, "%s:%d", this_node->name, this_node->count);
        if (this_node->next == NULL) {
            fprintf(new_txt, "\n");
            break;
        }
        fprintf(new_txt, ",");
        vote_node_t* next_node = this_node->next;
        free(this_node->name);
        free(this_node);
        this_node = next_node;
    }
    fclose(new_txt);
    exit(0);
}
