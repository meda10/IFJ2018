/*
 * FIT VUT, IFJ 2018
 * Authors:
 *    Petr Medek, xmedek07
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include "symtable.h"
#include "error.h"
#include "stringss.h"

/**
 * Inicializece struktury dat která se vkládá do tabulky symbolů
 * @param name jméno uzlu
 * @param len délka name
 * @return inicializovaná struktura
 */
TValues init_val(char *name, int len) {
    TValues data;
    if(name != NULL){
        if ((data.name = (char*) malloc((size_t)len + 1)) == NULL){
            errors_exit(INTERNAL_ERROR,"Internl Error\n");
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
    return data;
}

/**
 * Vytvoří novou tabulku symbolů
 * @return
 */
BTNode* make_new_table(){
    BTNode *b = (BTNode*) malloc(sizeof(struct node));
    if(b == NULL){
        errors_exit(INTERNAL_ERROR,"Internl Error\n");
    }
    memset(b,0,sizeof(BTNode));
    return b;
}

/**
 * inicializace tabulky symbolů
 * @param root
 */
void B_tree_init(BTNode *root) {
    *root = NULL;
}

/**
 * Hledání v tabulce symbolů podle názvu
 * @param root - tabulka symbolů
 * @param name - název
 * @return - najitý uzel / NULL
 */
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

/**
 * Vrací odkaz na kokální tabulku symbolů
 * @param root -tabulka symbolů
 * @param name -název funkce
 * @return - odkaz na lokální tabulku symbolů
 */
BTNode B_tree_search_local_table(BTNode root, char *name){
    BTNode old_root = root;
    while(root != NULL){
        if(name == NULL){
            return NULL;
        }
        if(strcmp(root->data.name,name) == 0){
            if(old_root != root->data.local_sym_table){
                return root->data.local_sym_table;
            } else{
                return NULL;
            }
        }
        if(strcmp(root->data.name,name) > 0){
            root = root->L_ptr;
        }else{
            root = root->R_ptr;
        }
    }
    return NULL;
}

/**
 * Vložení uzlu do tabulky symbolů
 * @param root - tabulka symbolů
 * @param data - data
 * @return
 */
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
            errors_exit(INTERNAL_ERROR,"Internl Error\n");
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
        //printf("Created Node %s, Left: %s Right: %s\n", new_node->data.name->str,new_node->L_ptr, new_node->R_ptr);
    }
    return RETURN_OK;
}

/**
 * průchod tabulky smybolů
 * @param root - uzel
 */
void B_tree_walk(BTNode root){
    if(root != NULL){
        B_tree_walk(root->L_ptr);
        B_tree_walk(root->R_ptr);
        printf("%s ", root->data.name);
        //printf("%s ->%p\n", root->data.name,root->data.name);
    }
}

/**
 * Uvolnení paměti, i lokálních tabulek symbolů
 * @param root - tabulka symbolů
 */
void B_tree_free(BTNode root){
    if(root != NULL){
        B_tree_free(root->L_ptr);
        B_tree_free(root->R_ptr);
        if(root->data.is_function && root->data.params != NULL){
            for (int i =1; i <= root->data.params_number; i++) {
                free(root->data.params[i]);
            }
            free(root->data.params);
            B_tree_free(root->data.local_sym_table);
        }
        free(root->data.name);
        //delete_string(root->data.name);
        //delete_string(&root->data.name);
        free(root);
    }
}

/**
 * Vytvoří uzel v tabulce symboů
 * @param table - tabulka
 * @param name - název parametru / funkce
 * @param type - typ
 * @param params_number - počet parametrů
 * @param params - parametry
 * @param defined - jestli je funkce definovaná
 * @param initialized - jestli je inicializovaná
 * @param is_function - jestli se jedná o funkci nebo proměnou
 * @param used - jestli se nejedná o mrtvý kód
 * @param local_sym_table - odkaz na lokální tabulku symbolů
 * @return
 */
int create_node(BTNode *table, char *name, int type, int params_number, char** params, bool defined, bool initialized, bool is_function, bool used, BTNode *local_sym_table) {
    BTNode node = B_tree_search(*table, name);

    BTNode no = B_tree_search(*table, "$$<&FUNKCE_NEMA_PARAMETRY&>$$");
    if(no != NULL){
        free(no->data.name);
        if ((no->data.name = (char*) malloc(strlen(name) + 1)) == NULL){
            errors_exit(INTERNAL_ERROR,"Internl Error\n");
        }
        strcpy(no->data.name, name);
        no->data.type = type;
        no->data.params_number = params_number;
        no->data.params = params;
        no->data.defined = defined;
        no->data.initialized = initialized;
        no->data.is_function = is_function;
        no->data.used = used;
        no->data.local_sym_table = *local_sym_table;

        return RETURN_OK;
    }


    if(node == NULL && name != NULL) {
        TValues val = init_val(name, (int)strlen(name) + 1);
        //val.value = value;
        val.type = type;
        val.params_number = params_number;
        val.params = params;
        val.defined = defined;
        val.initialized = initialized;
        val.is_function = is_function;
        val.used = used;
        val.local_sym_table = *local_sym_table;
        //printf("LOCAL %p\n",*local_sym_table);

        B_tree_insert(table, val);
        //printf("ADD: %s -> %p %d\n",val.name,val.name,val.params_number);

        return RETURN_OK;
    }else {
        return RET_ERR;
    }

}
