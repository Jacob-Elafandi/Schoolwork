/*login: elafa009
  date: 07/10/18
  name: Jacob Elafandi, Joshua Pratt
  id: 4457156, 4975616
  Extra credits: Yes
*/

#define _BSD_SOURCE

#include "votelist.h"

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
#include <pthread.h>

/************************************************************
 * Function: handle_leaves  								*
 * ---------------------------------------------------------*
 * This threadsafe function continually pulls input files   *
 * from a priority queue. It decrypts and counts their      *
 * votes, then aggregates them to all ancestor regions.     *
 * ---------------------------------------------------------*
 * STEP 1: 													*
 *	Typecast arg to leaf_info_t								*
 *	Create dynamic buffer									*
 * 															*
 * STEP 2: 													*
 *	Lock queue mutex										*
 *	Dequeue pqueue_node_t									*
 *	Unlock queue mutex										*
 *	Exit if node is NULL									*
 * 															*
 * STEP 3: 													*
 *	Assign input_path										*
 *	Open input_file											*
 *	Free input_path											*
 *	Get dag_node_t using get_dag_node function				*
 * 															*
 * STEP 4: 													*
 *	Lock log mutex											*
 *	Append "start" status to log file if no error in fopen	*
 *	Unlock log mutex										*
 * 															*
 * STEP 5: 													*
 *	Create linked list vote_list_t							*
 * 															*
 * STEP 6: 													*
 *	Read line of input file & check	for End-Of-Line (EOL)	*
 *	Decrypt line											*
 *	Aggregate vote to file									*
 *	Add votes to vote_list									*
 * 															*
 * STEP 7: 													*
 *	Lock node mutex											*
 *	Aggregate votes if file exists							*
 *	Write votes to file										*
 *	Free local memory										*
 *	Unlock node mutex										*
 * 															*
 * STEP 8: 													*
 *	Lock log mutex											*
 *	Append "end" status to log file if no error in fopen	*
 *	Unlock log mutex										*
 * 															*
 * STEP 9: 													*
 *	Free allocated memory									*
 * ---------------------------------------------------------*
 * inputs: arg												*
 * ---------------------------------------------------------*
 * arg: pointer to leaf_info								*
 ************************************************************/
