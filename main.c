#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parser.h"
#include "scaner.h"
#include "symtable.h"
#include "generator.h"
#include "string.h"
#include "examples.h"
#include "tokens.h"

BTNode *local_TS;
BTNode *root_GTS;
BTNode *main_local_TS;
BTNode temp_node;
BTNode NONE = NULL;

void built_in_functions(){  //TODO:Do we need local_symtable for buillt-in functions, i think no
    create_node(root_GTS, "inputs", -1, 0, NULL, true, true, true, false, &NONE);

    create_node(root_GTS, "inputi", -1, 0, NULL, true, true, true, false, &NONE);

    create_node(root_GTS, "inputf", -1, 0, NULL, true, true, true, false, &NONE);

    create_node(root_GTS, "print", -1, 1, NULL, true, true, true, false, &NONE);

    create_node(root_GTS, "length", -1, 1, NULL, true, true, true, false, &NONE);

    create_node(root_GTS, "substr", -1, 3, NULL, true, true, true, false, &NONE);

    create_node(root_GTS, "ord", -1, 2, NULL, true, true, true, false, &NONE);

    create_node(root_GTS, "chr", -1, 1, NULL, true, true, true, false, &NONE);
}

char **make_array(){
    char **pvowels = (char **) malloc(50 * sizeof(char *));
    return pvowels;
}

int fill_sym_table(){
    read_from_stdin();
    //FILE *source_file = open_file("/home/drankou/Desktop/IFJ/project/IFJ2018/source");
    
    string function_name;
    strInit(&function_name);

    int counter = 0;

    built_in_functions();
    token_t *token = make_new_token();
    get_next_token(token);
    while(token->type != ENDOFFILE){
        if (token->type == DEF){
            free(local_TS);
            local_TS = make_new_table();

            char **params = make_array();

            get_next_token(token);
            if(token->type == IDENTIFIER){
                strFree(&function_name);
                strInit(&function_name);
                strAddCharArray(&function_name,token->string.str);
            } else{
                exit(77);
            }

            get_next_token(token);
            if(token->type != OPENNING_BRACKET){
                exit(77);
            }
            while(token->type != CLOSING_BRACKET){
                get_next_token(token);
                if(token->type != IDENTIFIER){
                    create_node(local_TS, "$$<&FUNKCE_NEMA_PARAMETRY&>$$", -1, 0, NULL, false, true, false, false, local_TS);
                    break;
                }

                params[++counter] = (char *) malloc(token->string.length + 1 * sizeof(char));
                strcpy(params[counter], token->string.str);

                create_node(local_TS, params[counter], -1, 0, NULL, false, true, false, false, local_TS);

                get_next_token(token);
                if(token->type == COMMA){
                    continue;
                } else{
                    break;
                }
            }

            int result = create_node(root_GTS, function_name.str, -1, counter, params, true, false, true, false, local_TS);
            if (result != 0){
                fprintf(stderr, "Error: Multiple function definition!\n");
                return SEM_ERR;
            }
            counter = 0;
        }
        get_next_token(token);
    }

    free_token(token);
    strFree(&function_name);
/*
    if(fclose(source_file) == EOF){
        fprintf(stderr, "Internl Error: %s\n", strerror(errno));
        return INTERNAL_ERROR;
    }
*/
    return SEM_OK;
}

int main() {
    //vytvoreni globalni tabulky symbolu
    root_GTS = make_new_table();

    //vytvoreni lokalni tabulky symbolu
    local_TS = make_new_table();

    main_local_TS = make_new_table();
    create_node(root_GTS, "$$main$$", -1, 1, NULL, true, false, true, false, main_local_TS);    
    create_node(main_local_TS, "$$<&FUNKCE_NEMA_PARAMETRY&>$$", -1, 0, NULL, false, true, false, false, main_local_TS);
    //create_node(main_local_TS, "p1", -1, 0, NULL, false, true, false, false, main_local_TS);

    int result;
    result = fill_sym_table();  //naplneni globalni tabulki funkcemi
    if (result != SEM_OK) return SEM_ERR;

    //FILE *source_file = open_file("/home/drankou/Desktop/IFJ/project/IFJ2018/source");
    read_again();

    result = parse();
    printf("SYNTAX: %d\n", result);
    if(result != SYNTAX_OK) return SYNTAX_ERR;

    //globalni tabulka
    printf("Global Table:\n");
    B_tree_walk(*root_GTS);
    printf("\n");
    
    printf("MAIN BODY:\n");
    B_tree_walk(*main_local_TS);
    printf("\n");

    // //nalezeni a vzpis loklani tabulky --> funkce vypada takto --> def p1 (b,c,d)
    // BTNode local_table_1;
    // local_table_1 = B_tree_search_local_table(*root_GTS,"p1");
    // printf("\n");B_tree_walk(local_table_1);printf("\n");


    // //nalezeni a vzpis loklani tabulky --> funkce vypada takto --> def p2 ()
    // BTNode local_table_2;
    // local_table_2 = B_tree_search_local_table(*root_GTS,"p2");
    // printf("\n");B_tree_walk(local_table_2);printf("\n");

    // //pridani uzlu do loklani tabulky + vypis
    // create_node(&local_table_2, "hell", -1, 0, NULL, false, true, false, false, &local_table_2);
    // create_node(&local_table_2, "AAAAA", -1, 0, NULL, false, true, false, false, &local_table_2);

    // local_table_2 = B_tree_search_local_table(*root_GTS,"p2");
    // printf("\n");B_tree_walk(local_table_2);printf("\n");

    // B_tree_free(*root_GTS);

/*
    if(fclose(source_file) == EOF){
        fprintf(stderr, "Internl Error: %s\n", strerror(errno));
        return INTERNAL_ERROR;
    }
*/

    //FREE MEMORY
    B_tree_free(*root_GTS);
    free(root_GTS);
    free(local_TS);
    free(main_local_TS);
    //only when reading from stdin
    free_buffer();
}