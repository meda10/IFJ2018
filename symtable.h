#ifndef TREE_BT_H
#define TREE_BT_H

#define RET_OK 0
#define RET_ERR 1
#define INTERNAL_ERROR 99

typedef struct node {
    int data;
    struct node *L_ptr;
    struct node *R_ptr;
} *BTNode;

void B_tree_init(BTNode *root);
BTNode B_tree_search(BTNode root, int V);
int B_tree_insert(BTNode *root, int V);
void B_tree_walk(BTNode root);

#endif //TREE_BT_H