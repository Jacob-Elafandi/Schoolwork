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
        fprintf(stderr, "Usage: ./Aggregate_Votes <path>\n");
        exit(-1);
    }
    
    // Open the directory pointed to by the path
    DIR* this_dir = opendir(argv[1]);
    if (this_dir == NULL) {
        fprintf(stderr, "Error: Cannot open directory \"%s\"\n", argv[1]);
        exit(-2);
    }
    
    // Form a linked list of child process pids, one for each subfolder
    pid_node_t* child_pids = NULL;
    
    // Read each entry in the directory
    for (struct dirent* entry = readdir(this_dir); entry != NULL; entry = readdir(this_dir)) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            // don't need to examine entries for current or parent directories
            continue;
        }
        if (entry->d_type == DT_DIR) {
            // Found a subdirectory. Create child process
            // (Symbolic links have a different d_type, so this block won't run for them)
            pid_t pid = fork();
            if (pid < 0) {
                fprintf(stderr, "Error: Forking directory \"%s\" failed\n", entry->d_name);
                exit(-3);
            }
            if (pid) {
                // Place the new pid in the child pid linked list
                pid_node_t* new_head = malloc(sizeof(pid_node_t));
                new_head->pid = pid;
                new_head->next = child_pids;
                child_pids = new_head;
            }
            else {
                // Prevent child process from printing
                int null_output = open("/dev/null", O_WRONLY);
                dup2(null_output, STDOUT_FILENO);
                // Execute Aggregate_Votes recursively on the subdirectory
                char new_path[MAX_FILE_NAME_SIZE];
                strcpy(new_path, argv[1]);
                strcat(new_path, "/");
                strcat(new_path, entry->d_name);
                execl(argv[0], argv[0], new_path, (char*) NULL);
                fprintf(stderr, "Error: ./Aggregate_Votes recursive execution failed\n");
                exit(-4);
            }                
        }
        // If entry is not a directory, don't need to do anything
    }
    if (child_pids == NULL) {
        // No subdirectories were found, so this is a leaf node
        execl("./Leaf_Counter", "./Leaf_Counter", argv[1], (char*) NULL);
        fprintf(stderr, "Error: ./Leaf_Counter execution failed\n");
        exit(-5);
    }
    // If not leaf node, wait for all child processes to finish
    while (child_pids != NULL) {
        wait(&(child_pids->pid));
        pid_node_t* next_node = child_pids->next;
        free(child_pids);
        child_pids = next_node;
    }
    // Make a linked list to store the vote totals in
    vote_node_t* start_node = NULL;
    // Iterate through each subdirectory
    rewinddir(this_dir);
    for (struct dirent* entry = readdir(this_dir); entry != NULL; entry = readdir(this_dir)) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            // don't need to examine entries for current or parent directories
            continue;
        }
        if (entry->d_type == DT_DIR) {
            // Open txt and read it into a buffer
            char** child_votes;
            char txt_path[MAX_FILE_NAME_SIZE];
            strcpy(txt_path, argv[1]);
            strcat(txt_path, "/");
            strcat(txt_path, entry->d_name);
            strcat(txt_path, "/");
            strcat(txt_path, entry->d_name);
            strcat(txt_path, ".txt");
            int fd = open(txt_path, O_RDONLY);
            if (fd < 0) {
                fprintf(stderr, "Error: Couldn't open txt in directory \"%s\"\n", entry->d_name);
                exit(-6);
            }
            char buf[MAX_IO_BUFFER_SIZE];
            if (read(fd, buf, MAX_IO_BUFFER_SIZE) < 0) {
                fprintf(stderr, "Error: Couldn't read txt in directory \"%s\"\n", entry->d_name);
                close(fd);
                exit(-7);
            }
            close(fd);
            // Split txt into tokens
            char* line_break_pos = strchr(buf, '\n');
            if (line_break_pos != NULL) {
                *line_break_pos = '\0';
            }
            makeargv(buf, ":,", &child_votes);
            // Parse tokens
            for (int i = 0; child_votes[i] != NULL; i += 2) {
                if (child_votes[i+1] == NULL) {
                    fprintf(stderr, "Error: Bad txt format in directory \"%s\"\n", entry->d_name);
                    exit(-8);
                }
                if (start_node == NULL) {
                    // first candidate in first txt visited; need to make the first node
                    start_node = malloc(sizeof(vote_node_t));
                    start_node->name = malloc(strlen(child_votes[i]) + 1);
                    strcpy(start_node->name, child_votes[i]);
                    start_node->count = atoi(child_votes[i+1]);
                    start_node->next = NULL;
                }
                else {
                    for (vote_node_t* this_node = start_node; ; this_node = this_node->next) {
                        if (this_node == NULL) {
                            // found a new candidate; need to make a new node
                            vote_node_t* new_start_node = malloc(sizeof(vote_node_t));
                            new_start_node->next = start_node;
                            new_start_node->name = malloc(strlen(child_votes[i]) + 1);
                            strcpy(new_start_node->name, child_votes[i]);
                            new_start_node->count = atoi(child_votes[i+1]);
                            start_node = new_start_node;
                            break;
                        }
                        else if (strcmp(this_node->name, child_votes[i]) == 0) {
                            // this is the node that tallies this candidate's votes
                            this_node->count += atoi(child_votes[i+1]);
                            break;
                        }
                    }
                }
            }
            freemakeargv(child_votes);
        }
    }
    closedir(this_dir);
    
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
