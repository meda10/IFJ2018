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

    //vlozeni uzlu
    create_node(&root_ptr,"TEXT", "25" , 3,0, NULL,false, false,false,false, NULL);
    create_node(&root_ptr,"M", "2www5" , 3,0, NULL,false, false,false,false, NULL);
    create_node(&root_ptr,"O", "2kkk5" , 3,0, NULL,false, false,false,false, NULL);
    create_node(&root_ptr,"TEXT123", "25" , 3,0, NULL,false, false,false,false, NULL);
    create_node(&root_ptr,"H", "25" , 3,0, NULL,false, false,false,false, NULL);
    create_node(&root_ptr,"G", "2dawd" , 3,0, NULL,false, false,false,false, NULL);
    create_node(&root_ptr,"FF", "w5" , 3,0, NULL,false, false,false,false, NULL);
    create_node(&root_ptr,"AWD", "2w" , 3,0, NULL,false, false,false,false, NULL);

    //pokus o vloyeni stejneho prvku
    int ret = create_node(&root_ptr,"AWD", "684" , 3,0, NULL,false, true,true,false, NULL);
    if(ret == RET_ERR){
        fprintf(stderr,"ERROR: Already in symtable\n");
    }

    //Hleda v root_ptr H
    node = B_tree_search(root_ptr,"H");
    if(node != NULL){
        printf("FOUND: %s\n",node->data.name);
    } else{
        printf("NOT FOUND: H\n");
    }

    //Hleda v root_ptr X
    node = B_tree_search(root_ptr,"X");
    if(node != NULL){
        printf("FOUND: %s\n",node->data.name);
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