void* handle_leaves(void* arg) {
    //--------------------STEP 1: Set up leaf info, create buffer------------------------
    leaf_info_t* leaf_info = arg;
    char* buf = malloc(BUF_SIZE_START);
    int* buf_size = malloc(sizeof(int));
    *buf_size = BUF_SIZE_START;
    //-----------------------------------------------------------------------------------
	
    while (1) {
        //-----------------------STEP 2: Extract node from pqueue----------------------------
        pthread_mutex_lock(leaf_info->queue->mutex);
        pqueue_node_t* queue_node = pop_pqueue_node(leaf_info->queue);
        pthread_mutex_unlock(leaf_info->queue->mutex);
        if (queue_node == NULL) {
            // pqueue is empty, so all leaves have been handled. we're done
            free(buf);
            free(buf_size);
            return NULL;
        }
        //-----------------------------------------------------------------------------------
		
		//----------STEP 3: Open input file and get its corresponding DAG node---------------
        char* input_path = malloc(strlen(leaf_info->input_dir) + strlen(queue_node->file_name) + 2);
        strcpy(input_path, leaf_info->input_dir);
        strcat(input_path, "/");
        strcat(input_path, queue_node->file_name);
        FILE* input_file = fopen(input_path, "r");
        
		
        // Print error message to user if input_file is invalid
		if (input_file == NULL) {
			printf("input_file is undefined. Given string:%s\n",input_path);
            continue;
        }
        dag_node_t* dag_node = get_dag_node(queue_node->node_name, leaf_info->dag_root);
		
        // input_file isn't defined in DAG.txt
		if (dag_node == NULL) {    
            fclose(input_file);
            continue;
        }
		
		free(input_path);
        //-----------------------------------------------------------------------------------
		
		//-------------------STEP 4: Write start message to log file-------------------------
        pthread_mutex_lock(leaf_info->log_mutex);
        FILE* log_txt = fopen(leaf_info->log_txt_path, "a");
        if (log_txt == NULL) {
            pthread_mutex_unlock(leaf_info->log_mutex);
            printf("Error: Could not open log.txt\n");
            exit(-6);
        }
        fprintf(log_txt, "%s:%d:start\n", queue_node->node_name, pthread_self());
        fclose(log_txt);
        pthread_mutex_unlock(leaf_info->log_mutex);
        //-----------------------------------------------------------------------------------
		
		//--------------------STEP 5: Create linked list vote_list_t-------------------------
        vote_list_t* leaf_vote_list = malloc(sizeof(vote_list_t));
        leaf_vote_list->head = NULL;
        leaf_vote_list->winner = NULL;
        leaf_vote_list->winner_count = 0;
        //-----------------------------------------------------------------------------------
		
		//------------------------STEP 6: Decrypt and count votes----------------------------
        char* output_path = malloc(strlen(leaf_info->output_dir) + strlen(queue_node->file_name) + 2);
        strcpy(output_path, leaf_info->output_dir);
        strcat(output_path, "/");
        strcat(output_path, queue_node->file_name);
        FILE* output_file = fopen(output_path, "w");
        if (output_file == NULL) {
            printf("Error: Could not create output file \"%s\"\n", output_path);
            exit(-8);
        }
        while (read_line(input_file, buf, buf_size) == 0) {
            decrypt(buf);
            fprintf(output_file, "%s\n", buf);
            count_vote(leaf_vote_list, buf);
        }
        fclose(input_file);
        fclose(output_file);
        free(output_path);
        //-----------------------------------------------------------------------------------
		
		//-------STEP 7: Aggregate leaf's votes into each of its ancestors' votes------------
        for ( ; dag_node != NULL; dag_node = dag_node->parent) {
            if (dag_node->mutex != NULL) {
                // Ensures mutual exclusion
                pthread_mutex_lock(dag_node->mutex);
            }
            output_path = malloc(strlen(dag_node->folder_path) + strlen(dag_node->name) + 6);
            strcpy(output_path, dag_node->folder_path);
            strcat(output_path, "/");
            strcat(output_path, dag_node->name);
            strcat(output_path, ".txt");
            
            output_file = fopen(output_path, "r");
            vote_list_t* ancestor_vote_list;
			
			// If file already exists, read votes from it and merge with leaf's votes
            if (output_file != NULL) {    
                ancestor_vote_list = read_votes(output_file, buf, buf_size);
                fclose(output_file);
                combine_votes(leaf_vote_list, ancestor_vote_list);
            }
			
			// No votes to read, so ancestor's votes are just leaf's votes
            else {    
                ancestor_vote_list = leaf_vote_list;
            }
            
            // Write merged votes to file
            output_file = fopen(output_path, "w");
            write_votes(ancestor_vote_list, output_file);
            
            // If we're at the root, append the winner
            if (dag_node->parent == NULL) {
                fprintf(output_file, "WINNER:%s", ancestor_vote_list->winner);
            }
            
            if (ancestor_vote_list != leaf_vote_list) {
                free_vote_list(ancestor_vote_list);
            }
            
            fclose(output_file);
            free(output_path);
            if (dag_node->mutex != NULL) {
                pthread_mutex_unlock(dag_node->mutex);
            }
        }
        //-----------------------------------------------------------------------------------
		
		//---------------------STEP 8: Write end message to log file-------------------------
        pthread_mutex_lock(leaf_info->log_mutex);
        log_txt = fopen(leaf_info->log_txt_path, "a");
        if (log_txt == NULL) {
            pthread_mutex_unlock(leaf_info->log_mutex);
            printf("Error: Could not open log.txt\n");
            exit(-6);
        }
        fprintf(log_txt, "%s:%d:end\n", queue_node->node_name, pthread_self());
        fclose(log_txt);
        pthread_mutex_unlock(leaf_info->log_mutex);
        //-----------------------------------------------------------------------------------
		
		//-------------------------STEP 9: Free allocated memory-----------------------------
        free_pqueue_node(queue_node);
        free_vote_list(leaf_vote_list);
		//-----------------------------------------------------------------------------------
    }
}

