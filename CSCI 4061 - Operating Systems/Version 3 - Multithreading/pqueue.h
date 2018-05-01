#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <pthread.h>

// Doubly linked list node to keep track of names in a priority queue
typedef struct pqueue_node {
    char* node_name;    // e.g. Region_2
    char* file_name;    // e.g. Region_2_p_504
    int priority;       // e.g. 504
    struct pqueue_node* left;
    struct pqueue_node* right;
} pqueue_node_t;

// Priority queue
typedef struct {
    pqueue_node_t* leftmost;
    pqueue_node_t* rightmost;
    pthread_mutex_t* mutex;
} pqueue_t;

/************************************************************
 * Function: insert_pqueue_node								*
 * ---------------------------------------------------------*
 * This function inserts name_in into pqueue with priority 	*
 *	priority_in.											*
 * ---------------------------------------------------------*
 * inputs: name_in, priority_in, last_uscore, queue			*
 * ---------------------------------------------------------*
 * name_in: string pointer of the name to enqueue			*
 * priority_in: int of priority; lower int = higher priority*
 * last_uscore: int index of last underscore in name_in		*
 * queue: queue to enqueue to								*
 ************************************************************/
void insert_pqueue_node(char* name_in, int priority_in, int last_uscore, pqueue_t* queue) {
    pqueue_node_t* left = NULL;
    pqueue_node_t* right = NULL;  // Will get set later
    pqueue_node_t* new_node = malloc(sizeof(pqueue_node_t));
    right = queue->leftmost;
    while (right != NULL && right->priority > priority_in) {
        left = right;
        right = right->right;
    }
    if (left == NULL) {
        queue->leftmost = new_node;
    }
    else {
        left->right = new_node;
    }
    if (right == NULL) {
        queue->rightmost = new_node;
    }
    else {
        right->left = new_node;
    }
    new_node->file_name = malloc(strlen(name_in) + 1);
    strcpy(new_node->file_name, name_in);
    new_node->node_name = malloc(strlen(name_in) + 1);
    strcpy(new_node->node_name, name_in);
    if (last_uscore != -1) {
        new_node->node_name[last_uscore-2] = '\0';
    }
    new_node->priority = priority_in;
    new_node->left = left;
    new_node->right = right;
}

/************************************************************
 * Function: pop_pqueue_node								*
 * ---------------------------------------------------------*
 * This function exrtacts a node from priority queue		*
 * ---------------------------------------------------------*
 * inputs: queue											*
 * ---------------------------------------------------------*
 * queue: pointer to queue to be popped						*
 ************************************************************/
pqueue_node_t* pop_pqueue_node(pqueue_t* queue) {
    if (queue->rightmost == NULL) {
        return NULL;    // Queue is empty
    }
    pqueue_node_t* old_rightmost = queue->rightmost;
    if (old_rightmost->left != NULL) {
        old_rightmost->left->right = NULL;
    }
    else {
        queue->leftmost = NULL;
    }
    queue->rightmost = old_rightmost->left;
    old_rightmost->left = NULL;
    return old_rightmost;
}

/************************************************************
 * Function: free_pqueue_node								*
 * ---------------------------------------------------------*
 * This function frees the memory allocated to a node		*
 * ---------------------------------------------------------*
 * inputs: node												*
 * ---------------------------------------------------------*
 * node: pointer to node to be freed						*
 ************************************************************/
void free_pqueue_node(pqueue_node_t* node) {
    free(node->node_name);
    free(node->file_name);
    free(node);
}

/************************************************************
 * Function: free_pqueue									*
 * ---------------------------------------------------------*
 * This function frees the memory allocated to the queue,	*
 *	making sure to free the nodes first.					*
 * ---------------------------------------------------------*
 * inputs: queue											*
 * ---------------------------------------------------------*
 * queue: pointer to queue to be freed						*
 ************************************************************/
void free_pqueue(pqueue_t* queue) {
    while (queue->rightmost != NULL) {
        pqueue_node_t* old_rightmost = queue->rightmost;
        queue->rightmost = old_rightmost->left;
        free_pqueue_node(old_rightmost);
    }
    free(queue->mutex);
    free(queue);
}
