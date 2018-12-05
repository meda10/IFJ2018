#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generator.h"
#include "tokens.h"
#include "scaner.h"
#include "stringss.h"
#include "symtable.h"


int get_new_label_number(){
    return ++label_number;
}

//generovani vestavene funkci print
void generate_print(){
    strAddCharArray(&instrukce,"\n#PRINT\n");
    generate_function_start("print");

    strAddCharArray(&instrukce,"WRITE LF@V_0\n");
    strAddCharArray(&instrukce,"JUMP $$FUN_PRINT_END\n");

    generate_function_end("PRINT");
}


//generovani vestavene funkci inputs
void generate_inputs() {
    strAddCharArray(&instrukce,"\n#INPUTS\n");
    generate_function_start("inputs");

    strAddCharArray(&instrukce,"READ LF@$$FUN_RET string\n");

    strAddCharArray(&instrukce,"TYPE GF@$$var_1 LF@$$FUN_RET\n");
    strAddCharArray(&instrukce,"JUMPIFEQ $$FUN_INPUTS_END GF@$$var_1 string@string\n");
    strAddCharArray(&instrukce,"JUMPIFEQ $$FUN_INPUTS_FAIL LF@$$FUN_RET nil@nil\n");
    strAddCharArray(&instrukce,"JUMP $$FUN_INPUTS_END\n");

    strAddCharArray(&instrukce,"LABEL $$FUN_INPUTS_FAIL\n");
    strAddCharArray(&instrukce,"MOVE LF@$$FUN_RET nil@nil\n");
    strAddCharArray(&instrukce,"JUMP $$FUN_INPUTS_END\n");

    generate_function_end("INPUTS");
}


//generovani vestavene funkci inputi
void generate_inputi() {
    strAddCharArray(&instrukce,"\n#INPUTI\n");
    generate_function_start("inputi");

    strAddCharArray(&instrukce,"READ LF@$$FUN_RET int\n");
    strAddCharArray(&instrukce,"JUMPIFEQ $$FUN_INPUTI_FAIL LF@$$FUN_RET int@0\n");
    strAddCharArray(&instrukce,"JUMP $$FUN_INPUTI_END\n");

    strAddCharArray(&instrukce,"LABEL $$FUN_INPUTI_FAIL\n");
    strAddCharArray(&instrukce,"MOVE LF@$$FUN_RET int@0\n");
    strAddCharArray(&instrukce,"JUMP $$FUN_INPUTI_END\n");

    generate_function_end("INPUTI");
}

//generovani vestavene funkci inputf
void generate_inputf() {
    strAddCharArray(&instrukce,"\n#INPUTF\n");
    generate_function_start("inputf");

    strAddCharArray(&instrukce,"READ LF@$$FUN_RET float\n");
    strAddCharArray(&instrukce,"JUMPIFEQ $$FUN_INPUTF_FAIL LF@$$FUN_RET float@0x0p+0\n");
    strAddCharArray(&instrukce,"JUMP $$FUN_INPUTF_END\n");

    strAddCharArray(&instrukce,"LABEL $$FUN_INPUTF_FAIL\n");
    strAddCharArray(&instrukce,"MOVE LF@$$FUN_RET float@0x0p+0\n");
    strAddCharArray(&instrukce,"JUMP $$FUN_INPUTF_END\n");

    generate_function_end("INPUTF");
}

//generovani vestavene funkci length
void generate_length() {
    strAddCharArray(&instrukce,"\n#LENGTH\n");
    generate_function_start("length");

    strAddCharArray(&instrukce,"TYPE GF@$$var_1 LF@V_0\n");
    strAddCharArray(&instrukce,"JUMPIFEQ $$FUN_LENGTH_B GF@$$var_1 string@string\n");
    strAddCharArray(&instrukce,"JUMPIFEQ $$FUN_LENGTH_B GF@$$var_1 string@int\n");
    strAddCharArray(&instrukce,"JUMPIFEQ $$FUN_LENGTH_B GF@$$var_1 string@float\n");
    strAddCharArray(&instrukce,"JUMPIFEQ $$FUN_LENGTH_FAIL LF@V_0 nil@nil\n");

    strAddCharArray(&instrukce,"LABEL $$FUN_LENGTH_B\n");
    strAddCharArray(&instrukce,"STRLEN LF@$$FUN_RET LF@V_0\n"); //data musi byt na LF@V_0
    strAddCharArray(&instrukce,"JUMP $$FUN_LENGTH_END\n");

    strAddCharArray(&instrukce,"LABEL $$FUN_LENGTH_FAIL\n");
    strAddCharArray(&instrukce,"MOVE LF@$$FUN_RET int@0\n");

    generate_function_end("LENGTH");
}

//generovani vestavene funkci ord
void generate_ord(){
    strAddCharArray(&instrukce,"\n#ORD\n");
    generate_function_start("ord");

    strAddCharArray(&instrukce,"STRLEN LF@$$FUN_RET LF@V_0\n");
    strAddCharArray(&instrukce,"GT GF@$$var_1 LF@V_1 LF@$$FUN_RET\n");
    strAddCharArray(&instrukce,"LT GF@$$var_2 LF@V_1 int@1\n");

    strAddCharArray(&instrukce,"JUMPIFEQ $$FUN_ORD_FAIL GF@$$var_1 bool@true\n");
    strAddCharArray(&instrukce,"JUMPIFEQ $$FUN_ORD_FAIL GF@$$var_2 bool@true\n");

    strAddCharArray(&instrukce,"LABEL $$FUN_ORD_OK\n");
    strAddCharArray(&instrukce,"SUB LF@V_1 LF@V_1 int@1\n");
    strAddCharArray(&instrukce,"STRI2INT LF@$$FUN_RET LF@V_0 LF@V_1\n");

    generate_function_end("ORD");

    strAddCharArray(&instrukce,"LABEL $$FUN_ORD_FAIL\n");
    strAddCharArray(&instrukce,"MOVE LF@$$FUN_RET nil@nil\n");
    strAddCharArray(&instrukce,"JUMP $$FUN_ORD_END\n");
}

