/* login: elafa009
 * date: 02/19/17
 * name: Jacob_Elafandi, Joshua_Pratt
 * id: 4457156, 4975616 */

/*
 * VCforStudents.c
 *
 *  Created on: Feb 2, 2018
 *      Author: ayushi
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include "makeargv.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX_NODES 100
#define LINE_LENGTH 1024
#define INPUT_SIZE 50

//Function signatures
int parseInputLine(char *s, node_t *n);
void execNodesHelper(node_t *n, node_t * firstnode);

/**Function : parseInput
 * Arguments: 'filename' - name of the input file
 * 			  'n' - Pointer to Nodes to be allocated by parsing
 * Output: 0 if successful, -1 if failure
 * 
 * This function opens the input file and reads it line by line.
 * First, it creates an array of the candidates' names;
 * next, it calls parseInputLine on every other line to create a DAG.
 * 
 * When the DAG has been created, the function examines every node to determine
 * which program should be assigned to it.
 *   - The root node, identified by its parent ID of -1, is assigned ./find_winner, which
 *      tabulates the votes from its children's output files and records the overall winner.
 *   - The branch nodes, identified by a positive parent ID and a positive number of children,
 *      are assigned ./aggregate_votes, which is like find_winner but does not create a line
 *      in output saying who won (since it acts only on a subtree).
 *   - The leaf nodes, identified by a positive parent ID and zero children, are assigned
 *      ./leafcounter, which tabulates the votes in the one associated data file.
 */
int parseInput(char *filename, node_t *n) {
    FILE* file_pointer = file_open(filename);
    char* buf = malloc(LINE_LENGTH);
    
    // First line of file contains candidate names; store in array
    buf = read_line(buf, file_pointer);
    char** candidates;
    makeargv(buf, " \n", &candidates);
    
    do {
        // Parse the next line and abort if something went wrong
        if (parseInputLine(buf, n) < 0) {
            fclose(file_pointer);
            free(buf);
            free(candidates);
            return -1;
        }
        read_line(buf, file_pointer);
    }
    while(!feof(file_pointer));  // while not at end of file
    
    // For each other node, assign appropriate program
    for (node_t* temp = n; temp->id != -1; temp++) {
        if (temp->parent_id == -1) {
            // Assign find_winner to root node
            strcpy(n->prog, "./find_winner");
            strcpy(n->input[0], "./find_winner");
            sprintf(n->input[1], "%d", n->num_children);
            for (int i = 0; i < n->num_children; i++) {
                strcpy(n->input[2+i], findNodeByID(n, n->children[i])->output);
            }
            strcpy(n->input[2 + n->num_children], n->output);
            for (int j = 0; ; j++) {
                if (candidates[j] == NULL) {
                    n->input[3 + n->num_children + j] = NULL;
                    break;
                }
                strcpy(n->input[3 + n->num_children + j], candidates[j]);
            }
        }
        else if (temp->num_children == 0) {
            // Assign leafcounter
            strcpy(temp->prog, "./leafcounter");
            strcpy(temp->input[0], "./leafcounter");
            strcpy(temp->input[1], temp->name);
            strcpy(temp->input[2], temp->output);
            for (int j = 0; ; j++) {
                if (candidates[j] == NULL) {
                    temp->input[3 + temp->num_children + j] = NULL;
                    break;
                }
                strcpy(temp->input[3 + j], candidates[j]);
            }
        }
        else {
            // Assign aggregate_votes
            strcpy(temp->prog, "./aggregate_votes");
            strcpy(temp->input[0], "./aggregate_votes");
            sprintf(temp->input[1], "%d", temp->num_children);
            for (int i = 0; i < temp->num_children; i++) {
                strcpy(temp->input[2+i], findNodeByID(n, temp->children[i])->output);
            }
            strcpy(temp->input[2 + temp->num_children], temp->output);
            for (int j = 0; ; j++) {
                if (candidates[j] == NULL) {
                    temp->input[3 + temp->num_children + j] = NULL;
                    break;
                }
                strcpy(temp->input[3 + temp->num_children + j], candidates[j]);
            }
        }
    }
     
    fclose(file_pointer);
    free(buf);
    free(candidates);
    return 0;
}
    

/**Function : parseInputLine
 * Arguments: 's' - Line to be parsed
 * 			  'n' - Pointer to Nodes to be allocated by parsing
 * Output: 0 if successful, -1/-2/-3 if failure (depending on error type)
 * 
 * This function uses makeargv to separate the input line into tokens.
 * By the format of the tokens, it identifies the line as one of four possibilities:
 *  - Blank line: do nothing and return
 *  - Line with candidates' names: handled in parseInput, so do nothing and return
 *  - Line defining all node names: Create a node for each string. Allocate the memory
 *     for the node's input, set its name, output, and ID, and set the number of children
 *     and the parent ID to their default respective values of 0 and -1.
 *  - Line defining a node's children: For each child, place its ID in the parent's
 *     list of child IDs, and set its parent ID. If a child is also an ancestor of the
 *     parent, or already has a parent ID, abort and return a failure code.
 * */
