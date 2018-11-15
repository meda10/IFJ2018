#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "symtable.h"

Values init_val(const char *name) {
    Values data;
    data.name = name;
    data.value = NULL;
    data.type = -1;
    data.params_number = -1;
    data.params = NULL;
    data.defined = false;
    data.initialized = false;
    data.is_function = false;
    data.used = false;
    B_tree_init(&data.local_sym_table);
    return data;
}


void B_tree_init(BTNode *root) {
    *root = NULL;
}

BTNode B_tree_search(BTNode root, char *name){
    while(root != NULL){
        if(strcmp(root->data.name,name) == 0){ //todo cmp
            return root;
        }

        if(strcmp(root->data.name,name) > 0){ //todo cmp
            root = root->L_ptr;
        }else{
            root = root->R_ptr;
        }
    }
    return NULL;
}

int B_tree_insert(BTNode *root, struct Values data){
    BTNode tmp_root = *root;
    BTNode tmp = NULL;
    bool found = false;

    while((tmp_root != NULL) && (!found)){
        tmp = tmp_root;
        if(strcmp(tmp_root->data.name,data.name) == 0){ //todo cmp
            //printf("NODE %d, ALREADY EXISTS\n", tmp_root->data);
            //tmp_root->data = V; todo insert
            found = true;
        }
        if(strcmp(tmp_root->data.name,data.name) > 0){ //todo cmp
            tmp_root = tmp_root->L_ptr;
        }else{
            tmp_root = tmp_root->R_ptr;
        }
    }

    if(!found){
        BTNode new_node = (BTNode) malloc(sizeof(struct node));
        if(new_node == NULL){
            return INTERNAL_ERROR;
        }
        new_node->data = data;
        new_node->L_ptr = NULL;
        new_node->R_ptr = NULL;

        if(tmp == NULL){
            *root = new_node;
        }else{
            if(strcmp(tmp->data.name,data.name) > 0){ //todo cmp
                tmp->L_ptr = new_node;
            }else{
                tmp->R_ptr = new_node;
            }
        }
        //printf("Created Node %s, Left: %s Right: %s\n", new_node->data.name,new_node->L_ptr, new_node->R_ptr);
    }
    return RET_OK;
}

void B_tree_walk(BTNode root){
    if(root != NULL){
        B_tree_walk(root->L_ptr);
        printf("%s ", root->data.name);
        B_tree_walk(root->R_ptr);
    }
}

void B_tree_free(BTNode root){
    if(root != NULL){
        B_tree_free(root->L_ptr);
        B_tree_free(root->R_ptr);
        free(root);
    }
}

int create_node(BTNode *table, char *name, char *value, int type, int params_number, char** params, bool defined, bool initialized, bool is_function, bool used, BTNode *local_sym_table) {
    BTNode node = B_tree_search(*table, name);
    if(node == NULL) {
        Values val = init_val(name);
        val.value = value;
        val.type = type;
        val.params_number = params_number;
        val.params = params;
        val.defined = defined;
        val.initialized = initialized;
        val.is_function = is_function;
        val.used = used;
        val.local_sym_table = local_sym_table;

        B_tree_insert(table, val);
        return RET_OK;
    }else {
        return RET_ERR;
    }

}