//generovani vestavene funkci substr
void generate_substr(){
/*
    strAddCharArray(&instrukce,"LT GF@$$var_1 LF@V_0 int@0\n");
    strAddCharArray(&instrukce,"JUMPIFEQ $$FUN_substr_RETURN_NIL GF@$$var_1 bool@true\n");
    strAddCharArray(&instrukce,"GT GF@$$var_1 LF@V_0 int@255\n");
    strAddCharArray(&instrukce,"JUMPIFEQ $$FUN_chr_RETURN LF@RAN bool@true\n");
    strAddCharArray(&instrukce,"JUMP $$FUN_substr_A\n");


    strAddCharArray(&instrukce,"LABEL $$FUN_substr_RETURN_NIL\n");
    strAddCharArray(&instrukce,"MOVE LF@$$FUN_RET nil@nil\n");
    strAddCharArray(&instrukce,"POPFRAME\n");
    strAddCharArray(&instrukce,"RETURN\n");

*/
    strAddCharArray(&instrukce,"\n#Substr\n");
    generate_function_start("substr");
    strAddCharArray(&instrukce,"MOVE LF@$$FUN_RET string@\n");
    strAddCharArray(&instrukce,"SUB LF@V_1 LF@V_1 int@1\n");
    strAddCharArray(&instrukce,"SUB LF@V_2 LF@V_2 int@1\n");
    strAddCharArray(&instrukce,"DEFVAR LF@$$pom\n");
    strAddCharArray(&instrukce,"STRLEN LF@$$pom LF@V_0\n");
    strAddCharArray(&instrukce,"JUMPIFEQ SUBSTR_EQ LF@$$pom int@0\n");
    strAddCharArray(&instrukce,"DEFVAR LF@$$pom_1\n");
    strAddCharArray(&instrukce,"LT LF@$$pom_1 LF@V_1 int@0\n");
    strAddCharArray(&instrukce,"JUMPIFEQ SUBSTR_EQ LF@$$pom_1 bool@true\n");
    strAddCharArray(&instrukce,"LABEL SUBSTR_AG\n");
    strAddCharArray(&instrukce,"CREATEFRAME\n");
    strAddCharArray(&instrukce,"DEFVAR TF@$$pom_2\n");
    strAddCharArray(&instrukce,"DEFVAR TF@$$pom_3\n");
    strAddCharArray(&instrukce,"GETCHAR TF@$$pom_3 LF@V_0 LF@V_1\n");
    strAddCharArray(&instrukce,"ADD LF@V_1 LF@V_1 int@1\n");
    strAddCharArray(&instrukce,"CONCAT LF@$$FUN_RET LF@$$FUN_RET TF@$$pom_3\n");
    strAddCharArray(&instrukce,"JUMPIFEQ SUBSTR_EQ LF@V_2 int@0\n");
    strAddCharArray(&instrukce,"SUB LF@V_2 LF@V_2 int@1\n");
    strAddCharArray(&instrukce,"LT TF@$$pom_2 LF@V_1 LF@$$pom\n");
    strAddCharArray(&instrukce,"JUMPIFEQ SUBSTR_AG TF@$$pom_2 bool@true\n");
    strAddCharArray(&instrukce,"LABEL SUBSTR_EQ\n");
    strAddCharArray(&instrukce,"POPFRAME\n");
    strAddCharArray(&instrukce,"RETURN\n");
}

//generovani vestavene funkci chr
void generate_chr(){
    strAddCharArray(&instrukce,"\n#CHR\n");
    generate_function_start("chr");

    strAddCharArray(&instrukce,"DEFVAR LF@RAN\n");
    strAddCharArray(&instrukce,"LT LF@RAN LF@V_0 int@0\n");
    strAddCharArray(&instrukce,"JUMPIFEQ $$FUN_chr_RETURN LF@RAN bool@true\n");
    strAddCharArray(&instrukce,"GT LF@RAN LF@V_0 int@255\n");
    strAddCharArray(&instrukce,"JUMPIFEQ $$FUN_chr_RETURN LF@RAN bool@true\n");
    strAddCharArray(&instrukce,"JUMP $$FUN_chr_END\n");

    strAddCharArray(&instrukce,"LABEL $$FUN_chr_RETURN\n");
    strAddCharArray(&instrukce,"EXIT int@4\n");
    strAddCharArray(&instrukce,"POPFRAME\n");
    strAddCharArray(&instrukce,"RETURN\n");

    strAddCharArray(&instrukce,"LABEL $$FUN_chr_END\n");
    strAddCharArray(&instrukce,"INT2CHAR LF@$$FUN_RET LF@V_0\n");
    strAddCharArray(&instrukce,"POPFRAME\n");
    strAddCharArray(&instrukce,"RETURN\n");

}

//Deklarace a pocatecni inicializace proměnné na nil
void variable_declare(char *name) {
    char str[MAX_INSTRUCTION_LEN];
    strAddCharArray(&instrukce,"# Variable declare\n");
    sprintf(str, "DEFVAR LF@%s\n", name);
    strAddCharArray(&instrukce,str);
    sprintf(str, "MOVE LF@%s nil@nil\n", name);
    strAddCharArray(&instrukce,str);
}

