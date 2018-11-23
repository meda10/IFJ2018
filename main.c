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
BTNode current_LTS;
BTNode node;
BTNode NONE = NULL;
int number_of_func;
int lts_counter = 0;

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

int fill_TS(){
    FILE *source_file = open_file("/home/petr/CLionProjects/IFJ2018/source");
    string function_id;
    int params_number = 0;
    int initial_size = 5;
    int result;
    number_of_func = 0;
    string **params;

    built_in_functions();   //fill GTS with built-in functions first

    strInit(&function_id);  //string for storing function id
    

    params = malloc(sizeof(string*) * initial_size);
    if (params == NULL){
        fprintf(stderr, "Memmory allocation error.\n");
        return INTERNAL_ERROR;
    }
    token_t *token = make_new_token();
    get_next_token(token);
    while(token->type != ENDOFFILE){
        if (token->type == DEF){
            number_of_func++;
            get_next_token(token);  //function id
            strAddCharArray(&function_id, token->string.str);

            get_next_token(token); // OPENINGBRACKET
            while(true){
                get_next_token(token);  //parameter
                if (token->type == CLOSING_BRACKET)
                    break;

                if (params_number + 1 > initial_size){
                    initial_size *= 2;
                    params = realloc(params, sizeof(string*) * initial_size);
                }
                strInit(&params[params_number]);
                strCopyString(params[params_number], &token->string);
                params_number++;

                get_next_token(token);  //comma or closing bracket
                if (token->type == COMMA)
                    continue;
                else if (token->type == CLOSING_BRACKET)
                    break;
            }

            result = create_node(root_GTS, &function_id, -1, params_number, params, true, false, true, false, local_TS);
            if (result != 0){
                fprintf(stderr, "Error: Multiple function definition!\n");
                return SEM_ERR;
            }

            //filling local_table of function with it params as variables declaration
            for (int i = 0; i < params_number; ++i){
                create_node(local_TS, &params[i]->str, -1, 0, NULL, false, true, false, false, local_TS);
            }

            //-----------
            node = B_tree_search(*local_TS, "p1");
            if(node != NULL)
                printf("FOUND: %s\n",node->data.name);
            else
                printf("NOT FOUND: \n");
            
            node = B_tree_search(*local_TS, "p2");
            if(node != NULL)
                printf("FOUND: %s\n",node->data.name);
            else
                printf("NOT FOUND: \n");
            //------------

            for (int i = 1; i < params_number; ++i){    //check later if it's needed
                strFree(&params[i]);
            }
            params_number = 0;
        }
        //vytvoreni noveho ukazatele na lokalni tabulku
        free(local_TS);
        local_TS = make_new_table();
        strClear(&function_id);
        get_next_token(token);
    }

    strFree(&function_id);
    free(params);
    if(fclose(source_file) == EOF){
        fprintf(stderr, "Internl Error: %s\n", strerror(errno));
        return INTERNAL_ERROR;
    }
    free_token(token);
    return SEM_OK;
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
    //nevypise se nic protoze tabulka je prazdna

    //pridani uzlu do loklani tabulky + vypis
    create_node(&local_table_2, "hell", -1, 0, NULL, false, true, false, false, &local_table_2);
    printf("\n");B_tree_walk(local_table_2);printf("\n");


/*
    if(fclose(source_file) == EOF){
        fprintf(stderr, "Internl Error: %s\n", strerror(errno));
        return INTERNAL_ERROR;
    }
*/
}