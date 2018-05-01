#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include "dag.h"
#include "output.h"

/************************************************
 * A struct which contains all the information  *
 * needed to write to the client. This is what  *
 * is passed to the thread function.            *
 ************************************************/
typedef struct {
    dag_node_t* dag_root;
    int sock;
    char* ip;
    int port;
} client_info_t;

/************************************************
 * Handle the Return_Winner command (RW).       *
 * If all polls are closed, find and return     *
 * the winner. Otherwise, return "No votes."    *
 ************************************************/
void return_winner(char* buf, dag_node_t* dag_root) {
    if (dag_root->status != 2) {
        // Polls have not been closed in all areas
        // Find a leaf that hasn't been closed
        dag_node_t* node = dag_root;
        while (node->child != NULL) {
            dag_node_t* next_node = node->child;
            while (next_node->status == 2) {
                next_node = next_node->sibling;
            }
            node = next_node;
        }
        // Send the name of this leaf
        sprintf(buf, "RO;%s", node->name);
    }
    else if (dag_root->vote_list->winner == NULL) {
        // All polls have been closed, but no votes were registered
        sprintf(buf, "SC;No votes.");
    }
    else {
        // All polls have been closed and votes were registered
        sprintf(buf, "SC;Winner:%s", dag_root->vote_list->winner);
    }
}

/************************************************
 * Handle the Count_Votes command (CV).         *
 * If the region exists, count and return its   *
 * votes.                                       *
 ************************************************/
void count_votes(char* buf, char* region_name, dag_node_t* dag_root) {
    dag_node_t* node = get_dag_node(region_name, dag_root);
    if (node == NULL) {
        // Region does not exist
        sprintf(buf, "NR;%s", region_name);
    }
    else {
        pthread_mutex_lock(node->mutex);
        char* vote_string = get_vote_string(node->vote_list);
        pthread_mutex_unlock(node->mutex);
        if (vote_string[0] == '\0') {
            strcpy(buf, "SC;No votes.");
        }
        else {
            sprintf(buf, "SC;%s", vote_string);
        }
        free(vote_string);
    }
}


/************************************************
 * Handle the Open_Polls command (OP).          *
 * If the region exists and hasn't been opened, *
 * open it and all its descendants.             *
 ************************************************/
void open_polls(char* buf, char* region_name, dag_node_t* dag_root) {
    dag_node_t* node = get_dag_node(region_name, dag_root);
    if (node == NULL) {
        // Region does not exist
        sprintf(buf, "NR;%s", region_name);
    }
    else if (node->status == 1) {
        // Polls are already open
        sprintf(buf, "PF;%s open.", region_name);
    }
    else if (node->status == 2) {
        // Polls are already closed (can't be reopened)
        sprintf(buf, "RR;%s", region_name);
    }
    else {
        open_subtree(node);
        strcpy(buf, "SC;");
    }
}

/************************************************
 * Handle the Add_Votes command (AV).           *
 * If the region exists, is open, and is a leaf,*
 * sum up the votes in the file and add them to *
 * the region's node in the DAG.
 ************************************************/
void add_votes(char* buf, char* region_name, dag_node_t* dag_root) {
    dag_node_t* node = get_dag_node(region_name, dag_root);
    if (node == NULL) {
        // Region does not exist
        sprintf(buf, "NR;%s", region_name);
    }
    else if (node->child != NULL) {
        // Region is not a leaf
        sprintf(buf, "NL;%s", region_name);
    }
    else if (node->status != 1) {
        // Region is not open
        sprintf(buf, "RC;%s", region_name);
    }
    else {
        vote_list_t* new_votes = read_votes_from_buf(buf);
        while (node != NULL) {
            pthread_mutex_lock(node->mutex);
            add_vote_list(node->vote_list, new_votes);
            pthread_mutex_unlock(node->mutex);
            node = node->parent;
        }
        free_vote_list(new_votes);
        strcpy(buf, "SC;");
    }
}

/************************************************
 * Handle the Remove_Votes command (RV).        *
 * If the region exists, is open, and is a leaf,*
 * sum up the votes in the file and attempt to  *
 * subtract them from the region's node in the  *
 * DAG. If this would push any candidate into   *
 * the negative, return the names of all such   *
 * candidates. Otherwise, do the subtraction.   *
 ************************************************/
void remove_votes(char* buf, char* region_name, dag_node_t* dag_root) {
    dag_node_t* node = get_dag_node(region_name, dag_root);
    if (node == NULL) {
        // Region does not exist
        sprintf(buf, "NR;%s", region_name);
    }
    else if (node->child != NULL) {
        // Region is not a leaf
        sprintf(buf, "NL;%s", region_name);
    }
    else if (node->status != 1) {
        // Region is not open
        sprintf(buf, "RC;%s", region_name);
    }
    else {
        vote_list_t* new_votes = read_votes_from_buf(buf);
        char* bad_names = validate_removal(node->vote_list, new_votes);
        if (bad_names != NULL) {
            // Removal is invalid
            sprintf(buf, "IS;%s", bad_names);
            free(bad_names);
        }
        else {
            while (node != NULL) {
                pthread_mutex_lock(node->mutex);
                subtract_vote_list(node->vote_list, new_votes);
                pthread_mutex_unlock(node->mutex);
                node = node->parent;
            }
            strcpy(buf, "SC;");
        }
        free_vote_list(new_votes);
    }
}

