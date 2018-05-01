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
 * Node in a linked list. Represents a candidate*
 * and their number of votes in a region.       *
 ************************************************/
typedef struct vote_node {
	char* name;
	int count;
	struct vote_node* next;
} vote_node_t;

/************************************************
 * Linked list which records the current tallies*
 * for each candidate in a region.              *
 ************************************************/
typedef struct {
    vote_node_t* head;
    char* winner;
    int winner_count;
} vote_list_t;

/************************************************
 * Takes in a file name and creates a vote list *
 * by counting each line as a vote.             *
 ************************************************/
vote_list_t* read_votes_from_file(char* filename) {
    FILE* input = fopen(filename, "r");
    if (input == NULL) {
        return NULL;
    }
    
    vote_list_t* list = malloc(sizeof(vote_list_t));
    list->head = NULL;
    list->winner = NULL;
    list->winner_count = 0;
    
    char buf[256];
    while (1) {
        // Read the next line into the input buffer
        for (int i = 0; ; i++) {
	        char c = fgetc(input);
	        if (c == EOF || c == '\n') {
	            buf[i] = '\0';
	            break;
	        }
	        buf[i] = c;
	    }
	    if (buf[0] == '\0') {
	        // All lines have been read.
	        break;
	    }
	    vote_node_t* node = list->head;
        while (node != NULL && strcmp(node->name, buf)) {
            node = node->next;
        }
        if (node != NULL) {
            node->count++;
        }
        else {
            vote_node_t* new_head = malloc(sizeof(vote_node_t));
            new_head->name = malloc(strlen(buf) + 1);
            strcpy(new_head->name, buf);
            new_head->count = 1;
            new_head->next = list->head;
            list->head = new_head;
            node = new_head;
        }
        if (node->count > list->winner_count) {
            list->winner_count = node->count;
            list->winner = node->name;
        }
    }
    fclose(input);
    return list;
}

/************************************************
 * Takes in a client request and constructs a   *
 * vote list from its data.                     *
 ************************************************/
vote_list_t* read_votes_from_buf(char* buf) {
    
    vote_list_t* vote_list = malloc(sizeof(vote_list_t));
    vote_list->head = NULL;
    vote_list->winner = NULL;
    vote_list->winner_count = 0;
	
	char data[256];
	int buf_index = 19;
	while (buf[buf_index] != '\0') {
	    // Create new node
	    vote_node_t* node = malloc(sizeof(vote_node_t));
	    node->next = vote_list->head;
	    vote_list->head = node;
	    // Extract candidate name
	    int i;
	    for (i = 0; buf[buf_index] != ':'; i++) {
	        data[i] = buf[buf_index];
	        buf_index++;
	    }
	    data[i] = '\0';
	    buf_index++;
	    node->name = malloc(strlen(data) + 1);
	    strcpy(node->name, data);
	    // Extract candidate votes
	    for (i = 0; buf[buf_index] != ',' && buf[buf_index] != '\0'; i++) {
	        data[i] = buf[buf_index];
	        buf_index++;
	    }
	    data[i] = '\0';
	    if (buf[buf_index] != '\0') {
	        buf_index++;
	    }
	    node->count = atoi(data);
	    // Check if new winner
	    if (node->count > vote_list->winner_count) {
	        vote_list->winner = node->name;
	        vote_list->winner_count = node->count;
	    }
	}
	return vote_list;
}

/************************************************
 * Outputs the contents of a vote list in a     *
 * single string which can be sent as data.     *
 ************************************************/
char* get_vote_string(vote_list_t* list) {
    char* result = malloc(256);
    char num_buf[256];
    result[0] = '\0';
    vote_node_t* node = list->head;
    while (node != NULL) {
        sprintf(num_buf, "%d", node->count);
        strcat(result, node->name);
        strcat(result, ":");
        strcat(result, num_buf);
        if (node->next != NULL) {
            strcat(result, ",");
        }
        node = node->next;
    }
    return result;
}

