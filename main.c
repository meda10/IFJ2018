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
    B_tree_init(&root_ptr);

    //ulozi tokeny do tabulky symbolu
    token_t *token = make_new_token();
    for(int i = 0; i < 50; i++){
        if (get_next_token(token) == RET_ERR)
            return RET_ERR;
        else{
            create_node(&root_ptr,&token->string, 3,0, NULL,false, false,false,false, NULL);
        }
    }
    free_token(token);

    //Hleda v root_ptr 55
    node = B_tree_search(root_ptr,"55");
    if(node != NULL){
        printf("FOUND: %s\n",node->data.name->str);
    } else{
        printf("NOT FOUND: 55\n");
    }

    //Hleda v root_ptr X
    node = B_tree_search(root_ptr,"X");
    if(node != NULL){
        printf("FOUND: %s\n",node->data.name->str);
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