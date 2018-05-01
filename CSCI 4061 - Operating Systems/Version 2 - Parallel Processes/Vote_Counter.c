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

int find_cycle(char* root, char* dir_name, FILE* output_file);

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./Vote_Counter <path>\n");
        exit(-1);
    }
    
    // Call Aggregate_Votes on the directory pointed to by the path
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Error: Can't fork in root directory \"%s\"\n", argv[1]);
        exit(-2);
    }
    if (pid == 0) {
        // Prevent child process from printing
        int null_output = open("/dev/null", O_WRONLY);
        dup2(null_output, STDOUT_FILENO);
        // Execute Aggregate_Votes
        execl("./Aggregate_Votes", "./Aggregate_Votes", argv[1], (char*) NULL);
        fprintf(stderr, "Error: ./Aggregate_Votes execution from root failed\n");
        exit(-3);
    }
    else {
        wait(&pid);
        
        // Construct name of output file from path
        char* current_dir_name = get_current_dir_name(argv[1]);
        char txt_name[MAX_FILE_NAME_SIZE];
        strcpy(txt_name, argv[1]);
        strcat(txt_name, "/");
        strcat(txt_name, current_dir_name);
        strcat(txt_name, ".txt");
        
        // Open txt and read it into a buffer
        FILE* output_file = fopen(txt_name, "a+");
        if (output_file == NULL) {
            fprintf(stderr, "Error: Cannot open output\n");
            exit(-4);
        }
        char buf[MAX_IO_BUFFER_SIZE];
        if (fread(buf, MAX_IO_BUFFER_SIZE, 1, output_file) < 0) {
            fprintf(stderr, "Error: Cannot read output\n");
            fclose(output_file);
            exit(-5);
        }
        // Split txt into tokens
        char** votes;
        char* line_break_pos = strchr(buf, '\n');
        if (line_break_pos != NULL) {
            *line_break_pos = '\0';
        }
        makeargv(buf, ":,", &votes);
        // Parse tokens: find which candidate got the most votes
        char* winner;
        int max_votes = 0;
        for (int i = 0; votes[i] != NULL; i += 2) {
            if (votes[i+1] == NULL) {
                fprintf(stderr, "Error: Bad output format\n");
                fclose(output_file);
                exit(-6);
            }
            if (atoi(votes[i+1]) > max_votes) {
                max_votes = atoi(votes[i+1]);
                winner = votes[i];
            }
        }
        // Append winner information to end of file
        fprintf(output_file, "Winner:%s\n", winner);
        // Print file name to standard output
        printf("%s\n", txt_name);
        
        // Check for cycle (extra credit) 
        find_cycle(argv[1], argv[1], output_file);
        
        // Tidying up
        freemakeargv(votes);
        fclose(output_file);
        exit(0);
    }
}

int find_cycle(char* root, char* dir_name, FILE* output_file) {
    // Open the directory pointed to by the path
    DIR* this_dir = opendir(dir_name);
    if (this_dir == NULL) {
        fprintf(stderr, "Error: Cannot open directory \"%s\"\n", dir_name);
        exit(-2);
    }
    
    for (struct dirent* entry = readdir(this_dir); entry != NULL; entry = readdir(this_dir)) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            // don't need to examine entries for current or parent directories
            continue;
        }
        if (entry->d_type == DT_DIR) {
            // Found a subdirectory. Execute find_cycle recursively
            char new_path[MAX_FILE_NAME_SIZE];
            strcpy(new_path, dir_name);
            strcat(new_path, "/");
            
            strcat(new_path, entry->d_name);
            find_cycle(root, new_path, output_file);
        }
        else if (entry->d_type == DT_LNK) {
            // Found a symbolic link. Test if it's a cycle
            
            char* new_path = malloc(MAX_FILE_NAME_SIZE * sizeof(char));
            strcpy(new_path, dir_name);
            strcat(new_path, "/");
            strcat(new_path, entry->d_name);
            
            // Read the symbolic link's name into a buffer
            char buf[MAX_FILE_NAME_SIZE];
            int len = readlink(new_path, buf, sizeof(buf) - 1);
            if (len == -1) {
                printf("Error: Cannot open symbolic link at path %s", new_path);
                free(new_path);
                exit(-3);
            }
            buf[len] = '\0';
            
            // If the string is entirely made up of dots and slashes, link is a cycle
            // e.g. "../../../../.." or "."
            strcpy(new_path, dir_name);
            int is_loop = 1;
            for (int i = 0; i < len; i++) {
                if (buf[i] != '.' && buf[i] != '/') {
                    is_loop = 0;
                    break;
                }
                if (buf[i] == '.' && buf[i+1] == '.') {
                    // Move up a level
                    char* new_end = strrchr(new_path, '/');
                    while (new_end != NULL && new_end[1] == '\0') {
                        // Trim trailing slashes
                        *new_end = '\0';
                        new_end = strrchr(new_path, '/');
                    }
                    if (new_end == NULL) {
                        // We've gone up as far as we can. It's not a cycle
                        is_loop = 0;
                        break;
                    }
                    *new_end = '\0';
                }
            }
            if (is_loop) {
                char* cycle_from = get_current_dir_name(dir_name);
                char* cycle_to = get_current_dir_name(new_path);
                fprintf(output_file, "There is a cycle from %s to %s\n", cycle_from, cycle_to);
            }
            
            free(new_path);
        }
    }
    
    closedir(this_dir);
    return 0;
}