/************************************************
 * Handle the Open_Polls command (OP).          *
 * If the region exists and is open, close it   *
 * and all its descendants.                     *
 ************************************************/
void close_polls(char* buf, char* region_name, dag_node_t* dag_root) {
    dag_node_t* node = get_dag_node(region_name, dag_root);
    if (node == NULL) {
        // Region does not exist
        sprintf(buf, "NR;%s", region_name);
    }
    else if (node->status == 0) {
        // Region is unopened
        sprintf(buf, "PF;%s unopened.", region_name);
    }
    else if (node->status == 2) {
        // Region is already closed
        sprintf(buf, "PF;%s closed.", region_name);
    }
    else {
        close_subtree(node);
        strcpy(buf, "SC;");
    }
}

/************************************************
 * The thread function. Gets called once for    *
 * each client, with the client_info_t struct   *
 * defined above. Until the connection is       *
 * closed by the client, read requests, call    *
 * the appropriate handler function, and send   *
 * responses back to the client.                *
 ************************************************/
void* handle_client(void* arg) {
    client_info_t* info = (client_info_t*) arg;
    char* terminal_output;
    char buf[256];
    char region_name[16];
    while (recv(info->sock, buf, 256, 0) != 0) {
        terminal_output = get_client_terminal_output(buf);
        printf("Request received from client at %s:%d, %s\n", info->ip, info->port, terminal_output);
        free(terminal_output);
        // Get region name
        int i;
        region_name[15] = '\0';
        for (i = 14; buf[i+3] == ' ' && i >= 0; i--) {
            region_name[i] = '\0';
        }
        for ( ; i >= 0; i--) {
            region_name[i] = buf[i+3];
        }
        // Determine command
        if (buf[0] == 'R' && buf[1] == 'W') {
            return_winner(buf, info->dag_root);
        }
        else if (buf[0] == 'C' && buf[1] == 'V') {
            count_votes(buf, region_name, info->dag_root);
        }
        else if (buf[0] == 'O' && buf[1] == 'P') {
            open_polls(buf, region_name, info->dag_root);
        }
        else if (buf[0] == 'A' && buf[1] == 'V') {
            add_votes(buf, region_name, info->dag_root);
        }
        else if (buf[0] == 'R' && buf[1] == 'V') {
            remove_votes(buf, region_name, info->dag_root);
        }
        else if (buf[0] == 'C' && buf[1] == 'P') {
            close_polls(buf, region_name, info->dag_root);
        }
        else if (buf[0] == 'A' && buf[1] == 'R') {
            // This function is located in "dag.h"
            if (add_region(region_name, buf + 19, info->dag_root)) {
                // Region does not exist
                sprintf(buf, "NR;%s", region_name);
            }
            else {
                // New region successfully created
                strcpy(buf, "SC;");
            }
        }
        else {
            // Unknown command.
            strcpy(buf, "UC;");
        }
        // Send result to client
        terminal_output = get_server_terminal_output(buf);
        printf("Sending response to client at %s:%d, %s\n", info->ip, info->port, terminal_output);
        send(info->sock, buf, 256, 0);
        free(terminal_output);
    }
    
	// Close the connection.
	close(info->sock);
    printf("Closed connection with client at %s:%d\n", info->ip, info->port);
    return NULL;
}

/************************************************
 * The main function. Continually listens for   *
 * clients. When a connection is established,   *
 * it creates and immediately detaches a thread *
 * so that multiple clients can run in parallel.*
 * Must be terminated by a keyboard interrupt.  *
 ************************************************/
int main(int argc, char** argv) {
    // Check arguments
	if (argc != 3) {
		printf("Usage: ./server <DAG_FILE> <Server_Port>\n");
		exit(1);
	}
	char* dag_name = argv[1];
	unsigned long int server_port = strtoul(argv[2], NULL, 10);
	
	// Read in a DAG file.
	dag_node_t* dag_root = read_dag_from_file(dag_name);
	
	// Create a TCP socket.
	int sock = socket(AF_INET , SOCK_STREAM , 0);
	
	// Bind it to a local address.
	struct sockaddr_in servAddress;
	servAddress.sin_family = AF_INET;
	servAddress.sin_port = htons(server_port);
	servAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sock, (struct sockaddr *) &servAddress, sizeof(servAddress));
	
	// Listen on this socket.
	listen(sock, 0);
	printf("Server listening on port %d\n", server_port);
	
	pthread_t child_thread;
	
	while (1) {
		
		// Accept a connection.
		struct sockaddr_in clientAddress;
		socklen_t size = sizeof(struct sockaddr_in);
		int new_sock = accept(sock, (struct sockaddr *) &clientAddress, &size);
		int client_port = clientAddress.sin_port;
		char* client_ip = malloc(INET_ADDRSTRLEN);
		inet_ntop(AF_INET, &(clientAddress.sin_addr.s_addr), client_ip, INET_ADDRSTRLEN);
		printf("Connection initiated from client at %s:%d\n", client_ip, client_port);
		
		// Set up and execute thread
		client_info_t* info = (client_info_t*) malloc(sizeof(client_info_t));
		info->dag_root = dag_root;
		info->sock = new_sock;
		info->ip = client_ip;
		info->port = client_port;
		pthread_create(&child_thread, NULL, handle_client, (void*) info);
		pthread_detach(child_thread);
	}
	
	// Close the server socket.
	close(sock);
}