//Zacatek programu, header, pomocne promenne a skok do hlavniho tela
void generate_start(){
    strInit(&instrukce);
    label_number = 0;
    strAddCharArray(&instrukce,".IFJcode18\n");
    strAddCharArray(&instrukce,"DEFVAR GF@$$var_1\n");
    strAddCharArray(&instrukce,"DEFVAR GF@$$var_2\n");
    strAddCharArray(&instrukce,"DEFVAR GF@$$var_3\n");
    strAddCharArray(&instrukce,"DEFVAR GF@$$var_4\n");
    strAddCharArray(&instrukce,"DEFVAR GF@$$result\n");
    strAddCharArray(&instrukce,"JUMP $$main\n");

    //generovani vestavenych funkci
    generate_substr();
    generate_ord();
    generate_length();
    generate_inputf();
    generate_inputi();
    generate_inputs();
    generate_print();
    generate_chr();
}

//uvolneni alokovane pameti
void generate_free_memory(){
    delete_string(&instrukce);
}

//generovani labelu pro hlavni telo a vytvoreni LF pro ne
void generate_main(){
    strAddCharArray(&instrukce,"\n# Main\n");
    strAddCharArray(&instrukce,"LABEL $$main\n");
    strAddCharArray(&instrukce,"CREATEFRAME\n");
    strAddCharArray(&instrukce,"PUSHFRAME\n");
}

//konec hlavniho tela programu
void generate_main_end(){
    strAddCharArray(&instrukce,"\n#Main end\n");
    strAddCharArray(&instrukce,"POPFRAME\n");
    strAddCharArray(&instrukce,"CLEARS\n");
}

//Otestuje promnenou (prvni na zasobniku) jestli je float nebo int, jinak ukonci interpret s chybou 4, napr. ( 5.0 + a )
void generate_compare_variable_1_with_float(){
   
    char str[MAX_INSTRUCTION_LEN];
    int current_label_number = get_new_label_number();

    strAddCharArray(&instrukce,"# Compare variable (first in stack) with float\n");

    strAddCharArray(&instrukce,"POPS GF@$$var_1\n");

    strAddCharArray(&instrukce,"TYPE GF@$$var_3 GF@$$var_1\n");

    sprintf(str, "JUMPIFEQ $IF_$%d_$TRUE_$INT GF@$$var_3 string@int\n",current_label_number);
    strAddCharArray(&instrukce,str);
    sprintf(str, "JUMPIFEQ $IF_$%d_$TRUE_$FLOAT GF@$$var_3 string@float\n",current_label_number);
    strAddCharArray(&instrukce,str);

    strAddCharArray(&instrukce,"EXIT int@4\n");

    sprintf(str, "LABEL $IF_$%d_$TRUE_$INT\n",current_label_number);
    strAddCharArray(&instrukce,str);
    strAddCharArray(&instrukce,"PUSHS GF@$$var_1\n");
    generate_stack_1_to_float();
    sprintf(str, "JUMP $IF_$%d_$TRUE_$INT_$NEXT\n",current_label_number);
    strAddCharArray(&instrukce,str);

    sprintf(str, "LABEL $IF_$%d_$TRUE_$FLOAT\n",current_label_number);
    strAddCharArray(&instrukce,str);
    strAddCharArray(&instrukce,"PUSHS GF@$$var_1\n");

    sprintf(str, "LABEL $IF_$%d_$TRUE_$INT_$NEXT\n",current_label_number);
    strAddCharArray(&instrukce,str);
}

//Otestuje promnenou (prvni na zasobniku) jestli je float nebo int, pokud je float tak zmeni konstantu jinak ukonci interpretr chybou 4, napr. ( 5 + a )
void generate_compare_variable_1_with_int(){
    
    char str[MAX_INSTRUCTION_LEN];
    int current_label_number = get_new_label_number();

    strAddCharArray(&instrukce,"# Compare variable (first in stack) with int\n");

    strAddCharArray(&instrukce,"POPS GF@$$var_1\n");

    strAddCharArray(&instrukce,"TYPE GF@$$var_3  GF@$$var_1\n");

    sprintf(str, "JUMPIFEQ $IF_$%d_$TRUE_$INT GF@$$var_3 string@int\n",current_label_number);
    strAddCharArray(&instrukce,str);
    sprintf(str, "JUMPIFEQ $IF_$%d_$TRUE_$FLOAT  GF@$$var_3 string@float\n",current_label_number);
    strAddCharArray(&instrukce,str);

    strAddCharArray(&instrukce,"EXIT int@4\n");

    sprintf(str, "LABEL $IF_$%d_$TRUE_$INT\n",current_label_number);
    strAddCharArray(&instrukce,str);
    strAddCharArray(&instrukce,"PUSHS GF@$$var_1\n");
    sprintf(str, "JUMP $IF_$%d_$TRUE_$INT_$NEXT\n",current_label_number);
    strAddCharArray(&instrukce,str);

    sprintf(str, "LABEL $IF_$%d_$TRUE_$FLOAT\n",current_label_number);
    strAddCharArray(&instrukce,str);
    generate_stack_1_to_float();
    strAddCharArray(&instrukce,"PUSHS GF@$$var_1\n");

    sprintf(str, "LABEL $IF_$%d_$TRUE_$INT_$NEXT\n",current_label_number);
    strAddCharArray(&instrukce,str);
}

//Otestuje promnenou (prvni na zasobniku) jestli je string, pokud neni ukonci interpretr chybou 4, napr. ( "ahoj" + a)
void generate_compare_variable_1_with_string(){

    char str[MAX_INSTRUCTION_LEN];
    int current_label_number = get_new_label_number();

    strAddCharArray(&instrukce,"# Compare variable (first in stack) with string\n");

    strAddCharArray(&instrukce,"POPS GF@$$var_1\n");

    strAddCharArray(&instrukce,"TYPE GF@$$var_3 GF@$$var_1\n");

    sprintf(str, "JUMPIFEQ $IF_$%d_$TRUE_$STRING GF@$$var_3 string@string\n",current_label_number);
    strAddCharArray(&instrukce,str);

    strAddCharArray(&instrukce,"EXIT int@4\n");

    sprintf(str, "LABEL $IF_$%d_$TRUE_$STRING\n",current_label_number);
    strAddCharArray(&instrukce,str);
    strAddCharArray(&instrukce,"PUSHS GF@$$var_1\n");
}

