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

    string tmp;
    strInit(&tmp);

    BTNode ROOT;     B_tree_init(&ROOT);
    BTNode NEMA_LOCALNI_TABULKU = NULL;
    BTNode TAB_2;    B_tree_init(&TAB_2);

    //lokalni tabulka - prvni prvak
    strAddCharArray(&tmp,"Local_1");
    create_node(&TAB_2, &tmp, 5, 80, NULL, true, false, true, false, &TAB_2);
    strClear(&tmp);

    //globalni tabulka
    strAddCharArray(&tmp,"GLOBAL_A");
    create_node(&ROOT, &tmp, -1, 2, NULL, true, false, true, false, &NEMA_LOCALNI_TABULKU);
    strClear(&tmp);

    //globalni tabulka - odkaz na LOKALNI TABULKU
    strAddCharArray(&tmp,"GLOBAL_B");
    create_node(&ROOT, &tmp, -1, 3, NULL, true, false, true, false, &TAB_2);
    strClear(&tmp);

    //globalni tabulka
    strAddCharArray(&tmp,"GLOBAL_C");
    create_node(&ROOT, &tmp, -1, 4, NULL, true, false, true, false, &NEMA_LOCALNI_TABULKU);
    strClear(&tmp);

    //globalni tabulka
    strAddCharArray(&tmp,"GLOBAL_D");
    create_node(&ROOT, &tmp, -1, 5, NULL, true, false, true, false, &NEMA_LOCALNI_TABULKU);
    strClear(&tmp);

    //lokalni tabulka dalsi prvek
    strAddCharArray(&tmp,"Local_2");
    create_node(&TAB_2, &tmp, 5, 80, NULL, true, false, true, false, &TAB_2);
    strClear(&tmp);
    delete_string(&tmp);

    BTNode i;
    BTNode g;

    // walk ROOOT
    B_tree_walk(ROOT);
    printf("\n");

    //nalezeni lokalni tabulky
    g = B_tree_search(ROOT,"GLOBAL_B");
    i = g->data.local_sym_table;

    //walk local table
    B_tree_walk(i);
    printf("\n");

    B_tree_free(i);
    B_tree_free(ROOT);



        //--------- Implementing array of localsymtables-------------\\
    // local_TS = malloc(sizeof(BTNode) * (number_of_func + 1));    //1 == main function
    // memcpy(local_TS[lts_counter], root_GTS, sizeof(BTNode));
    // current_LTS = local_TS[lts_counter];    //set current_LTS
    
    // B_tree_walk(local_TS[lts_counter]);   //prints only last(inputs), why?
    // lts_counter++;

    //-----------------------------------------------------------

    
    FILE *source_file = open_file("/home/petr/CLionProjects/IFJ2018/source");
    //result = parse();
    //printf("%d\n", result);

    //B_tree_free(root_GTS);

    
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


    // generate_start();
    // generate_main();

    // variable_declare(INTEGER_TYPE,"a");
    // generate_variable_assign(INT_E,"a","0");
    // variable_declare(INTEGER_TYPE,"b");
    // generate_variable_assign(INT_E,"b","10");

    // //podminka while
    // generate_while_condition_check(1);
    // generate_push(IDENTIFIER,"a");
    // generate_push(INTEGER_TYPE,"10");
    // generate_comparative_operations(G_TYPE_NOT_EQUAL);
    // generate_pop_to_result();

    // //telo while
    // generate_while_start(1);
    //     generate_push(IDENTIFIER,"a");
    //     generate_push(INTEGER_TYPE,"1");
    //     generate_mathemeatical_operations(G_TYPE_PLUS);
    //     generate_pop_to_variable("a");
    //     generate_print(IDENTIFIER,"a");
    //     generate_print(STRING_TYPE,"\n");
    // generate_while_end(1);


    // generate_main_end();

    // printf("%s",instrukce.str);

    // generate_free_memory();

    //--------------------------------------------------



    //free_labels();
    if(fclose(source_file) == EOF){
        fprintf(stderr, "Internl Error: %s\n", strerror(errno));
        return INTERNAL_ERROR;
    }
}