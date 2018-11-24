#include <zconf.h>
#include <malloc.h>
#include <stdlib.h>
#include "stringss.h"
#include "symtable.h"
#include "scaner.h"
#include "generator.h"
#include "tokens.h"

void example_lolalni_tabulky(){
    string tmp;
    strInit(&tmp);

    ///vytvoření globální tabulky
    BTNode *ROOT;
    ROOT = make_new_table();

    ///vztvoření lokální tabulky
    BTNode *local_table;
    local_table = make_new_table();

    //lokalni tabulka - paramater 1
    strAddCharArray(&tmp,"Local_1");
    create_node(local_table, tmp.str, 5, 80, NULL, true, false, true, false, local_table);
    strClear(&tmp);

    //lokalni tabulka - parametr 2
    strAddCharArray(&tmp,"Local_2");
    create_node(local_table, tmp.str, 5, 80, NULL, true, false, true, false, local_table);
    strClear(&tmp);



    //globalni tabulka - FUNKCE = odkaz na LOKALNI TABULKU ktera obsahuje parametry funkce
    strAddCharArray(&tmp,"FUNKCE");
    create_node(ROOT, tmp.str, -1, 3, NULL, true, false, true, false, local_table);
    strClear(&tmp);

    //globalni tabulka - GLOBALNI PROMENA
    strAddCharArray(&tmp,"random_globalni_promena");
    create_node(ROOT, tmp.str, -1, 4, NULL, true, false, true, false, ROOT);
    strClear(&tmp);

    /*
     * další funkce
    */

    //vytvoření nového ukazatekle na lokalni tabulku
    free(local_table);
    local_table = make_new_table();

    //Dalsi lokalni tabulka
    strAddCharArray(&tmp,"Another_Local_1");
    create_node(local_table, tmp.str, 5, 14, NULL, true, false, true, false, local_table);
    strClear(&tmp);

    strAddCharArray(&tmp,"Another_Local_2");
    create_node(local_table, tmp.str, 5, 14, NULL, true, false, true, false, local_table);
    strClear(&tmp);

    //globalni tabulka - přidání FUNKCE_2 = obsahuje odkaz na lokalnitabulku naplněnou parametry
    strAddCharArray(&tmp,"FUNKCE_2");
    create_node(ROOT, tmp.str, -1, 5, NULL, true, false, true, false, local_table);
    strClear(&tmp);


    // walk ROOOT
    B_tree_walk(*ROOT);
    printf("\n");

    //nalezeni lokalni tabulky
    BTNode local_table_1;
    local_table_1 = B_tree_search_local_table(*ROOT,"FUNKCE");
    //walk local table
    B_tree_walk(local_table_1);printf("\n");

    //nalezeni lokalni tabulky
    BTNode local_table_2;
    local_table_2 = B_tree_search_local_table(*ROOT,"FUNKCE_2");
    //walk local table
    B_tree_walk(local_table_2);printf("\n");
    /*
        BTNode local_table_3;
        local_table_2 = B_tree_search_local_table(*ROOT,"random_globalni_promena");
        if(local_table_3 == NULL){
            printf("HELL\n");
        }
    */

    free(ROOT);
    free(local_table);
    B_tree_free(local_table_1);
    B_tree_free(local_table_2);
    //B_tree_free(r);
    B_tree_free(*ROOT);
    delete_string(&tmp);
}

void example_generator_1 (){
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
}

void example_generator_2 (){
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

}

void example_generator_3(){

    //--------------------------------------------------
    //                   Priklad 3
    //--------------------------------------------------
    /*
     int result; //globalni promena
     function HELL(a,b){
        return a * b
     }
     main{
        int x;
        x = HELL(10,5);
        printf(x);
     }
     */
    generate_start();

    //funkce
    generate_function_start("HELL");
    generate_read_function_params(1, "a");
    generate_read_function_params(2, "b");
    generate_push(IDENTIFIER,"a");
    generate_push(IDENTIFIER,"b");
    generate_mathemeatical_operations(G_TYPE_MUL);
    generate_function_return("HELL");
    generate_function_end("HELL");

    //Main
    generate_main();

    variable_declare(INTEGER_TYPE,"x");

    //předání parametrů funkce
    generate_TF_for_function_args();
    generate_assign_arguments_to_function(INT_E, 1, "10");
    generate_assign_arguments_to_function(INT_E, 2, "5");
    //volani funkce
    generate_function_call("HELL");
    //Přiřazení návratové hodnoty funkce do c
    generate_function_return_value_assign_to_var("x");
    generate_print(IDENTIFIER,"x");
    generate_main_end();


    printf("%s",instrukce.str);

    generate_free_memory();
};

void example_scanner(){

    token_t *token = make_new_token();
    for (int i = 0; i < 500; ++i) {
        get_next_token(token);
        if(token->type == ENDOFFILE){
            print_token(token);
            break;
        } else{
            //if(token->type != EOL)
            print_token(token);
        }
    }
    free_token(token);
}

void example_sym_table_fill(BTNode *root_GTS,char *f_1_name,char *f_2_name){
    //globalni tabulka
    B_tree_walk(*root_GTS);

    //nalezeni a vzpis loklani tabulky --> funkce vypada takto --> def p1 (b,c,d)
    BTNode local_table_1;
    local_table_1 = B_tree_search_local_table(*root_GTS,f_1_name);
    printf("\n");B_tree_walk(local_table_1);printf("\n");


    //nalezeni a vzpis loklani tabulky --> funkce vypada takto --> def p2 ()
    BTNode local_table_2;
    local_table_2 = B_tree_search_local_table(*root_GTS,f_2_name);
    printf("\n");B_tree_walk(local_table_2);printf("\n");

    //pridani uzlu do loklani tabulky + vypis
    create_node(&local_table_2, "Test_param_1", -1, 0, NULL, false, true, false, false, &local_table_2);
    create_node(&local_table_2, "Test_param_2", -1, 0, NULL, false, true, false, false, &local_table_2);

    local_table_2 = B_tree_search_local_table(*root_GTS,f_2_name);
    printf("\n");B_tree_walk(local_table_2);printf("\n");
}