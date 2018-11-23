#ifndef TREE_BT_H
#define TREE_BT_H

#include <stdbool.h>
#include "string.h"


#define RET_OK 0
#define RET_ERR 1
#define INTERNAL_ERROR 99

typedef union val{
    int int_val;
    double double_val;
    char *string_val;
}U_value;

typedef struct Values {
    char *name;
    //char *value;
    int type;
    int params_number;
    char **params;      //kde bude ulozen typ parametru?
    bool defined;
    bool initialized;
    bool is_function; // pozname jestli je to funkce nebo promena

    bool used;    // kontrola jestli to neni mrtvy kod

    struct node *local_sym_table;
}TValues;

typedef struct node {
    //int data;
    struct Values data;
    struct node *L_ptr;
    struct node *R_ptr;
} *BTNode;


BTNode* make_new_table();
void B_tree_init(BTNode *root);
BTNode B_tree_search(BTNode root, char* name);
int B_tree_insert(BTNode *root, struct Values data);
void B_tree_walk(BTNode root);
void B_tree_free(BTNode root);
int create_node(BTNode *table, string *name, int type, int params_number, char** params, bool defined, bool initialized, bool is_function, bool used, BTNode *local_sym_table);

#endif //TREE_BT_H