/************************************************************
 * Function: main           								*
 * ---------------------------------------------------------*
 * This is the main function of the votecounter program.    *
 * It creates all necessary threads and data structures to  *
 * count and aggregate all votes in a DAG.                  *
 * ---------------------------------------------------------*
 * 															*
 * STEP 1: 													*
 *	Check for correct number of arguments & values			*
 * 															*
 * STEP 2: 													*
 *	Delete output_dir if it exists							*
 *	Create new copy of output_dir							*
 * 															*
 * STEP 3: 													*
 *	Open DAG.txt											*
 *	Read nodes from DAG.txt									*
 *	Create root node & populate								*
 *	Create & populate child nodes 							*
 * 															*
 * STEP 4: 													*
 *	Create priority queue   								*
 * 															*
 * STEP 5: 													*
 *	NAME_p_INT filenames insert NAME with a priority of INT	*
 *		into the queue.										*
 *	All other filenames insert filename with lowest priority*
 *		into the queue.										*
 * 															*
 * STEP 6: 													*
 *	Create log file (overwrites if necessary)				*
 * 															*
 * STEP 7: 													*
 *	Create threads											*
 *	Initialize threads										*
 *	Execute threads											*
 * 															*
 * STEP 8: 													*
 *	Free allocated memory									*
 * ---------------------------------------------------------*
 * inputs: argc, argv										*
 * ---------------------------------------------------------*
 * argc: int size of argv string array						*
 * 															*
 * argv: pointer to array of string pointers containing 	*
 *	input parameters from the command line.					*
 ************************************************************/
