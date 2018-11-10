#include <stdio.h>
#include <stdlib.h>
#include "symtable.h"


void B_tree_init(BTNode *root) {
    *root = NULL;
}

BTNode B_tree_search(BTNode root, int V){
    while (root != NULL) {
        //printf("Looking for %d, looking at %d\n", V, root->data);

        if (root->data == V){ //todo cmp
            return root;
        }

        if (root->data < V){ //todo cmp
            root = root->R_ptr;
        } else{
            root = root->L_ptr;
        }
    }
    return NULL;
}

int B_tree_insert(BTNode *root, int V){
    BTNode tmp_root = *root;
    BTNode tmp = NULL;

    while(tmp_root != NULL){
        tmp = tmp_root;
        if (tmp_root->data == V){ //todo cmp
            //printf("NODE %d, ALREADY EXISTS\n", tmp_root->data);
            return RET_ERR;
        }
        if (tmp_root->data > V){ //todo cmp
            tmp_root = tmp_root->L_ptr;
        } else{
            tmp_root = tmp_root->R_ptr;
        }
    }

    BTNode new_node = (BTNode) malloc(sizeof(struct node));
    if(new_node == NULL){
        //todo malloc error
        return INTERNAL_ERROR;
    }
    new_node->data = V;
    new_node->L_ptr = NULL;
    new_node->R_ptr = NULL;

    if (tmp == NULL) {
        *root = new_node;

    } else {
        if (tmp->data > V) { //todo cmp
            tmp->L_ptr = new_node;
        } else {
            tmp->R_ptr = new_node;
        }
    }
    //printf("Created Node %d, Left: %s Right: %s\n", new_node->data,new_node->L_ptr, new_node->R_ptr);
    return RET_OK;
}

void B_tree_walk(BTNode root) {
    if (root != NULL){
        B_tree_walk(root->L_ptr);
        printf("%d ", root->data);
        B_tree_walk(root->R_ptr);
    }
}