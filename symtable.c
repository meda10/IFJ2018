#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "symtable.h"
#include "string.h"

TValues init_val(char *name, int len) {
    TValues data;
    if(name != NULL){
        if ((data.name = (char*) malloc((size_t)len + 1)) == NULL){
            fprintf(stderr, "Internl Error: %s\n", strerror(errno));
            //todo set_error_string();
        }
        strcpy(data.name, name);
        //printf("CPY: %s -> %s FROM %p TO %p\n",name,data.name,name,data.name);
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
}


void B_tree_init(BTNode *root) {
    *root = NULL;
}

BTNode B_tree_search(BTNode root, char *name){
    while(root != NULL){
        if(name == NULL){
            return NULL;
        }
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
        //printf("LF -> %s IN -> %s\n",tmp_root->data.name,data.name);
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

        //printf("CREATE NEW NODE: %s  %p\n",new_node->data.name, new_node->data.name);
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
        B_tree_walk(root->R_ptr);
        printf("%s ", root->data.name);
        //printf("%s ->%p\n", root->data.name,root->data.name);
    }
}

void B_tree_free(BTNode root){
    if(root != NULL){
        B_tree_free(root->L_ptr);
        B_tree_free(root->R_ptr);
        free(root->data.name);
        //delete_string(root->data.name);
        //delete_string(&root->data.name);
        free(root);
    }
}

int create_node(BTNode *table, string *name, int type, int params_number, string** params, bool defined, bool initialized, bool is_function, bool used, BTNode *local_sym_table) {
    BTNode node = B_tree_search(*table, name->str);

    if(node == NULL && name->str != NULL) {
        TValues val = init_val(name->str, name->length);
        //val.value = value;
        val.type = type;
        val.params_number = params_number;
        val.params = params;
        val.defined = defined;
        val.initialized = initialized;
        val.is_function = is_function;
        val.used = used;
        val.local_sym_table = local_sym_table;

        B_tree_insert(table, val);
        //printf("ADD: %s -> %p %d\n",val.name,val.name,val.params_number);

        return RET_OK;
    }else {
        return RET_ERR;
    }

}