int main(int argc, char** argv) {
	
	//-----------------------STEP 1: Verify argument fidelity----------------------------
    if (argc < 4 || argc > 5) {
        printf("Usage: ./votecounter DAG.txt <input_dir> <output_dir> <num_threads>\n");
        exit(-1);
    }
    int num_threads = 4;  // Default value if none given
    if (argc == 5) {
        num_threads = atoi(argv[4]);
        if (num_threads <= 0) {
            printf("Error: num_threads must be a positive integer\n");
            exit(-1);
        }
    }
    //-----------------------------------------------------------------------------------
	
	//------------------------STEP 2: Create empty output_dir----------------------------
    remove_dir(argv[3]);
    mkdir(argv[3], 0700);
    //-----------------------------------------------------------------------------------
	
    //------------------------STEP 3: Create and populate DAG----------------------------
    FILE* dag_txt = fopen(argv[1], "r");
    if (dag_txt == NULL) {
        printf("Error: Couldn't open file: %s\n", argv[1]);
        exit(-1);
    }
    dag_node_t* dag_root = NULL;
    char* buf = malloc(BUF_SIZE_START);
    int* buf_size = malloc(sizeof(int));
    *buf_size = BUF_SIZE_START;
	
    // Read each line of DAG.txt with a dynamically-sized buffer
    while (read_line(dag_txt, buf, buf_size) == 0) {
        char** tokens;
        int num_tokens = makeargv(buf, ":", &tokens);
        dag_node_t* node = get_dag_node(tokens[0], dag_root);
		
        if (node == NULL) {    
            if (dag_root != NULL) {
                printf("Error: attempting to define child before parent in DAG\n");
                exit(-2);
            }
			
            // DAG is empty. Create root node and populate
            dag_root = malloc(sizeof(dag_node_t));
            node = dag_root;
            dag_root->name = malloc(strlen(tokens[0]) + 1);
            strcpy(dag_root->name, tokens[0]);
            dag_root->parent = NULL;
			
            // Construct folder path by appending '/' and name to output_dir
            dag_root->folder_path = malloc(strlen(argv[3]) + strlen(tokens[0]) + 2);
            strcpy(dag_root->folder_path, argv[3]);
            strcat(dag_root->folder_path, "/");
            strcat(dag_root->folder_path, tokens[0]);
            mkdir(dag_root->folder_path, 0700);
            
        }
        node->mutex = malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(node->mutex, NULL);
		
        // Create child nodes
        node->num_children = num_tokens - 1;
        node->children = malloc(sizeof(dag_node_t) * (num_tokens - 1));
		
		// Populate child nodes
        for (int i = 0; i < num_tokens - 1; i++) {
            
            node->children[i].name = malloc(strlen(tokens[i+1]) + 1);
            strcpy(node->children[i].name, tokens[i+1]);
            node->children[i].mutex = NULL;
            node->children[i].num_children = 0;
            node->children[i].children = NULL;
            node->children[i].parent = node;
			
            // Construct folder path by appending '/' and name to parent's folder path
            node->children[i].folder_path =
              malloc(strlen(node->folder_path) + strlen(tokens[i+1]) + 2);
            strcpy(node->children[i].folder_path, node->folder_path);
            strcat(node->children[i].folder_path, "/");
            strcat(node->children[i].folder_path, tokens[i+1]);
            mkdir(node->children[i].folder_path, 0700);
        }
        freemakeargv(tokens);
    }
    fclose(dag_txt);
    //-----------------------------------------------------------------------------------
	
	//-------------------------STEP 4: Create priority queue-----------------------------
    pqueue_t* queue = malloc(sizeof(pqueue_t));
    queue->leftmost = NULL;
    queue->rightmost = NULL;
    queue->mutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(queue->mutex, NULL);
    //-----------------------------------------------------------------------------------
	
	//-------------------STEP 5: Insert files into priority queue------------------------
    DIR* input_dir = opendir(argv[2]);
    if (input_dir == NULL) {
        printf("Error: Cannot open directory \"%s\"\n", argv[2]);
        exit(-4);
    }
    for (struct dirent* entry = readdir(input_dir); entry != NULL; entry = readdir(input_dir)) {
        // Ignore directories starting with '.'
		if (entry->d_name[0] == '.') {
            continue;
        }
        
        char* last_uscore_char = strrchr(entry->d_name, '_'); // address of last '_'
        if (last_uscore_char == NULL) {
            insert_pqueue_node(entry->d_name, 2147483647, -1, queue);
        }
        else {
            int last_uscore = strrchr(entry->d_name, '_') - entry->d_name;  // index of last '_'
			
			// Final underscore is last character or less than 3 characters preceding
            if (last_uscore < 3 || last_uscore >= strlen(entry->d_name) - 1) {
				insert_pqueue_node(entry->d_name, 2147483647, -1, queue);
            }
			
			// Final underscore is not preceded by "_p"
            else if (entry->d_name[last_uscore-1] != 'p' || entry->d_name[last_uscore-2] != '_') {
                insert_pqueue_node(entry->d_name, 2147483647, -1, queue);
            }
			
			// atoi returns 0 on failure, so check specifically.
            else if (strcmp(last_uscore_char + 1, "0") == 0) {
                insert_pqueue_node(entry->d_name, 0, last_uscore, queue);
            }
            else {
				int priority = atoi(last_uscore_char + 1);
				
				// Int cast failed. Format is NAME_p_NONINT
                if (priority == 0) {    
                    insert_pqueue_node(entry->d_name, 2147483647, -1, queue);
                }
				
				// Int cast successful. Format is NAME_p_INT
                else {
                    insert_pqueue_node(entry->d_name, priority, last_uscore, queue);
                }
            }
        }
    }
    closedir(input_dir);
    if (queue->leftmost == NULL) {
        printf("error: input directory is empty\n");
        exit(-5);
    }
    //-----------------------------------------------------------------------------------
	
	//-----------------------------STEP 6: Create log.txt--------------------------------
    char* log_txt_path = malloc(strlen(argv[3]) + 9);
    strcpy(log_txt_path, argv[3]);
    strcat(log_txt_path, "/log.txt");
    FILE* log_txt = fopen(log_txt_path, "w");   // Wipes file if already present
    fclose(log_txt);
    //-----------------------------------------------------------------------------------
	
	//-----------------------STEP 7: Create and launch threads---------------------------
    pthread_t pool[num_threads];
    leaf_info_t* leaf_info = malloc(sizeof(leaf_info_t));
    leaf_info->dag_root = dag_root;
    leaf_info->queue = queue;
    leaf_info->input_dir = argv[2];
    leaf_info->output_dir = argv[3];
    leaf_info->log_txt_path = log_txt_path;
    leaf_info->log_mutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(leaf_info->log_mutex, NULL);
    // Launch threads
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&pool[i], NULL, handle_leaves, (void*) leaf_info);
    }
    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(pool[i], NULL);
    }
    
	//-------------------------STEP 8: Free allocated memory-----------------------------
    free(buf);
    free(buf_size);
    free_dag(dag_root);
    free_pqueue(queue);
    free(log_txt_path);
    free(leaf_info->log_mutex);
    free(leaf_info);
	//-----------------------------------------------------------------------------------
}