/************************************************
 * Adds one vote list to another. Combines a    *
 * candidate's votes if present in both lists.  *
 ************************************************/
void add_vote_list(vote_list_t* total, vote_list_t* input) {
    if (total == NULL || input == NULL) {
        printf("Error: null arguments to add_votes\n");
        exit(-11);
    }
    for (vote_node_t* input_node = input->head; input_node != NULL; input_node = input_node->next) {
        // Scan through the total list to see if there's a match for the input node
        vote_node_t* total_node = total->head;
        while (total_node != NULL && strcmp(input_node->name, total_node->name)) {
            total_node = total_node->next;
        }
		
		// Add votes to candidate already present...
        if (total_node != NULL) {
            total_node->count += input_node->count;
        }
		// ...or create child node in ancestory and populate
        else {
            total_node = malloc(sizeof(vote_node_t));
            total_node->name = malloc(strlen(input_node->name) + 1);
            strcpy(total_node->name, input_node->name);
            total_node->count = input_node->count;
            total_node->next = total->head;
            total->head = total_node;
        }
        if (total_node->count > total->winner_count) {
            total->winner_count = total_node->count;
            total->winner = total_node->name;
        }
    }
}

/************************************************
 * If removing votes from a list would push any *
 * candidate into the negative, return a string *
 * of all such candidates. Else, return NULL.   *
 ************************************************/
char* validate_removal(vote_list_t* total, vote_list_t* input) {
    // Returns NULL if removal is valid. Otherwise, returns bad candidate names
    char* result = NULL;
    if (total == NULL || input == NULL) {
        printf("Error: null arguments to validate_removal\n");
        exit(-11);
    }
    for (vote_node_t* input_node = input->head; input_node != NULL; input_node = input_node->next) {
        // Scan through the total list to see if there's a match for the input node
        vote_node_t* total_node = total->head;
        while (total_node != NULL && strcmp(input_node->name, total_node->name)) {
            total_node = total_node->next;
        }
		// If candidate is not present or has too few votes, removal is invalid
		if (total_node == NULL || total_node->count < input_node->count) {
		    if (result == NULL) {
		        result = malloc(256);
		        strcpy(result, input_node->name);
		    }
		    else {
		        strcat(result, ",");
		        strcat(result, input_node->name);
		    }
		}
    }
    return result;
}

/************************************************
 * Removes one vote list from another. Assumes  *
 * validate_removal has already been called, so *
 * no candidate will go into the negative.      *
 ************************************************/
void subtract_vote_list(vote_list_t* total, vote_list_t* input) {
    if (total == NULL || input == NULL) {
        printf("Error: null arguments to remove_votes\n");
        exit(-11);
    }
    for (vote_node_t* input_node = input->head; input_node != NULL; input_node = input_node->next) {
        // Scan through the total list to see if there's a match for the input node
        vote_node_t* total_node = total->head;
        while (total_node != NULL && strcmp(input_node->name, total_node->name)) {
            total_node = total_node->next;
        }
		// Subtract votes from candidate already present...
        if (total_node != NULL) {
            total_node->count -= input_node->count;
        }
		// ...or if candidate is not present, something went wrong
		else {
		    printf("Error: invalid subtraction passed to remove_votes\n");
		    exit(-11);
		}
    }
    // Find new winner
    total->winner_count = 0;
    for (vote_node_t* total_node = total->head; total_node != NULL; total_node = total_node->next) {
        if (total_node->count > total->winner_count) {
            total->winner_count = total_node->count;
            total->winner = total_node->name;
        }
    }
}

/************************************************
 * Frees memory allocated to a vote list, all   *
 * its nodes, and all data therein.             *
 ************************************************/
void free_vote_list(vote_list_t* list) {
    if (list == NULL) {
        return;
    }
    vote_node_t* node = list->head;
    while (node != NULL) {
        vote_node_t* new_node = node->next;
        free(node->name);
        free(node);
        node = new_node;
    }
    free(list);
}
