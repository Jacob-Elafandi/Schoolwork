#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <pthread.h>

#include "votelist.h"

#define MAX_REGION_NAME_LENGTH 15

/************************************************
 * The node of a Directed Acyclic Graph.        *
 * Contains a mutex, status int, and vote list. *
 * Has links to its parent, first child, and its*
 * parent's next child (i.e. its elder sibling).*
 ************************************************/
typedef struct dag_node {
    char* name;
    pthread_mutex_t* mutex;
    int status;  // 0 = unopened, 1 = open, 2 = closed
    vote_list_t* vote_list;
    struct dag_node* child;
    struct dag_node* sibling;
    struct dag_node* parent;
} dag_node_t;


/************************************************
 * Looks through the tree for a node with the   *
 * specified name. Returns NULL if not found.   *
 ************************************************/
dag_node_t* get_dag_node(char* this_name, dag_node_t* root) {
    if (root == NULL || this_name == NULL) {
        return NULL;
    }
	if (strcmp(this_name, root->name) == 0) {
        return root;
    }
	for (dag_node_t* next = root->child; next != NULL; next = next->sibling) {
        dag_node_t* result = get_dag_node(this_name, next);
        if (result != NULL) {
            return result;
        }
    }
    return NULL;
}

/************************************************
 * Frees all nodes in the tree and all their    *
 * component elements.                          *
 ************************************************/
void free_dag(dag_node_t* root) {
    if (root == NULL) {
        return;
    }
    free(root->name);
    free(root->mutex);
    free_vote_list(root->vote_list);
    dag_node_t* next = root->child;
    while (next != NULL) {
        dag_node_t* after_next = next->sibling;
        free_dag(next);
        next = after_next;
    }
    free(root);
}

/************************************************
 * If this node has never been opened, open it  *
 * and attempt to open all its descendants.     *
 ************************************************/
void open_subtree(dag_node_t* node) {
    if (node->status == 0) {
        pthread_mutex_lock(node->mutex);
        node->status = 1;
        pthread_mutex_unlock(node->mutex);
        dag_node_t* child_node = node->child;
        while (child_node != NULL) {
            open_subtree(child_node);
            child_node = child_node->sibling;
        }
    }
}

/************************************************
 * If this node is open, close it and attempt   *
 * to close all its descendants.                *
 ************************************************/
void close_subtree(dag_node_t* node) {
    if (node->status == 1) {
        pthread_mutex_lock(node->mutex);
        node->status = 2;
        pthread_mutex_unlock(node->mutex);
        dag_node_t* child_node = node->child;
        while (child_node != NULL) {
            close_subtree(child_node);
            child_node = child_node->sibling;
        }
    }
}

/************************************************
 * Add a region to the DAG by making a new node *
 * as the new youngest sibling of its parent,   *
 * with a link to the previous youngest sibling.*
 ************************************************/
int add_region(char* parent_name, char* child_name, dag_node_t* root) {
    dag_node_t* parent_node = get_dag_node(parent_name, root);
    if (parent_node == NULL) {
        return -1;
    }
    dag_node_t* child_node = malloc(sizeof(dag_node_t));
    child_node->name = malloc(strlen(child_name) + 1);
    strcpy(child_node->name, child_name);
    child_node->mutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(child_node->mutex, NULL);
    child_node->vote_list = malloc(sizeof(vote_list_t));
    child_node->vote_list->head = NULL;
    child_node->vote_list->winner = NULL;
    child_node->vote_list->winner_count = 0;
    child_node->status = 0;
    child_node->child = NULL;
    child_node->sibling = parent_node->child;
    child_node->parent = parent_node;
    pthread_mutex_lock(parent_node->mutex);
    parent_node->child = child_node;
    pthread_mutex_unlock(parent_node->mutex);
    return 0;
}

/************************************************
 * Take in the name of a file and construct a   *
 * DAG from its contents. Return the root.      *
 ************************************************/
dag_node_t* read_dag_from_file(char* dag_name) {
    FILE* dag_txt = fopen(dag_name, "r");
    if (dag_txt == NULL) {
        printf("Error: Couldn't open file: %s\n", dag_name);
        exit(-1);
    }
    char region_name[MAX_REGION_NAME_LENGTH + 1];
    char c = 'A';  // Placeholder, to avoid EOF condition
    dag_node_t* root = NULL;
    while (c != EOF) {
        // Scan in region name
        for (int i = 0; i <= MAX_REGION_NAME_LENGTH; i++) {
            c = fgetc(dag_txt);
            if (c == ':' || c == '\n' || c == EOF) {
                region_name[i] = '\0';
                break;
            }
            region_name[i] = c;                
        }
        // Get node from region name
        dag_node_t* node = get_dag_node(region_name, root);
        if (node == NULL) {
            if (root != NULL) {
                printf("Error: attempting to define child before parent in DAG\n");
                exit(-2);
            }
            // DAG is empty. Create root node and populate
            root = malloc(sizeof(dag_node_t));
            root->name = malloc(strlen(region_name) + 1);
            strcpy(root->name, region_name);
            root->mutex = malloc(sizeof(pthread_mutex_t));
            pthread_mutex_init(root->mutex, NULL);
            root->vote_list = malloc(sizeof(vote_list_t));
            root->vote_list->head = NULL;
            root->vote_list->winner = NULL;
            root->vote_list->winner_count = 0;
            root->status = 0;
            root->child = NULL;
            root->sibling = NULL;
            root->parent = NULL;
            node = root;
        }
        
        // Add children to DAG until end of line is reached
        while (c != '\n' && c != EOF) {
            // Scan in child region name
            for (int i = 0; i <= MAX_REGION_NAME_LENGTH; i++) {
                c = fgetc(dag_txt);
                if (c == ':' || c == '\n' || c == EOF) {
                    region_name[i] = '\0';
                    break;
                }
                region_name[i] = c;                
            }
            // Create new node with new region name, and insert
            dag_node_t* new_child = malloc(sizeof(dag_node_t));
            new_child->name = malloc(strlen(region_name) + 1);
            strcpy(new_child->name, region_name);
            new_child->mutex = malloc(sizeof(pthread_mutex_t));
            pthread_mutex_init(new_child->mutex, NULL);
            new_child->vote_list = malloc(sizeof(vote_list_t));
            new_child->vote_list->head = NULL;
            new_child->vote_list->winner = NULL;
            new_child->vote_list->winner_count = 0;
            new_child->status = 0;
            new_child->child = NULL;
            new_child->sibling = node->child;
            new_child->parent = node;
            node->child = new_child;
        }
    }
    return root;
}