int parseInputLine(char *s, node_t *n) {
    char** tokens;
    int num_tokens = makeargv(s, " \n", &tokens);
    // Test for empty or commented line
    if (num_tokens == 0 || tokens[0][0] == '#') {
        return 0;
    }
    // Test for line containing information of candidates
    else if (strspn(tokens[0], "1234567890") > 0) {
        // This line is handled in parseInput
        return 0;
    }
    // Test for line containing all node names
    else if (num_tokens < 2 || strcmp(tokens[1], ":") != 0) {
        // Create a node for each token in the line
        for (int token_index = 0; tokens[token_index] != NULL; token_index++) {
            // 'input' is a 2D char array that will get passed to execv
            (n + token_index)->input = malloc(INPUT_SIZE * sizeof(char*));
            for (int i = 0; i < INPUT_SIZE; i++) {
                (n + token_index)->input[i] = malloc(LINE_LENGTH * sizeof(char));
            }
            strcpy((n + token_index)->name, tokens[token_index]);
            strcpy((n + token_index)->output, tokens[token_index]);
            prepend((n + token_index)->output, "Output_");
            (n + token_index)->num_children = 0;  // Default; may get incremented later
            (n + token_index)->id = token_index + 1;  // Unique for every node
            (n + token_index)->parent_id = -1;  // Will get set for every non-root node later
        }
        (n + num_tokens)->id = -1;
        return 0;
    }
    // All other lines (define DAG's structure)
    node_t* parent = findnode(n, tokens[0]);
    if (parent->num_children != 0) {
        printf("Error: More than one line in input defines children for node '%s'\n", parent->name);
        return -1;
    }
    parent->num_children = num_tokens - 2;
    for (int index = 2; index < num_tokens; index++) {
        node_t* child = findnode(n, tokens[index]);
        // Check if child is also an ancestor
        node_t* ancestor = parent;
        while (ancestor->parent_id != -1) {
            ancestor = findNodeByID(n, ancestor->parent_id);
            if (ancestor == child) {
                printf("Error: Loop detected - node '%s' is both child and ancestor of node '%s'\n",
                        ancestor->name, parent->name);
                return -2;
            }
        }
        // Check if child has two parents
        if (child->parent_id != -1) {
            printf("Error: Node '%s' is child of both '%s' and '%s'\n",
                   child->name, parent->name, findNodeByID(n, child->parent_id)->name);
            return -3;
        }
        child->parent_id = parent->id;
        parent->children[index - 2] = child->id;
    }
    return 0;
}

/**Function : execNodes
 * Argument: 'n' - Pointer to first node to be executed
 *
 * This function calls its helper to execute all the nodes' programs. It does so
 * in a fork so as to ensure that the deallocation code in main gets executed
 * after the winner has been found.
 * */
void execNodes(node_t *n) {
    pid_t pid = fork();
    if (pid == 0) {
        execNodesHelper(n, n);
    }
    else {
        wait(&pid);
    }
}


/**Function : execNodesHelper
 * Arguments: 'n' - Pointer to root of subtree to be executed
 *            'firstnode' - Pointer to root of entire tree
 * 
 * If 'n' has children, this function forks and recursively calls them all so that their
 * processes, which are independent, run in parallel. It then waits for every child
 * process to finish before tabulating its children's votes.
 * If 'n' doesn't have children, this function calls the program straight away.
 * */
void execNodesHelper(node_t *n, node_t * firstnode) {
    // "firstnode" used in finding nodes by ID
    if (n->num_children > 0) {
        // Execute all children and store their pid's in a list
        pid_t child_pids[10];
        for (int i = 0; i < n->num_children; i++) {
            pid_t pid = fork();
            if (pid == 0) {
                execNodesHelper(findNodeByID(firstnode, n->children[i]), firstnode);
                exit(0);
            }
            else {
                child_pids[i] = pid;
            }
        }
        // Wait for all children to finish executing
        for (int i = 0; i < n->num_children; i++) {
            wait(&child_pids[i]);
        }
    }
    // Redirect output to appropriate file and execute appropriate program
    int out = open(n->output, O_CREAT | O_RDONLY);
    chmod(n->output, 0700);  // User can read, write, and execute
    dup2(out, 1);
    execv(n->prog, n->input);
}


int main(int argc, char **argv){

	// Allocate space for MAX_NODES to node pointer
	struct node* mainnodes=(struct node*)malloc(sizeof(struct node)*MAX_NODES);
	
	// Correct program usage?
	if (argc != 2){
		printf("Usage: %s Program\n", argv[0]);
		free(mainnodes);
		return -1;
	}
	
	int return_value;
	
	// Call parseInput
	if (parseInput(argv[1], mainnodes) == 0) {
	    // Call execNodes on the root node
	    execNodes(mainnodes);
	    return_value = 0;
	}
	else {
	    // parseInput failed. Need to deallocate memory and abort
	    return_value = -2;
	}
	
	// Tidying up
	for (node_t* n = mainnodes; n->id != -1; n++) {
	    if (n->input == NULL) {
	        continue;
	    }
	    for (int i = 0; n->input[i] != NULL; i++) {
	        free(n->input[i]);
	    }
	    free(n->input);
	}
	free(mainnodes);
	return return_value;
}
