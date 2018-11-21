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
#include "tokens.h"

BTNode *local_TS;
BTNode root_GTS;
BTNode node;
int number_of_func;
int ts_number;

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
    ts_number = 5; 
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
            strCopyString(&function_id, &token->string);

            get_next_token(token); // OPENINGBRACKET
            while(true){
                get_next_token(token);  //parameter
                if (token->type == CLOSING_BRACKET)
                    break;

                if (params_number + 1 > initial_size){
                    initial_size *= 2;
                    params = realloc(params, sizeof(string*) * initial_size);
                }
                //TODO params
                params[params_number] = &token->string;
                //printf("%s\n", token->string.str);
                params_number++;

                get_next_token(token);  //comma or closing bracket
                if (token->type == COMMA)
                    continue;
                else if (token->type == CLOSING_BRACKET)
                    break;
            }
            // params[0]->str = "afaff";
            // printf("params[0] :%s\n", params[0]->str);
            result = create_node(&root_GTS, &function_id, -1, params_number, params, true, false, true, false, NULL);
            if (result != 0){
                //TODO if it should be an error
                printf("Warning: Multiple function definition!\n");
                return SEM_ERR;
            }
        }
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

    FILE *source_file = open_file("/home/petr/CLionProjects/IFJ2018/source");
    result = parse();
    //printf("%d\n", result);
/*
    printf("GTS :\n");
    B_tree_walk(root_GTS);
    printf("\n");

    BTNode n;
    char *c= "inputi";
    node = B_tree_search(root_GTS,c);
    if(node != NULL){
        printf("FOUND: %s\n",node->data.name);
    } else{
        printf("NOT FOUND: %s\n",c);
    }
    printf("\n");



    B_tree_free(root_GTS);
*/

    
    /**
     * Tabulka symbolu - priklad pouziti
     */
/*
    
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

    /**
     * Generator
     */


    //start generatoru
    generate_start();
    generate_main();

    //deklarace promene a prirayeni hodnoty
    variable_declare(INTEGER_TYPE,"a");
    generate_variable_assign(INT_E,"a","87");

    //push hodnot na zasobnik a secteni
    generate_push(IDENTIFIER,"a");
    generate_push(DOUBLE_TYPE,"55.5");
    generate_mathemeatical_operations(G_TYPE_PLUS);

    //vypis hodnoty
    gensr("");
    generate_print_result();

    //konec
    generate_main_end();

    if(fclose(source_file) == EOF){
        fprintf(stderr, "Internl Error: %s\n", strerror(errno));
        return INTERNAL_ERROR;
    }
}