//Otestuje promnenou (druhou na zasobniku) jestli je float nebo int, jinak ukonci interpretr chybou 4, napr. ( a + 5.0 )
void generate_compare_variable_2_with_float(){
   
    char str[MAX_INSTRUCTION_LEN];
    int current_label_number = get_new_label_number();

    strAddCharArray(&instrukce,"# Compare variable (second in stack) with float\n");

    strAddCharArray(&instrukce,"POPS GF@$$var_1\n");
    strAddCharArray(&instrukce,"POPS GF@$$var_2\n");

    strAddCharArray(&instrukce,"TYPE GF@$$var_4 GF@$$var_2\n");

    sprintf(str, "JUMPIFEQ $IF_$%d_$TRUE_$INT GF@$$var_4 string@int\n",current_label_number);
    strAddCharArray(&instrukce,str);
    sprintf(str, "JUMPIFEQ $IF_$%d_$TRUE_$FLOAT GF@$$var_4 string@float\n",current_label_number);
    strAddCharArray(&instrukce,str);

    strAddCharArray(&instrukce,"EXIT int@4\n");

    sprintf(str, "LABEL $IF_$%d_$TRUE_$INT\n",current_label_number);
    strAddCharArray(&instrukce,str);
    strAddCharArray(&instrukce,"PUSHS GF@$$var_2\n");
    generate_stack_1_to_float();
    strAddCharArray(&instrukce,"PUSHS GF@$$var_1\n");
    sprintf(str, "JUMP $IF_$%d_$TRUE_$INT_$NEXT\n",current_label_number);
    strAddCharArray(&instrukce,str);

    sprintf(str, "LABEL $IF_$%d_$TRUE_$FLOAT\n",current_label_number);
    strAddCharArray(&instrukce,str);
    strAddCharArray(&instrukce,"PUSHS GF@$$var_2\n");
    strAddCharArray(&instrukce,"PUSHS GF@$$var_1\n");

    sprintf(str, "LABEL $IF_$%d_$TRUE_$INT_$NEXT\n",current_label_number);
    strAddCharArray(&instrukce,str);
}

//Otestuje promnenou (druhou na zasobniku) jestli je float nebo int, pokud je float tak zmeni konstantu jinak ukonci interpretr chybou 4, napr. ( a + 5 )
void generate_compare_variable_2_with_int(){
    
    char str[MAX_INSTRUCTION_LEN];
    int current_label_number = get_new_label_number();

    strAddCharArray(&instrukce,"# Compare variable (second in stack) with int\n");

    strAddCharArray(&instrukce,"POPS GF@$$var_1\n");
    strAddCharArray(&instrukce,"POPS GF@$$var_2\n");

    strAddCharArray(&instrukce,"TYPE GF@$$var_4 GF@$$var_2\n");

    sprintf(str, "JUMPIFEQ $IF_$%d_$TRUE_$INT GF@$$var_4 string@int\n",current_label_number);
    strAddCharArray(&instrukce,str);
    sprintf(str, "JUMPIFEQ $IF_$%d_$TRUE_$FLOAT GF@$$var_4 string@float\n",current_label_number);
    strAddCharArray(&instrukce,str);

    strAddCharArray(&instrukce,"EXIT int@4\n");

    sprintf(str, "LABEL $IF_$%d_$TRUE_$INT\n",current_label_number);
    strAddCharArray(&instrukce,str);
    strAddCharArray(&instrukce,"PUSHS GF@$$var_2\n");
    strAddCharArray(&instrukce,"PUSHS GF@$$var_1\n");
    sprintf(str, "JUMP $IF_$%d_$TRUE_$INT_$NEXT\n",current_label_number);
    strAddCharArray(&instrukce,str);

    sprintf(str, "LABEL $IF_$%d_$TRUE_$FLOAT\n",current_label_number);
    strAddCharArray(&instrukce,str);
    strAddCharArray(&instrukce,"PUSHS GF@$$var_2\n");
    strAddCharArray(&instrukce,"PUSHS GF@$$var_1\n");
    generate_stack_1_to_float();

    sprintf(str, "LABEL $IF_$%d_$TRUE_$INT_$NEXT\n",current_label_number);
    strAddCharArray(&instrukce,str);
}

//Otestuje promnenou (druhou na zasobniku) jestli je string, pokud neni ukonci interpretr chybou 4, napr. ( a + "ahoj" )
void generate_compare_variable_2_with_string(){

    char str[MAX_INSTRUCTION_LEN];
    int current_label_number = get_new_label_number();

    strAddCharArray(&instrukce,"# Compare variable (second in stack) with string\n");

    strAddCharArray(&instrukce,"POPS GF@$$var_1\n");
    strAddCharArray(&instrukce,"POPS GF@$$var_2\n");

    strAddCharArray(&instrukce,"TYPE GF@$$var_4 GF@$$var_2\n");

    sprintf(str, "JUMPIFEQ $IF_$%d_$TRUE_$STRING GF@$$var_4 string@string\n",current_label_number);
    strAddCharArray(&instrukce,str);

    strAddCharArray(&instrukce,"EXIT int@4\n");

    sprintf(str, "LABEL $IF_$%d_$TRUE_$STRING\n",current_label_number);
    strAddCharArray(&instrukce,str);
    strAddCharArray(&instrukce,"PUSHS GF@$$var_2\n");
    strAddCharArray(&instrukce,"PUSHS GF@$$var_1\n");
}

