#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <pthread.h>

/************************************
 * Structure: dag_node				*
 * ---------------------------------*
 * This structure organizes the dag	*
 * 	to be used elsewhere in the 	*
 *	code as a linked list.							*
 * ---------------------------------*
 * name: string pointer to dag's	*
 * 	name.							*
 *									*
 * folder_path: string pointer to 	*
 * 	the folder path.				*
 *									*
 * mutex: pointer containing the	*
 * 	mutex for the node.				*
 *									*
 * num_children: integer with the #	*
 * 	of child nodes.					*
 *									*
 * children: pointer to array of 	*
 *	dag_node containing children.	*
 *									*
 * parent: pointer to dag_node of	*
 *	the parent node.				*
 ************************************/
typedef struct dag_node {
    char* name;
    char* folder_path;
    pthread_mutex_t* mutex;
    int num_children;
    struct dag_node* children;
    struct dag_node* parent;
} dag_node_t;

/************************************
 * Function: get dag node			*
 * ---------------------------------*
 * The function recursively 		*
 * 	searches root and its children 	*
 * 	for the node named by this_name.*
 * 	The found node is returned 		*
 * 	as result.						*
 * ---------------------------------*
 * inputs: this_name, root			*
 * outputs: result					*
 * ---------------------------------*
 * this_name: a pointer to a string	*
 * 	containing the name of the 		*
 * 	desired dag_node				*
 *									*
 * root: the current dag_node 		*
 * 	to search within for this_name	*
 *									*
 * result: the dag_node that 		*
 * 	was searched for within the 	*
 * 	dag using this_name				*
 ************************************/
dag_node_t* get_dag_node(char* this_name, dag_node_t* root) {
    if (root == NULL || this_name == NULL) {
        return NULL;
    }
	if (strcmp(this_name, root->name) == 0) {
        return root;
    }
	for (int i = 0; i < root->num_children; i++) {
        dag_node_t* result = get_dag_node(this_name, &(root->children[i]));
        if (result != NULL) {
            return result;
        }
    }
    return NULL;
}

/************************************
 * Funtcion: free memory allocated	*
 * ---------------------------------*
 * This function recursively frees 	*
 * 	the children of the node passed	*
 *	into the function and itself.	*
 * ---------------------------------*
 * inputs: root						*
 * ---------------------------------*
 * root: pointer to the current 	*
 *	node.							*
*************************************/
void free_dag_helper(dag_node_t* root) {
    if (root == NULL) {
        return;
    }
    free(root->name);
    free(root->folder_path);
    free(root->mutex);
    for (int i = 0; i < root->num_children; i++) {
        free_dag_helper(&(root->children[i]));
    }
    if (root->children != NULL) {
        free(root->children);
    }
}

/************************************
 * Funtcion: free dag				*
 * ---------------------------------*
 * This function frees up the dag	*
 * 	tree and all the child nodes.	*
 * ---------------------------------*
 * inputs: root						*
 * ---------------------------------*
 * root: pointer to the root node of*
 *	the dag.						*
*************************************/
void free_dag(dag_node_t* root) {
    if (root == NULL) {
        return;
    }
    free_dag_helper(root);
    free(root);
}
