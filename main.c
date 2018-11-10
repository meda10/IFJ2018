#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "parser.h"
#include "scaner.h"
#include "symtable.h"

int main() {

    FILE *source_file = open_file("/home/petr/CLionProjects/IFJ2018/source");

    //int result = parse();
    //printf("%d\n", result);

    /**
     * Jenom Scaner - cteni jednotlivych tokenu
     */
/*
    for(int i = 0; i < 500; i++){
        token_t *token = make_new_token();
        if (get_next_token(token) == RET_ERR)
            return RET_ERR;
        else
            print_token(token);

        if(token->type == 39){
            free_token(token);
            return 0;
        } else{
            free_token(token);
        }

    }
*/
    /**
     * Tabulka symbolu - priklad pouziti
     */
/*
    BTNode root_ptr;
    BTNode node;
    int a;
    int ret = 0;
    B_tree_init(&root_ptr);

    // Prida cisla 4,2,6,1,3,5,7
    int numbers[] = { 4, 2, 6, 1, 3, 5, 7 };

    for(int i = 0; i < 7; i++){
        ret = B_tree_insert(&root_ptr,numbers[i]);
        if(ret != 0){
            exit(ret);
        }
    }

    //Hleda v root_ptr cislo 85
    a = 85;
    node = B_tree_search(root_ptr,a);
    if(node != NULL){
        printf("FOUND: %d\n",node->data);
    } else{
        printf("NOT FOUND: %d\n",a);
    }

    //Hleda v root_ptr cislo 5
    a = 5;
    node = B_tree_search(root_ptr,a);
    if(node != NULL){
        printf("FOUND: %d\n",node->data);
    } else{
        printf("NOT FOUND: %d\n",a);
    }

    //Projde cely strom a vytiskne ho na STDOUT
    printf("STROM: ");
    B_tree_walk(root_ptr);
    printf("\n");
*/
    if(fclose(source_file) == EOF){
        fprintf(stderr, "Internl Error: %s\n", strerror(errno));
        return INTERNAL_ERROR;
    }

}