/**
 * Otestuje typy promnenych ( a + b )
 */
int generate_compare_variable_with_variable(int operator_type){

    char str[MAX_INSTRUCTION_LEN];
    int current_label_number = get_new_label_number();

    strAddCharArray(&instrukce,"# Compare variable with variable\n");

    strAddCharArray(&instrukce,"POPS GF@$$var_1\n");
    strAddCharArray(&instrukce,"POPS GF@$$var_2\n");

    strAddCharArray(&instrukce,"TYPE GF@$$var_4 GF@$$var_2\n");

    sprintf(str, "JUMPIFEQ $IF_$VARIABLE_$%d$TRUE_$INT GF@$$var_4 string@int\n",current_label_number);
    strAddCharArray(&instrukce,str);
    sprintf(str, "JUMPIFEQ $IF_$VARIABLE_$%d$TRUE_$FLOAT GF@$$var_4 string@float\n",current_label_number);
    strAddCharArray(&instrukce,str);
    sprintf(str, "JUMPIFEQ $IF_$VARIABLE_$%d$TRUE_$STRING GF@$$var_4 string@string\n",current_label_number);
    strAddCharArray(&instrukce,str);

    strAddCharArray(&instrukce,"EXIT int@4\n");

    sprintf(str, "LABEL $IF_$VARIABLE_$%d$TRUE_$INT\n",current_label_number);
    strAddCharArray(&instrukce,str);
    strAddCharArray(&instrukce,"PUSHS GF@$$var_2\n");
    strAddCharArray(&instrukce,"PUSHS GF@$$var_1\n");
    generate_compare_variable_1_with_int();
    sprintf(str, "JUMP $IF_$VARIABLE_$%d$TRUE_$NEXT\n",current_label_number);
    strAddCharArray(&instrukce,str);

    sprintf(str, "LABEL $IF_$VARIABLE_$%d$TRUE_$FLOAT\n",current_label_number);
    strAddCharArray(&instrukce,str);
    strAddCharArray(&instrukce,"PUSHS GF@$$var_2\n");
    strAddCharArray(&instrukce,"PUSHS GF@$$var_1\n");
    generate_compare_variable_1_with_float();
    sprintf(str, "JUMP $IF_$VARIABLE_$%d$TRUE_$NEXT\n",current_label_number);
    strAddCharArray(&instrukce,str);

   switch(operator_type){
        case G_TYPE_MUL:
        case G_TYPE_IDIV:
        case G_TYPE_MINUS:       
        case G_TYPE_LESS:
        case G_TYPE_LESS_OR_EQUAL:
        case G_TYPE_GREATER:
        case G_TYPE_GREATER_OR_EQUAL:

            sprintf(str, "LABEL $IF_$VARIABLE_$%d$TRUE_$STRING\n",current_label_number);
            strAddCharArray(&instrukce,str);

            strAddCharArray(&instrukce,"EXIT int@4\n");

            sprintf(str, "LABEL $IF_$VARIABLE_$%d$TRUE_$NEXT\n",current_label_number);
            strAddCharArray(&instrukce,str);
            break;

        case G_TYPE_PLUS:

            sprintf(str, "LABEL $IF_$VARIABLE_$%d$TRUE_$STRING\n",current_label_number);
            strAddCharArray(&instrukce,str);
            strAddCharArray(&instrukce,"PUSHS GF@$$var_2\n");
            strAddCharArray(&instrukce,"PUSHS GF@$$var_1\n");
            generate_compare_variable_1_with_string();

            generate_concat();

            sprintf(str, "JUMP $IF_$STRING_$%d$CONCAT\n",current_label_number);
            strAddCharArray(&instrukce,str);

            sprintf(str, "LABEL $IF_$VARIABLE_$%d$TRUE_$NEXT\n",current_label_number);
            strAddCharArray(&instrukce,str);
            return current_label_number;
            break;

        case G_TYPE_EQUAL:
        case G_TYPE_NOT_EQUAL: 

            sprintf(str, "LABEL $IF_$VARIABLE_$%d$TRUE_$STRING\n",current_label_number);
            strAddCharArray(&instrukce,str);
            strAddCharArray(&instrukce,"PUSHS GF@$$var_2\n");
            strAddCharArray(&instrukce,"PUSHS GF@$$var_1\n");

            sprintf(str, "LABEL $IF_$VARIABLE_$%d$TRUE_$NEXT\n",current_label_number);
            strAddCharArray(&instrukce,str);
            break;
    }

    return 0;
}

//Převede prvni hodnotu na zasobniku na float
void generate_stack_1_to_float(){
    strAddCharArray(&instrukce,"INT2FLOATS\n");
}

//Převede prvni hodnotu na zasobniku na int
void generate_stack_1_to_int(){
    strAddCharArray(&instrukce,"FLOAT2INTS\n");
}

//Převede druhou hodnotu na zasobniku na float
void generate_stack_2_to_float(){
    strAddCharArray(&instrukce,"POPS GF@$$var_4\n");
    strAddCharArray(&instrukce,"INT2FLOATS\n");
    strAddCharArray(&instrukce,"PUSHS GF@$$var_4\n");
}

//Převede druhou hodnotu na zasobniku na int
void generate_stack_2_to_int(){
    strAddCharArray(&instrukce,"POPS GF@$$var_4\n");
    strAddCharArray(&instrukce,"FLOAT2INTS\n");
    strAddCharArray(&instrukce,"PUSHS GF@$$var_4\n");
}

