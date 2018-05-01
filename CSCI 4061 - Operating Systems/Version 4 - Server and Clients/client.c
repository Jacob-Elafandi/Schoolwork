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

#include "votelist.h"
#include "output.h"

/************************************************
 * The client main function. Connects to a      *
 * server, sends it commands, reads the         *
 * responses, and then closes the connection.   *
 ************************************************/
int main(int argc, char** argv) {

	if (argc != 4) {
	    printf("Usage: ./client <REQ_FILE> <Server_IP> <Server_Port>\n");
	    exit(-1);
	}

	// Create a TCP socket.
	int sock = socket(AF_INET , SOCK_STREAM , 0);
	
	// Specify an address to connect to
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(atoi(argv[3]));
	address.sin_addr.s_addr = inet_addr(argv[2]);
	
	// Connect it.
	if (connect(sock, (struct sockaddr *) &address, sizeof(address)) == 0) {
	    printf("Initiated connection with server at %s:%s\n", argv[2], argv[3]);
	
		// Buffer for data.
		char input_buf[1024];
		char server_buf[256];
		char* terminal_output;

		// Open the file.
		FILE* input = fopen(argv[1], "r");
		if (input == NULL) {
		    printf("Error: Could not open file \"%s\"\n", argv[1]);
		    close(sock);
	        printf("Closed connection with server at %s:%s\n", argv[2], argv[3]);
		    exit(-2);
		}
		
		// Change to the directory which contains the input file.
		*strrchr(argv[1], '/') = '\0';
		while (chdir(argv[1]));
		
		while (1) {
		    // Read the next line into the input buffer
		    for (int i = 0; ; i++) {
		        char c = fgetc(input);
		        if (c == EOF || c == '\n') {
		            input_buf[i] = '\0';
		            break;
		        }
		        input_buf[i] = c;
		    }
		    if (input_buf[0] == '\0') {
		        // All lines have been read.
		        break;
		    }
		    // Convert line into request to server
		    char* request = strtok(input_buf, " ");
		    if (strcmp(request, "Return_Winner") == 0) {
		        strcpy(server_buf, "RW;               ;");
		    }
		    else if (strcmp(request, "Add_Votes") == 0 || strcmp(request, "Remove_Votes") == 0) {
		        char* region = strtok(NULL, " ");
		        if (strcmp(request, "Add_Votes") == 0) {
		            sprintf(server_buf, "AV;%s", region);
		        }
		        else {
		            sprintf(server_buf, "RV;%s", region);
		        }
		        // Pad region with spaces
	            for (int i = strlen(server_buf); i <= 17; i++) {
	                server_buf[i] = ' ';
	            }
	            server_buf[18] = ';';
	            server_buf[19] = '\0';
	            char* vote_file_name = strtok(NULL, " ");
	            vote_list_t* vote_list = read_votes_from_file(vote_file_name);
	            if (vote_list == NULL) {
	                printf("Vote file %s does not exist\n", vote_file_name);
	                continue;
	            }
	            char* vote_string = get_vote_string(vote_list);
	            strcat(server_buf, vote_string);
	            free_vote_list(vote_list);
	            free(vote_string);
		    }
		    else {
		        char* region = strtok(NULL, " ");
		        if (strcmp(request, "Count_Votes") == 0) {
		            sprintf(server_buf, "CV;%s", region);
		        }
		        else if (strcmp(request, "Open_Polls") == 0) {
		            sprintf(server_buf, "OP;%s", region);
		        }
		        else if (strcmp(request, "Close_Polls") == 0) {
		            sprintf(server_buf, "CP;%s", region);
		        }
		        else {
		            printf("Unknown request: %s\n", region);
		            continue;
		        }
		        // Pad region with spaces
	            for (int i = strlen(server_buf); i <= 17; i++) {
	                server_buf[i] = ' ';
	            }
	            server_buf[18] = ';';
	            server_buf[19] = '\0';
		    }
		    // Send request to server
		    terminal_output = get_client_terminal_output(server_buf);
		    printf("Sending request to server: %s\n", terminal_output);
		    send(sock, server_buf, 256, 0);
		    free(terminal_output);
		    recv(sock, server_buf, 256, 0);
		    terminal_output = get_server_terminal_output(server_buf);
		    printf("Received response from server: %s\n", terminal_output);
		    free(terminal_output);
		}
		
		// Close the file.
		fclose(input);
		close(sock);
	    printf("Closed connection with server at %s:%s\n", argv[2], argv[3]);
		
	} else {
	
		perror("Connection failed!");
	}
}
