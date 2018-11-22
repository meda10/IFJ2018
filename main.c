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
    B_tree_init(&root_GTS); //todo B_tree_free(root_GTS)
    int result;
    result = fill_TS();
    if (result != SEM_OK) return SEM_ERR;

    FILE *source_file = open_file("/home/petr/CLionProjects/IFJ2018/source");
    result = parse();
    ///B_tree_free(root_GTS); //todo B_tree_free(root_GTS)
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
    //--------------------------------------------------
    //                   Priklad 1
    //--------------------------------------------------
    /*
     ---- Pseudo kod ----
     int result; //globalni promena

     int a = 0;
     a = 50;
     int b = 0;
     b = 15;

     if(5 > 3){
        if(a == 50){
            result = a + b;
            printf(result);
        }else{
            result = "FAIL";
            printf(result);
        }
     }else {
        if(b >= 16){
            result = a - b;
            printf(result);
        }else{
            result = "SUCCESS" + " END!!";
            printf(result);
        }
     }

     string str;
     str = "Muj string\n +#-->\\"
     printf(str)

     */

    /*
    //start generatoru
    generate_start();
    generate_main();

    //deklarace promene a prirazeni hodnoty
    variable_declare(INTEGER_TYPE,"a");
    generate_variable_assign(INT_E,"a","50");

    //deklarace promene a prirazeni hodnoty
    variable_declare(INTEGER_TYPE,"b");
    generate_variable_assign(INT_E,"b","15");

    //push hodnot co se maji porovnat
    generate_push(INTEGER_TYPE,"5");
    generate_push(INTEGER_TYPE,"10");

    //porovnani hodnot
    generate_comparative_operations(G_TYPE_GREATER);

    //pop hodnoty a test IF podminky
    generate_pop_to_result();
    generate_if(1);
        //push hodnot na zasobnik a test IF podminky
        generate_push(IDENTIFIER,"a");
        generate_push(INTEGER_TYPE,"50");
        generate_comparative_operations(G_TYPE_EQUAL);
        generate_pop_to_result();
        //vnorent IF
        generate_if(2);
            generate_push(IDENTIFIER,"a");
            generate_push(IDENTIFIER,"b");
            generate_mathemeatical_operations(G_TYPE_PLUS);
            //vypis
            generate_pop_to_result();
            generate_print_result();
        generate_else(2,true);
            generate_push(STRING_TYPE,"FAIL");
            generate_pop_to_result();
            generate_print_result();
        generate_if_else_end(2);

    //ELSE
    generate_else(1,true);
        //push hodnot na zasobnik a test IF podminky
        generate_push(IDENTIFIER,"b");
        generate_push(INTEGER_TYPE,"16");
        generate_comparative_operations(G_TYPE_GREATER_OR_EQUAL);
        generate_pop_to_result();
        //vnorent IF
        generate_if(3);
            generate_push(IDENTIFIER,"a");
            generate_push(IDENTIFIER,"b");
            generate_mathemeatical_operations(G_TYPE_MINUS);
            //vypis
            generate_pop_to_result();
            generate_print_result();
        generate_else(3,true);
            generate_push(STRING_TYPE,"SUCCESS");
            generate_push(STRING_TYPE," END!!");
            generate_concat();
            generate_pop_to_result();
            generate_print_result();
        generate_if_else_end(3);
    generate_if_else_end(1);

    //test stringu
    variable_declare(STRING_TYPE,"str");
    generate_variable_assign(STRING_E,"str","\nMuj string\n +#-->\\\n");

    //vypis promene
    generate_print(IDENTIFIER,"str");

    //konec
    generate_main_end();

    printf("%s",instrukce.str);

    generate_free_memory();
    */
    //--------------------------------------------------

    //--------------------------------------------------
    //                   Priklad 2
    //--------------------------------------------------
    /*
     int result; //globalni promena

     int a = 0;
     a = 5;
     int b = 0;
     b = 10;

     while(a != b){
        a = a + 1;
        printf(a);
        printf(\n);
     }

     */


    generate_start();
    generate_main();

    variable_declare(INTEGER_TYPE,"a");
    generate_variable_assign(INT_E,"a","0");
    variable_declare(INTEGER_TYPE,"b");
    generate_variable_assign(INT_E,"b","10");

    //podminka while
    generate_while_condition_check(1);
    generate_push(IDENTIFIER,"a");
    generate_push(INTEGER_TYPE,"10");
    generate_comparative_operations(G_TYPE_NOT_EQUAL);
    generate_pop_to_result();

    //telo while
    generate_while_start(1);
        generate_push(IDENTIFIER,"a");
        generate_push(INTEGER_TYPE,"1");
        generate_mathemeatical_operations(G_TYPE_PLUS);
        generate_pop_to_variable("a");
        generate_print(IDENTIFIER,"a");
        generate_print(STRING_TYPE,"\n");
    generate_while_end(1);


    generate_main_end();

    printf("%s",instrukce.str);

    generate_free_memory();

    //--------------------------------------------------





    //free_labels();
    if(fclose(source_file) == EOF){
        fprintf(stderr, "Internl Error: %s\n", strerror(errno));
        return INTERNAL_ERROR;
    }
}