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
BTNode node;
BTNode NONE = NULL;

void built_in_functions(){  //TODO Do we need local_symtable for buillt-in functions, i think no
    string tmp;
    strInit(&tmp);

    strAddCharArray(&tmp,"inputs");
    create_node(root_GTS, tmp.str, -1, 0, NULL, true, false, true, false, &NONE);

    strClear(&tmp);

    strAddCharArray(&tmp,"inputi");
    create_node(root_GTS, tmp.str, -1, 0, NULL, true, false, true, false, &NONE);
    strClear(&tmp);

    strAddCharArray(&tmp,"inputf");
    create_node(root_GTS, tmp.str, -1, 0, NULL, true, false, true, false, &NONE);
    strClear(&tmp);

    strAddCharArray(&tmp,"print"); //number of params >=1  ?
    create_node(root_GTS, tmp.str, -1, 1, NULL, true, false, true, false, &NONE);
    strClear(&tmp);

    strAddCharArray(&tmp,"length");
    create_node(root_GTS, tmp.str, -1, 1, NULL, true, false, true, false, &NONE);
    strClear(&tmp);

    strAddCharArray(&tmp,"substr");
    create_node(root_GTS, tmp.str, -1, 3, NULL, true, false, true, false, &NONE);
    strClear(&tmp);

    strAddCharArray(&tmp,"ord");
    create_node(root_GTS, tmp.str, -1, 2, NULL, true, false, true, false, &NONE);
    strClear(&tmp);

    strAddCharArray(&tmp,"chr");
    create_node(root_GTS, tmp.str, -1, 1, NULL, true, false, true, false, &NONE);
    strClear(&tmp);

    delete_string(&tmp);
}

char **make_array(){
    char **pvowels = (char **) malloc(50 * sizeof(char *));
    return pvowels;
}

int fill_sym_table(){
    FILE *source_file = open_file("/home/petr/CLionProjects/IFJ2018/source");
    string function_name;
    strInit(&function_name);

    int cunter = 0;

    built_in_functions();
    token_t *token = make_new_token();
    get_next_token(token);
    while(token->type != ENDOFFILE){
        if (token->type == DEF){
            free(local_TS);
            local_TS = make_new_table();

            char **pvowels = make_array();


            get_next_token(token);
            strFree(&function_name);
            strInit(&function_name);
            strAddCharArray(&function_name,token->string.str);

            get_next_token(token);
            while(token->type != CLOSING_BRACKET){
                get_next_token(token);
                if(token->type != IDENTIFIER){
                    create_node(local_TS, "$$<&FUNKCE_NEMA_PARAMETRY&>$$", -1, 0, NULL, false, true, false, false, local_TS);
                    break;
                }

                pvowels[++cunter] = (char *) malloc(token->string.length + 1 * sizeof(char));
                strcpy(pvowels[cunter], token->string.str);

                create_node(local_TS, pvowels[cunter], -1, 0, NULL, false, true, false, false, local_TS);

                get_next_token(token);
                if(token->type == COMMA){
                    continue;
                } else{
                    break;
                }
            }

            int result = create_node(root_GTS, function_name.str, -1, cunter, pvowels, true, false, true, false, local_TS);
            if (result != 0){
                fprintf(stderr, "Error: Multiple function definition!\n");
                return SEM_ERR;
            }
            cunter = 0;
        }
        get_next_token(token);
    }

    free_token(token);
    strFree(&function_name);
    return SEM_OK;
}

int main() {
    //vytvoreni globalni tabulky symbolu
    root_GTS = make_new_table();

    //vytvoreni lokalni tabulky symbolu
    local_TS = make_new_table();

    int result;
    //naplneni globalni tabulki funkcemi
    result = fill_sym_table();
    if (result != SEM_OK) return SEM_ERR;

    //globalni tabulka
    B_tree_walk(*root_GTS);

    //nalezeni a vzpis loklani tabulky --> funkce vypada takto --> def p1 (b,c,d)
    BTNode local_table_1;
    local_table_1 = B_tree_search_local_table(*root_GTS,"p1");
    printf("\n");B_tree_walk(local_table_1);printf("\n");


    //nalezeni a vzpis loklani tabulky --> funkce vypada takto --> def p2 ()
    BTNode local_table_2;
    local_table_2 = B_tree_search_local_table(*root_GTS,"p2");
    printf("\n");B_tree_walk(local_table_2);printf("\n");

    //pridani uzlu do loklani tabulky + vypis
    create_node(&local_table_2, "hell", -1, 0, NULL, false, true, false, false, &local_table_2);
    create_node(&local_table_2, "AAAAA", -1, 0, NULL, false, true, false, false, &local_table_2);

    local_table_2 = B_tree_search_local_table(*root_GTS,"p2");
    printf("\n");B_tree_walk(local_table_2);printf("\n");

    B_tree_free(*root_GTS);


/*
    if(fclose(source_file) == EOF){
        fprintf(stderr, "Internl Error: %s\n", strerror(errno));
        return INTERNAL_ERROR;
    }
*/
}