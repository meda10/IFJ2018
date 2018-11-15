#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parser.h"
#include "scaner.h"
#include "symtable.h"

int main() {

    FILE *source_file = open_file("/home/petr/CLionProjects/IFJ2018/source");

    int result = parse();
    printf("%d\n", result);

    /**
     * Jenom Scaner - cteni jednotlivych tokenu
     */
/*
    token_t *token = make_new_token();
    for(int i = 0; i < 500; i++){
        if (get_next_token(token) == RET_ERR)
            return RET_ERR;
        else
            print_token(token);

        if(token->type == 39){
            free_token(token);
            return 0;
        }
    }
*/
    /**
     * Tabulka symbolu - priklad pouziti
     */
/*
    BTNode root_ptr;
    BTNode node;
    int ret = 0;
    BTNode param = NULL;
    B_tree_init(&root_ptr);

    //create_node(&btGetVariable(symtable, functionName)->data.treeOfFunction, name, type, true, false, false, &params, NULL, 0); // Add new arguments
    //create_node(&root_ptr, "NAME",true, false, false, &params, NULL, 0); // Add new arguments

    create_node(&root_ptr,"M",false,true,false,0,&param);
    create_node(&root_ptr,"H",false,true,false,0,&param);
    create_node(&root_ptr,"I",false,true,false,0,&param);
    create_node(&root_ptr,"W",false,true,false,0,&param);
    create_node(&root_ptr,"C",false,true,false,5,&param);
    create_node(&root_ptr,"M",true,true,false,88,&param); // chyba

    //Hleda v root_ptr H
    node = B_tree_search(root_ptr,"C");
    if(node != NULL){
        printf("FOUND: %s P_NUM: %d\n",node->data.name,node->data.params_number);
    } else{
        printf("NOT FOUND: H\n");
    }

    //Hleda v root_ptr X
    node = B_tree_search(root_ptr,"X");
    if(node != NULL){
        printf("FOUND: %s P_NUM: %d\n",node->data.name,node->data.params_number);
    } else{
        printf("NOT FOUND: X\n");
    }

    //Projde cely strom a vytiskne ho na STDOUT
    printf("STROM: ");
    B_tree_walk(root_ptr);
    printf("\n");

    //Projde cely strom a uvolni pamet
    B_tree_free(root_ptr);
*/
    if(fclose(source_file) == EOF){
        fprintf(stderr, "Internl Error: %s\n", strerror(errno));
        return INTERNAL_ERROR;
    }

}