//Provede konkatenaci 2 stringy na vrcholu zasobniku, vysledek da na zasobnik
void generate_concat(){
    strAddCharArray(&instrukce,"POPS GF@$$var_4\n");
    strAddCharArray(&instrukce,"POPS GF@$$var_3\n");
    strAddCharArray(&instrukce,"CONCAT GF@$$var_1 GF@$$var_3 GF@$$var_4\n");
    strAddCharArray(&instrukce,"PUSHS GF@$$var_1\n");
}

//Hodnota z vrcholu zasobniku se ulozi do promenne
void generate_pop_to_variable(char* name){
    char str[MAX_INSTRUCTION_LEN];
    if (strcmp(name, "$$result") == 0){
        sprintf(str, "POPS GF@$$result\n");    
    }else{
        sprintf(str, "POPS LF@%s\n", name);
    }
    strAddCharArray(&instrukce,str);
}

//Push hodnoty na zásobník
void generate_push(int type, char* name) {
    char str[MAX_INSTRUCTION_LEN];
    strAddCharArray(&instrukce,"# Push\n");
    double a = 0;
    string s;
    switch(type){
        case INTEGER_TYPE:
            sprintf(str, "PUSHS int@%d\n",string_To_Int(name));
            strAddCharArray(&instrukce,str);
            break;
        case DOUBLE_TYPE:
            a = string_to_Double(name);
            sprintf(str, "PUSHS float@%a\n",a);
            strAddCharArray(&instrukce,str);
            break;
        case STRING_TYPE:
            strInit(&s);
            for (int i = 0; i < (int)strlen(name); ++i) {
                char c = name[i];
                if(c == '\"' && i == 0){

                }else if(c == '\"' && i == (int)strlen(name)-1){

                }else if(c == '#'){
                    strAddCharArray(&s,"\\035");
                } else if (c == '\\'){
                    i++;
                    c = name[i];
                    if(c == 'n'){
                        strAddCharArray(&s,"\\010");
                    } else if(c == 't'){
                        strAddCharArray(&s,"\\009");
                    } else if(c == 's'){
                        strAddCharArray(&s,"\\032");
                    } else if(c == '"'){
                        strAddCharArray(&s,"\"");
                    } else if(c == '\\'){
                        strAddCharArray(&s,"\\092");
                    }
                } else if(c <= 30){
                    string tmp;
                    strInit(&tmp);
                    int g = (int) c;
                    sprintf(tmp.str, "\\%03d", g);
                    strAddCharArray(&s,tmp.str);
                    strFree(&tmp);
                } else{
                    strAddChar(&s,c);
                }
            }
            sprintf(str, "PUSHS string@%s\n",s.str);
            strAddCharArray(&instrukce,str);
            free(s.str);
            break;
        case IDENTIFIER:
            sprintf(str, "PUSHS LF@%s\n",name);
            strAddCharArray(&instrukce,str);
            break;
        default:
            break;
    }
}

/**
 * Matematické operace na zásobníku
 * @param type - typ operace (G_TYPE_PLUS,G_TYPE_DIV,G_TYPE_MINUS,G_TYPE_MUL)
 */
void generate_mathemeatical_operations(int type , int label_number){
    int current_label_number;
    if (label_number == 0)
        current_label_number = get_new_label_number();
    else 
        current_label_number = label_number;

    char str[MAX_INSTRUCTION_LEN];
    strAddCharArray(&instrukce,"# Mathematical operations\n");
    switch (type){
        case G_TYPE_PLUS:
            strAddCharArray(&instrukce,"ADDS\n");
            sprintf(str, "LABEL $IF_$STRING_$%d$CONCAT\n",current_label_number);
            strAddCharArray(&instrukce,str);
            break;

        case G_TYPE_DIV:
            strAddCharArray(&instrukce,"POPS GF@$$var_1\n");
            
            sprintf(str, "JUMPIFNEQ $IF_$%d_$FLOAT_$NOT_$ZERO GF@$$var_1 float@0x0p+0\n", current_label_number);
            strAddCharArray(&instrukce,str);
            
            strAddCharArray(&instrukce,"EXIT int@9\n");

            sprintf(str, "LABEL $IF_$%d_$FLOAT_$NOT_$ZERO\n",current_label_number);
            strAddCharArray(&instrukce,str);
            strAddCharArray(&instrukce,"PUSHS GF@$$var_1\n");
            strAddCharArray(&instrukce,"DIVS\n");
            break;

        case G_TYPE_IDIV:
            strAddCharArray(&instrukce,"POPS GF@$$var_1\n");
            
            sprintf(str, "JUMPIFNEQ $IF_$%d_$INT_$NOT_$ZERO GF@$$var_1 int@0\n", current_label_number);
            strAddCharArray(&instrukce,str);
            
            strAddCharArray(&instrukce,"EXIT int@9\n");

            sprintf(str, "LABEL $IF_$%d_$INT_$NOT_$ZERO\n",current_label_number);
            strAddCharArray(&instrukce,str);
            strAddCharArray(&instrukce,"PUSHS GF@$$var_1\n");
            strAddCharArray(&instrukce,"IDIVS\n");
            break;

        case G_TYPE_MINUS:
            strAddCharArray(&instrukce,"SUBS\n");
            break;

        case G_TYPE_MUL:
            strAddCharArray(&instrukce,"MULS\n");
            break;
            
        default:
            break;
    }
}


