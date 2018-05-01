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

#include "util.h"


// Linked list node used in reading/writing output txt files
typedef struct vote_node {
	char* name;
	int count;
	struct vote_node* next;
} vote_node_t;

// Linked list
typedef struct {
    vote_node_t* head;
    char* winner;
    int winner_count;
} vote_list_t;

/************************************************************
 * Function: count_vote 									*
 * ---------------------------------------------------------*
 * This function increments a candidate's vote count by 1   *
 *	or creates a new node for the candidate if it is their 	*
 *	first vote.												*
 * ---------------------------------------------------------*
 * inputs: list, name										*
 * ---------------------------------------------------------*
 * list: pointer to list containing the candidates and their*
 *	votes													*
 * name: string name of the candidate						*
 ************************************************************/
void count_vote(vote_list_t* list, char* name) {
    if (name == NULL) {
        return;
    }
    if (list == NULL) {
        printf("Error: Attempting to add to a null vote list\n");
        exit(-7);
    }
    vote_node_t* node = list->head;
    while (node != NULL && strcmp(node->name, name)) {
        node = node->next;
    }
    if (node != NULL) {
        node->count++;
    }
    else {
        vote_node_t* new_head = malloc(sizeof(vote_node_t));
        new_head->name = malloc(strlen(name) + 1);
        strcpy(new_head->name, name);
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

/************************************************************
 * Function: read_votes										*
 * ---------------------------------------------------------*
 * This function reads the votes from the file passed in	*
 * ---------------------------------------------------------*
 * inputs: file, buf, buf_size								*
 * ---------------------------------------------------------*
 * file: pointer to the file to be read from		    	*
 * buf: pointer to destination of the read operation		*
 * buf_size: length of buf_size used in determining errors	*
 ************************************************************/
vote_list_t* read_votes(FILE* file, char* buf, int* buf_size) {
    if (file == NULL || buf == NULL) {
        printf("Error: null arguments to read_votes\n");
        exit(-9);
    }
    vote_list_t* vote_list = malloc(sizeof(vote_list_t));
    vote_list->head = NULL;
    vote_list->winner = NULL;
    vote_list->winner_count = 0;
	
	// Parse each line in the file into a vote node
    while (read_line(file, buf, buf_size) == 0) {    
        char* colon = strrchr(buf, ':');
        int count = atoi(colon + 1);
		
        if (count <= 0) {
            // line is "WINNER:<winner>". doesn't represent votes
            continue;
        }
        *colon = '\0';
        vote_node_t* node = malloc(sizeof(vote_node_t));
        node->name = malloc(colon - buf + 1);
        node->count = count;
        strcpy(node->name, buf);
        node->next = vote_list->head;
        vote_list->head = node;
        if (node->count > vote_list->winner_count) {
            vote_list->winner_count = node->count;
            vote_list->winner = node->name;
        }
    }
    return vote_list;
}

/************************************************************
 * Function: write_votes    								*
 * ---------------------------------------------------------*
 * This function writes the vote list to a file specified	*
 * ---------------------------------------------------------*
 * inputs: list, file                                       *
 * ---------------------------------------------------------*
 * list: pointer to the vote list							*
 * file: pointer to the file to be written to			    *
 ************************************************************/
void write_votes(vote_list_t* list, FILE* file) {
    if (list == NULL || file == NULL) {
        printf("Error: null arguments to write_votes\n");
        exit(-10);
    }
    for (vote_node_t* node = list->head; node != NULL; node = node->next) {
        fprintf(file, "%s:%d\n", node->name, node->count);
    }
}

/************************************************************
 * Function: combine_votes									*
 * ---------------------------------------------------------*
 * This function combines the votes of a child list with 	*
 *	its ancestor list. If ancestor doesn't have a    		*
 *	candidate's name, then a new child node is created in 	*
 *	the ancestor list to contain the new candidate.			*
 * ---------------------------------------------------------*
 * inputs: child, ancestor									*
 * ---------------------------------------------------------*
 * child: pointer to child vote list						*
 * ancestor: pointer to ancestor vote list					*
 ************************************************************/
void combine_votes(vote_list_t* child, vote_list_t* ancestor) {
    if (child == NULL || ancestor == NULL) {
        printf("Error: null arguments to combine_votes\n");
        exit(-11);
    }
    for (vote_node_t* child_node = child->head; child_node != NULL; child_node = child_node->next) {
        // Scan through the ancestor list to see if there's a match for the child node
        vote_node_t* ancestor_node = ancestor->head;
        while (ancestor_node != NULL && strcmp(child_node->name, ancestor_node->name)) {
            ancestor_node = ancestor_node->next;
        }
		
		// Add votes to candidate already present...
        if (ancestor_node != NULL) {
            ancestor_node->count += child_node->count;
        }
		// ...or create child node in ancestory and populate
        else {
            ancestor_node = malloc(sizeof(vote_node_t));
            ancestor_node->name = malloc(strlen(child_node->name) + 1);
            strcpy(ancestor_node->name, child_node->name);
            ancestor_node->next = ancestor->head;
            ancestor->head = ancestor_node;
        }
        if (ancestor_node->count > ancestor->winner_count) {
            ancestor->winner_count = ancestor_node->count;
            ancestor->winner = ancestor_node->name;
        }
    }
}

// Free a list and all its nodes
/************************************************************
 * Function: free_vote_list									*
 * ---------------------------------------------------------*
 * This function frees the memory allocated to the vote list*
 *	passed to this function.  This function makes sure to   *
 *	free the sub-elements of the list before finally freeing*
 *	the list itself.										*
 * ---------------------------------------------------------*
 * inputs: list												*
 * ---------------------------------------------------------*
 * list: pointer to the list to be freed from memory		*
 ************************************************************/
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
