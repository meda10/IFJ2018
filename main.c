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
BTNode root_GTS;
BTNode current_LTS;
BTNode node;
int number_of_func;
int lts_counter = 0;

void built_in_functions(){
    string tmp;
    strInit(&tmp);

    strAddCharArray(&tmp,"inputs");
    create_node(&root_GTS, &tmp, -1, 0, NULL, true, false, true, false, &root_GTS);
    strClear(&tmp);

    strAddCharArray(&tmp,"inputi");
    create_node(&root_GTS, &tmp, -1, 0, NULL, true, false, true, false, &root_GTS);
    strClear(&tmp);

    strAddCharArray(&tmp,"inputf");
    create_node(&root_GTS, &tmp, -1, 0, NULL, true, false, true, false, &root_GTS);
    strClear(&tmp);

    strAddCharArray(&tmp,"print"); //number of params >=1  ?
    create_node(&root_GTS, &tmp, -1, 1, NULL, true, false, true, false, &root_GTS);
    strClear(&tmp);

    strAddCharArray(&tmp,"length");
    create_node(&root_GTS, &tmp, -1, 1, NULL, true, false, true, false, &root_GTS);
    strClear(&tmp);

    strAddCharArray(&tmp,"substr");
    create_node(&root_GTS, &tmp, -1, 3, NULL, true, false, true, false, &root_GTS);
    strClear(&tmp);

    strAddCharArray(&tmp,"ord");
    create_node(&root_GTS, &tmp, -1, 2, NULL, true, false, true, false, &root_GTS);
    strClear(&tmp);

    strAddCharArray(&tmp,"chr");
    create_node(&root_GTS, &tmp, -1, 1, NULL, true, false, true, false, &root_GTS);
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
            // for (int i = 0; i < params_number; ++i){
            //     printf("params[%d] :%s\n",i, params[i]->str);
            // }
            BTNode NO_LOCAL_SYM_TABLE = NULL;
            result = create_node(&root_GTS, &function_id, -1, params_number, params, true, false, true, false, &NO_LOCAL_SYM_TABLE);

            if (result != 0){
                fprintf(stderr, "Error: Multiple function definition!\n");
                return SEM_ERR;
            }
            for (int i = 1; i < params_number; ++i){    //check later if it's needed
                strFree(&params[i]);
            }
            params_number = 0;
        }
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

int main() {

    B_tree_init(&root_GTS);
    int result;
    result = fill_TS();
    if (result != SEM_OK) return SEM_ERR;

    B_tree_walk(root_GTS);
    printf("\n");

    FILE *source_file = open_file("/home/petr/CLionProjects/IFJ2018/source");
    result = parse();
    printf("%d\n", result);

    B_tree_free(root_GTS);

    if(fclose(source_file) == EOF){
        fprintf(stderr, "Internl Error: %s\n", strerror(errno));
        return INTERNAL_ERROR;
    }

    //example_lolalni_tabulky();
}