//Operace na porovnavani hodnot na zasobniku
void generate_comparative_operations(int type){
    char str[MAX_INSTRUCTION_LEN];
    strAddCharArray(&instrukce,"# Comparative operations\n");
    switch (type){
        case G_TYPE_LESS:
            strAddCharArray(&instrukce,"LTS\n");
            break;
        case G_TYPE_LESS_OR_EQUAL:

            strAddCharArray(&instrukce,"POPS GF@$$var_3\n");
            strAddCharArray(&instrukce,"POPS GF@$$var_4\n");

            strAddCharArray(&instrukce,"PUSHS GF@$$var_4\n");
            strAddCharArray(&instrukce,"PUSHS GF@$$var_3\n");

            strAddCharArray(&instrukce,"LTS\n");
            strAddCharArray(&instrukce,"POPS GF@$$var_1\n");

            strAddCharArray(&instrukce,"PUSHS GF@$$var_4\n");
            strAddCharArray(&instrukce,"PUSHS GF@$$var_3\n");

            strAddCharArray(&instrukce,"EQS\n");
            strAddCharArray(&instrukce,"POPS GF@$$var_2\n");

            int current_label_number = get_new_label_number();

            sprintf(str, "JUMPIFEQ %%L_NUM_%d_YES GF@$$var_1 bool@true\n", current_label_number);
            strAddCharArray(&instrukce,str);
            sprintf(str, "JUMPIFEQ %%L_NUM_%d_YES GF@$$var_2 bool@true\n", current_label_number);
            strAddCharArray(&instrukce,str);
            sprintf(str, "JUMP %%L_NUM_%d_NOT\n", current_label_number);
            strAddCharArray(&instrukce,str);
            //false
            sprintf(str, "LABEL %%L_NUM_%d_NOT\n", current_label_number);
            strAddCharArray(&instrukce,str);
            strAddCharArray(&instrukce,"PUSHS bool@false\n");
            sprintf(str, "JUMP %%L_NUM_%d\n", current_label_number);
            strAddCharArray(&instrukce,str);
            //true
            sprintf(str, "LABEL %%L_NUM_%d_YES\n", current_label_number);
            strAddCharArray(&instrukce,str);
            strAddCharArray(&instrukce,"PUSHS bool@true\n");
            sprintf(str, "JUMP %%L_NUM_%d\n", current_label_number);
            strAddCharArray(&instrukce,str);
            //end
            sprintf(str, "LABEL %%L_NUM_%d\n", current_label_number);
            strAddCharArray(&instrukce,str);

            break;
        case G_TYPE_GREATER:
            strAddCharArray(&instrukce,"GTS\n");
            break;
        case G_TYPE_GREATER_OR_EQUAL:
            strAddCharArray(&instrukce,"POPS GF@$$var_3\n");
            strAddCharArray(&instrukce,"POPS GF@$$var_4\n");

            strAddCharArray(&instrukce,"PUSHS GF@$$var_4\n");
            strAddCharArray(&instrukce,"PUSHS GF@$$var_3\n");

            strAddCharArray(&instrukce,"GTS\n");
            strAddCharArray(&instrukce,"POPS GF@$$var_1\n");

            strAddCharArray(&instrukce,"PUSHS GF@$$var_4\n");
            strAddCharArray(&instrukce,"PUSHS GF@$$var_3\n");

            strAddCharArray(&instrukce,"EQS\n");
            strAddCharArray(&instrukce,"POPS GF@$$var_2\n");

            int current_label_number_g = get_new_label_number();

            sprintf(str, "JUMPIFEQ %%L_NUM_%d_YES GF@$$var_1 bool@true\n", current_label_number_g);
            strAddCharArray(&instrukce,str);
            sprintf(str, "JUMPIFEQ %%L_NUM_%d_YES GF@$$var_2 bool@true\n", current_label_number_g);
            strAddCharArray(&instrukce,str);
            sprintf(str, "JUMP %%L_NUM_%d_NOT\n", current_label_number_g);
            strAddCharArray(&instrukce,str);
            //false
            sprintf(str, "LABEL %%L_NUM_%d_NOT\n", current_label_number_g);
            strAddCharArray(&instrukce,str);
            strAddCharArray(&instrukce,"PUSHS bool@false\n");
            sprintf(str, "JUMP %%L_NUM_%d\n", current_label_number_g);
            strAddCharArray(&instrukce,str);
            //true
            sprintf(str, "LABEL %%L_NUM_%d_YES\n", current_label_number_g);
            strAddCharArray(&instrukce,str);
            strAddCharArray(&instrukce,"PUSHS bool@true\n");
            sprintf(str, "JUMP %%L_NUM_%d\n", current_label_number_g);
            strAddCharArray(&instrukce,str);
            //end
            sprintf(str, "LABEL %%L_NUM_%d\n", current_label_number_g);
            strAddCharArray(&instrukce,str);
            break;
        case G_TYPE_EQUAL:
            strAddCharArray(&instrukce,"EQS\n");
            break;
        case G_TYPE_NOT_EQUAL:
            strAddCharArray(&instrukce,"EQS\n");
            strAddCharArray(&instrukce,"NOTS\n");
            break;
        default:
            break;
    }
}

//generovani zacatku if vetvi
void generate_if(int num) {
    char str[MAX_INSTRUCTION_LEN];
    strAddCharArray(&instrukce,"#if\n");
    sprintf(str, "JUMPIFEQ ELSE_%d GF@$$result bool@false\n",num);
    strAddCharArray(&instrukce,str);
}

//generovani else vetvi
void generate_else(int num) {
    char str[MAX_INSTRUCTION_LEN];
    strAddCharArray(&instrukce,"#else \n");
    sprintf(str, "LABEL ELSE_%d\n",num);
    strAddCharArray(&instrukce,str);
}

void generate_if_else_end(int num) {
    char str[MAX_INSTRUCTION_LEN];
    strAddCharArray(&instrukce,"#if-else end \n");
    sprintf(str, "LABEL IF_ELSE_END_%d\n",num);
    strAddCharArray(&instrukce,str);
}

