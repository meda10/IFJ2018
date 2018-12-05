/*
 * FIT VUT, IFJ 2018
 * Authors:
 *    Aliaksandr Drankou, xdrank00
 *    Petr Medek, xmedek07
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parser.h"
#include "scaner.h"
#include "symtable.h"
#include "generator.h"
#include "error.h"
#include "stringss.h"
#include "tokens.h"

BTNode *local_TS;
BTNode *root_GTS;
BTNode *main_local_TS;
BTNode temp_node;
BTNode NONE = NULL;

void built_in_functions(){
    create_node(root_GTS, "inputs", -1, 0, NULL, true, true, true, false, &NONE);

    create_node(root_GTS, "inputi", -1, 0, NULL, true, true, true, false, &NONE);

    create_node(root_GTS, "inputf", -1, 0, NULL, true, true, true, false, &NONE);

    create_node(root_GTS, "print", -1, 1, NULL, true, true, true, false, &NONE);

    create_node(root_GTS, "length", -1, 1, NULL, true, true, true, false, &NONE);

    create_node(root_GTS, "substr", -1, 3, NULL, true, true, true, false, &NONE);

    create_node(root_GTS, "ord", -1, 2, NULL, true, true, true, false, &NONE);

    create_node(root_GTS, "chr", -1, 1, NULL, true, true, true, false, &NONE);
}

int fill_sym_table(){
    read_from_stdin();

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
                errors_exit(SYNTAX_ERROR,"Syntax error in function definition.\n");
            }

            get_next_token(token);
            if(token->type != OPENNING_BRACKET){
                errors_exit(SYNTAX_ERROR,"Syntax error in function definition.\n");
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
                errors_exit(SEMANTIC_ERROR_UNDEFINED_VARIABLE,"Multiple function definition!\n");
            }
            counter = 0;
        }
        get_next_token(token);
    }

    free_token(token);
    strFree(&function_name);

    return RETURN_OK;
}

int main() {
    //vytvoreni globalni tabulky symbolu
    root_GTS = make_new_table();

    //vytvoreni lokalni tabulky symbolu
    local_TS = make_new_table();

    main_local_TS = make_new_table();
    create_node(root_GTS, "$$main$$", -1, 1, NULL, true, false, true, false, main_local_TS);    
    create_node(main_local_TS, "$$<&FUNKCE_NEMA_PARAMETRY&>$$", -1, 0, NULL, false, true, false, false, main_local_TS);

    int result;
    result = fill_sym_table();  //naplneni globalni tabulki funkcemi
    if (result != RETURN_OK)
        errors_exit(INTERNAL_ERROR, "internal compiler error.\n");

    read_again();

    generate_start();
    generate_main();
    result = parse();
    if(result != RETURN_OK)
        errors_exit(SYNTAX_ERROR, "syntax error.\n");

    generate_main_end();
    printf("%s",instrukce.str);
    generate_free_memory();

    //FREE MEMORY
    B_tree_free(*root_GTS);
    free(root_GTS);
    free(local_TS);
    free(main_local_TS);
    //only when reading from stdin
    free_buffer();
}