generate_jump_to_if_else_end(int num){
    char str[MAX_INSTRUCTION_LEN];
    sprintf(str, "JUMP IF_ELSE_END_%d\n",num);
    strAddCharArray(&instrukce,str);
}

//Zacatek WHILE (Generuje se před zpracovanim podminky)
void generate_while_condition_check(int num) {
    char str[MAX_INSTRUCTION_LEN];
    strAddCharArray(&instrukce,"# While condition\n");
    sprintf(str, "LABEL WHILE_%d_CONDITION\n",num);
    strAddCharArray(&instrukce,str);
}

//Zacatek těla funkce WHILE
void generate_while_start(int num) {
    char str[MAX_INSTRUCTION_LEN];
    strAddCharArray(&instrukce,"# While start\n");
    sprintf(str, "JUMPIFEQ WHILE_%d_END GF@$$result bool@false\n",num);
    strAddCharArray(&instrukce,str);
}

// Konec WHILE
void generate_while_end(int num){
    char str[MAX_INSTRUCTION_LEN];
    sprintf(str, "JUMP WHILE_%d_CONDITION\n",num);
    strAddCharArray(&instrukce,str);
    strAddCharArray(&instrukce,"# While end\n");
    sprintf(str,"LABEL WHILE_%d_END\n",num);
    strAddCharArray(&instrukce,str);
}

// Používá se před voláním funkce
// Vytvoří proměnou v TF a přidá do ní hodnotu, TF se pak pushne do funkci 
void generate_assign_arguments_to_function(int value_type, int num, char *value){
    char str[MAX_INSTRUCTION_LEN];
    string s;
    sprintf(str, "DEFVAR TF@V_%d\n",num);
    strAddCharArray(&instrukce,str);
    switch (value_type) {
        case INTEGER_TYPE:
            sprintf(str, "MOVE TF@V_%d int@%d\n", num, string_To_Int(value));
            strAddCharArray(&instrukce,str);
            break;
        case DOUBLE_TYPE:
            sprintf(str, "MOVE TF@V_%d float@%a\n", num, string_to_Double(value));
            strAddCharArray(&instrukce,str);
            break;
        case STRING_TYPE:
            strInit(&s);
            for (int i = 0; i < (int)strlen(value); ++i) {
                char c = value[i];
                if(c == '\"' && i == 0){

                }else if(c == '\"' && i == (int)strlen(value)-1){

                }else if(c == '#'){
                    strAddCharArray(&s,"\\035");
                } else if (c == '\\'){
                    i++;
                    c = value[i];
                    if(c == 'n'){
                        strAddCharArray(&s,"\\010");
                    } else if(c == 't'){
                        strAddCharArray(&s,"\\009");
                    } else if(c == 's'){
                        strAddCharArray(&s,"\\032");
                    } else if(c == '"'){
                        strAddCharArray(&s,"\"");
                    } else if(c == '\\'){
                        strAddCharArray(&s,"\\092");
                    }
                } else if(c <= 30){
                    string tmp;
                    strInit(&tmp);
                    int a = (int) c;
                    sprintf(tmp.str, "\\%03d", a);
                    strAddCharArray(&s,tmp.str);
                    strFree(&tmp);
                } else{
                    strAddChar(&s,c);
                }
            }
            sprintf(str, "MOVE TF@V_%d string@%s\n", num, s.str);
            strAddCharArray(&instrukce,str);
            free(s.str);
            break;
        case IDENTIFIER:
            sprintf(str, "MOVE TF@V_%d LF@%s\n", num, value);
            strAddCharArray(&instrukce,str);
        default:
            break;
    }
}

//Používá se ve funkci k načtení parametrů
void generate_read_function_params(int num, char *name){
    char str[MAX_INSTRUCTION_LEN];
    sprintf(str, "DEFVAR LF@%s\n",name);
    strAddCharArray(&instrukce,str);
    sprintf(str, "MOVE LF@%s LF@V_%d\n",name,num);
    strAddCharArray(&instrukce,str);
}

//Volání funkce
void generate_function_call(char* name){
    char str[MAX_INSTRUCTION_LEN];
    sprintf(str, "CALL $$FUN_%s_START\n", name);
    strAddCharArray(&instrukce,str);
}

//Vytvoreni docasneho ramce pro predani parametru do funkce
void generate_TF_for_function_args(){
    strAddCharArray(&instrukce,"CREATEFRAME\n");
}

//Přiřadí návratovou hodnotu funkce do proměnné
void generate_retval_to_var(char *name){
    char str[MAX_INSTRUCTION_LEN];
    sprintf(str, "MOVE LF@%s TF@$$FUN_RET\n",name);
    strAddCharArray(&instrukce,str);
}

//Zacatek funkce
void generate_function_start(char *name){
    char str[MAX_INSTRUCTION_LEN];
    sprintf(str, "LABEL $$FUN_%s_START\n",name);
    strAddCharArray(&instrukce,str);
    strAddCharArray(&instrukce,"PUSHFRAME\n");
    variable_declare("$$FUN_RET");
}


//Return z funkce, vezme poslední hodnotu z zásobníku a uloží ji jako výsledek funkce
void generate_function_return(char *name){
    char str[MAX_INSTRUCTION_LEN];
    strAddCharArray(&instrukce,"POPS LF@$$FUN_RET\n");
    sprintf(str, "JUMP $$FUN_%s_END\n",name);
    strAddCharArray(&instrukce,str);
}

//Konec funkce 
void generate_function_end(char *name){
    char str[MAX_INSTRUCTION_LEN];
    sprintf(str, "LABEL $$FUN_%s_END\n",name);
    strAddCharArray(&instrukce,str);
    strAddCharArray(&instrukce,"POPFRAME\n");
    strAddCharArray(&instrukce,"RETURN\n");
}