#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generator.h"
#include "tokens.h"
#include "scaner.h"
#include "string.h"
#include "symtable.h"


char *get_new_label() {
    char *reg = (char *) malloc(30 * sizeof(char));
    sprintf(reg, "%%L_NUM_%d", label_number++);
    return reg; //todo malloc
}

char *get_frame() {
    char *frameTP = (char *) malloc(30 * sizeof(char));
    if (inScope) {
        strcpy(frameTP, "LF");
    } else {
        strcpy(frameTP, "GF");
    }
    return frameTP;
}

char *get_type(int type) {
    char *t = (char *) malloc(10 * sizeof(char));
    switch (type) {
        case INTEGER_TYPE:
            strcpy(t, "int");
            break;
        case DOUBLE_TYPE:
            strcpy(t, "float");
            break;
        case STRING_TYPE:
            strcpy(t, "string");
            break;
        default:
            break;
    }
    return t;
}

/**
 * Deklarace proměné
 * @param type - typ proměné (INTEGER_TYPE,DOUBLE_TYPE,STRING_TYPE)
 * @param name - jmeno promene
 */
void variable_declare(int type, char *name) {
    printf("# Variable declare\n");
    printf("DEFVAR %s@%s\n", get_frame(), name);
    double a = 0;
    switch (type) {
        case INTEGER_TYPE:
            printf("MOVE %s@%s int@0\n", get_frame(), name);
            break;
        case DOUBLE_TYPE:
            printf("MOVE %s@%s float@%a\n", get_frame(), name, a);
            break;
        case STRING_TYPE:
            printf("MOVE %s@%s string@\n", get_frame(), name);
            break;
    }
}

//todo Neni funkcni
void generate_inputs() {
    printf("LABEL %%INPUTS\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@$$result_inputs\n");
    printf("MOVE LF@$$result_inputs string@\n");


    printf("READ LF@$$result_inputs string\n");

    printf("POPFRAME\n");
    printf("RETURN\n");
}

/**
 * Zacatek programu
 */
void generate_start(){
    label_number = 0;
    if_num = 0;
    if_else_num = 0;
    if_end_num = 0;
    inScope = false;

    printf(".IFJcode18\n");

    printf("DEFVAR GF@$$input\n");
    //printf("MOVE GF@$$input string@?\\032\n");

    printf("DEFVAR GF@$$var_1\n");
    printf("DEFVAR GF@$$var_2\n");
    printf("DEFVAR GF@$$var_3\n");
    printf("DEFVAR GF@$$var_4\n");

    printf("DEFVAR GF@$$result\n");

    printf("JUMP $$main\n");
}

/**
 * LAbel main
 */
void generate_main(){
    printf("\n# Main\n");

    printf("LABEL $$main\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
}

/**
 * Konec main
 */
void generate_main_end(){
    printf("\n#Main end\n");

    printf("POPFRAME\n");
    printf("CLEARS\n");
}

/**
 * Zkontroluje že vrchní dvě hodnoty na zásobníku jsou stejného typu,pokud ne převede je na float
 */
void data_conversion() {
    printf("# Data Conversion\n");
    printf("POPS GF@$$var_3\n");
    printf("POPS GF@$$var_4\n");

    printf("TYPE GF@$$var_1 GF@$$var_3\n");
    printf("TYPE GF@$$var_2 GF@$$var_4\n");

    char *label = get_new_label();

    printf("JUMPIFEQ %s GF@$$var_1 GF@$$var_2\n", label);
    printf("JUMPIFEQ %sV1_NOT GF@$$var_1 string@float\n", label);

    printf("INT2FLOAT GF@$$var_3 GF@$$var_3\n");

    printf("LABEL %sV1_NOT\n", label);
    printf("JUMPIFEQ %s GF@$$var_2 string@float\n", label);
    printf("INT2FLOAT GF@$$var_4 GF@$$var_4\n");

    printf("LABEL %s\n", label);
    printf("PUSHS GF@$$var_4\n");
    printf("PUSHS GF@$$var_3\n");
}

/**
 * Převede vrchní dvě hodnoty na zásobníku na float
 */
void data_conversion_to_float() {
    printf("# Data Conversion to float\n");
    printf("POPS GF@$$var_3\n");
    printf("POPS GF@$$var_4\n");

    printf("TYPE GF@$$var_1 GF@$$var_3\n");
    printf("TYPE GF@$$var_2 GF@$$var_4\n");

    char *label = get_new_label();

    printf("JUMPIFNEQ %sV1_NOT GF@$$var_1 string@float\n", label);
    printf("JUMPIFNEQ %sV2_NOT GF@$$var_1 string@float\n", label);
    printf("JUMPIFEQ %s GF@$$var_1 GF@$$var_2\n", label);


    printf("LABEL %sV1_NOT\n", label);
    printf("INT2FLOAT GF@$$var_3 GF@$$var_3\n");
    printf("JUMPIFNEQ %sV2_NOT GF@$$var_1 string@float\n", label);
    printf("JUMPIFEQ %s GF@$$var_1 GF@$$var_2\n", label);

    printf("LABEL %sV2_NOT\n", label);
    printf("INT2FLOAT GF@$$var_4 GF@$$var_4\n");
    printf("JUMP %s\n", label);

    printf("LABEL %s\n", label);
    printf("PUSHS GF@$$var_4\n");
    printf("PUSHS GF@$$var_3\n");
}


/**
 * Převede vrchní dvě hodnoty na zásobníku na int
 */
void data_conversion_to_int() {
    printf("# Data Conversion to float\n");
    printf("POPS GF@$$var_3\n");
    printf("POPS GF@$$var_4\n");

    printf("TYPE GF@$$var_1 GF@$$var_3\n");
    printf("TYPE GF@$$var_2 GF@$$var_4\n");

    char *label = get_new_label();

    printf("JUMPIFNEQ %sV1_NOT GF@$$var_1 string@int\n", label);
    printf("JUMPIFNEQ %sV2_NOT GF@$$var_1 string@int\n", label);
    printf("JUMPIFEQ %s GF@$$var_1 GF@$$var_2\n", label);


    printf("LABEL %sV1_NOT\n", label);
    printf("FLOAT2INT GF@$$var_3 GF@$$var_3\n");
    printf("JUMPIFNEQ %sV2_NOT GF@$$var_1 string@int\n", label);
    printf("JUMPIFEQ %s GF@$$var_1 GF@$$var_2\n", label);

    printf("LABEL %sV2_NOT\n", label);
    printf("FLOAT2INT GF@$$var_4 GF@$$var_4\n");
    printf("JUMP %s\n", label);

    printf("LABEL %s\n", label);
    printf("PUSHS GF@$$var_4\n");
    printf("PUSHS GF@$$var_3\n");
}

/**
 * Převede prvni hodnotu na zasobniku na float
 */
void generate_stack_1_to_float(){
    printf("INT2FLOATS\n");
}

/**
 * Převede prvni hodnotu na zasobniku na int
 */
void generate_stack_1_to_int(){
    printf("FLOAT2INTS\n");
}

/**
 * Převede druhou hodnotu na zasobniku na float
 */
void generate_stack_2_to_float(){
    printf("POPS GF@$$var_4\n");
    printf("INT2FLOATS\n");
    printf("PUSHS GF@$$var_4\n");
}

/**
 * Převede druhou hodnotu na zasobniku na int
 */
void generate_stack_2_to_int(){
    printf("POPS GF@$$var_4\n");
    printf("FLOAT2INTS\n");
    printf("PUSHS GF@$$var_4\n");
}

/**
 * Přiřadí hodnotu proměnné
 * @param expresion_type - typ přiřazení (INT_E,DOUBLE_E,STRING_E,VARIABLE_E)
 * @param variable_name - jmeno proměnné
 * @param variable_value - hodnota proměnné (pokud jde o VARIABLE_E tak je zde jmeno druhé proměnné)
 */
void generate_variable_assign(int expresion_type,char* variable_name, char* variable_value) {
    printf("# Variable assign\n");
    string s;
    switch (expresion_type) {
        case INT_E: {
            printf("MOVE %s@%s int@%d\n", get_frame(), variable_name, string_To_Int(variable_value)); //todo %d
            break;
        }
        case DOUBLE_E: {
            printf("MOVE %s@%s float@%a\n", get_frame(), variable_name, string_to_Double(variable_value));
            break;
        }
        case STRING_E:
            strInit(&s);
            for (int i = 0; i < strlen(variable_value); ++i) {
                char c = variable_value[i];
                if(c == '#'){
                    strAddCharArray(&s,"\\035");
                } else if (c == '\\'){
                    strAddCharArray(&s,"\\092");
                } else if(c <= 32){
                    char tmp[3];
                    sprintf(tmp, "\\%03d", c);
                    strAddCharArray(&s,tmp);
                } else{
                    strAddChar(&s,c);
                }
            }
            printf("MOVE %s@%s string@%s\n", get_frame(), variable_name, s.str);
            free(s.str);
            break;
        case VARIABLE_E:
            printf("MOVE %s@%s %s@%s\n", get_frame(), variable_name, get_frame(),variable_value);
            break;

        default:
            break;
    }

}

/**
 * Spoji 2 stringy na vrcholu zasobniku, vysledek da na zasobnik
 */
void generate_concat(){
    printf("POPS GF@$$var_4\n");
    printf("POPS GF@$$var_3\n");
    printf("CONCAT GF@$$var_1 GF@$$var_3 GF@$$var_4\n");
    printf("PUSHS GF@$$var_1\n");
}

/**
 * Vezme hodnotu z vrcholu zasobniku a ulozi ji do promene
 * @param name - nazev promene
 */
void generate_pop_to_variable(char* name){
    printf("POPS %s@%s\n",get_frame(),name);
}

/**
 * Pushne hodnotu na zásobník
 * @param type - typ hodnoty (INTEGER_TYPE,DOUBLE_TYPE,STRING_TYPE,IDENTIFIER)
 * @param name - hodnota (v případě identifikátoru je zde jeho název)
 */
void generate_push(int type, char* name) {
    printf("# Push\n");
    double a = 0;
    string s;
    switch(type){
        case INTEGER_TYPE:
            printf("PUSHS int@%d\n",string_To_Int(name));
            break;
        case DOUBLE_TYPE:
            a = string_to_Double(name);
            printf("PUSHS float@%a\n",a);
            break;
        case STRING_TYPE:
            strInit(&s);
            for (int i = 0; i < strlen(name); ++i) {
                char c = name[i];
                if(c == '#'){
                    strAddCharArray(&s,"\\035");
                } else if (c == '\\'){
                    strAddCharArray(&s,"\\092");
                } else if(c <= 32){
                    char tmp[3];
                    sprintf(tmp, "\\%03d", c);
                    strAddCharArray(&s,tmp);
                } else{
                    strAddChar(&s,c);
                }
            }
            printf("PUSHS string@%s\n",s.str); //todo
            free(s.str);
            break;
        case IDENTIFIER:
            printf("PUSHS %s@%s\n",get_frame(),name);
            break;
        default:
            //todo
            break;
    }
}

/**
 * Matematické operace na zásobníku
 * @param type - typ operace (G_TYPE_PLUS,G_TYPE_DIV,G_TYPE_MINUS,G_TYPE_MUL)
 */
void generate_mathemeatical_operations(int type){
    data_conversion();
    printf("# Mathematical operations\n");
    switch (type){
        case G_TYPE_PLUS:
            printf("ADDS\n");
            break;
        case G_TYPE_DIV:
            data_conversion_to_float();
            printf("DIVS\n");
            break;
        case G_TYPE_IDIV:
            data_conversion_to_int();
            printf("IDIVS\n");
            break;
        case G_TYPE_MINUS:
            printf("SUBS\n");
            break;
        case G_TYPE_MUL:
            printf("MULS\n");
            break;
        default:
            //todo
            break;
    }
}

/**
 * Operace na porovnavani hodnot na zasobniku
 * @param type - typ operace (G_TYPE_LESS,G_TYPE_LESS_OR_EQUAL,G_TYPE_GREATER,
 *                            G_TYPE_GREATER_OR_EQUAL,G_TYPE_NOT_EQUAL,G_TYPE_EQUAL)
 */
void generate_comparative_operations(int type){
    data_conversion();
    printf("# Comparative operations\n");
    switch (type){
        case G_TYPE_LESS:
            printf("LTS\n");
            break;
        case G_TYPE_LESS_OR_EQUAL:

            printf("POPS GF@$$var_3\n");
            printf("POPS GF@$$var_4\n");

            printf("PUSHS GF@$$var_4\n");
            printf("PUSHS GF@$$var_3\n");

            printf("LTS\n");
            printf("POPS GF@$$var_1\n");

            printf("PUSHS GF@$$var_4\n");
            printf("PUSHS GF@$$var_3\n");

            printf("EQS\n");
            printf("POPS GF@$$var_2\n");

            char *label = get_new_label();

            printf("JUMPIFEQ %s_YES GF@$$var_1 bool@true\n", label);
            printf("JUMPIFEQ %s_YES GF@$$var_2 bool@true\n", label);
            printf("JUMP %s_NOT\n", label);
            //false
            printf("LABEL %s_NOT\n", label);
            printf("PUSHS bool@false\n");
            printf("JUMP %s\n", label);
            //true
            printf("LABEL %s_YES\n", label);
            printf("PUSHS bool@true\n");
            printf("JUMP %s\n", label);
            //end
            printf("LABEL %s\n", label);

            break;
        case G_TYPE_GREATER:
            printf("GTS\n");
            break;
        case G_TYPE_GREATER_OR_EQUAL:
            printf("POPS GF@$$var_3\n");
            printf("POPS GF@$$var_4\n");

            printf("PUSHS GF@$$var_4\n");
            printf("PUSHS GF@$$var_3\n");

            printf("GTS\n");
            printf("POPS GF@$$var_1\n");

            printf("PUSHS GF@$$var_4\n");
            printf("PUSHS GF@$$var_3\n");

            printf("EQS\n");
            printf("POPS GF@$$var_2\n");

            char *l = get_new_label();

            printf("JUMPIFEQ %s_YES GF@$$var_1 bool@true\n", l);
            printf("JUMPIFEQ %s_YES GF@$$var_2 bool@true\n", l);
            printf("JUMP %s_NOT\n", l);
            //false
            printf("LABEL %s_NOT\n", l);
            printf("PUSHS bool@false\n");
            printf("JUMP %s\n", l);
            //true
            printf("LABEL %s_YES\n", l);
            printf("PUSHS bool@true\n");
            printf("JUMP %s\n", l);
            //end
            printf("LABEL %s\n", l);
            break;
        case G_TYPE_EQUAL:
            printf("EQS\n");
            break;
        case G_TYPE_NOT_EQUAL:
            printf("EQS\n");
            printf("NOTS\n");
            break;
        default:
            //todo
            break;
    }
}

/**
 * Print hodnoty
 * @param type - typ hodnoty (INTEGER_TYPE,DOUBLE_TYPE,STRING_TYPE,IDENTIFIER)
 * @param name - hodnota (pokud jde o IDENTIFIER tak je zde jeho nazev)
 */
void generate_print(int type, char* name) {
    string s;
    switch (type) {
        case INTEGER_TYPE:
            printf("WRITE int@%d\n", string_To_Int(name));
            break;
        case DOUBLE_TYPE:
            printf("WRITE float@%a\n", string_to_Double(name));
            break;
        case STRING_TYPE:
            strInit(&s);
            for (int i = 0; i < strlen(name); ++i) {
                char c = name[i];
                if(c == '#'){
                    strAddCharArray(&s,"\\035");
                } else if (c == '\\'){
                    strAddCharArray(&s,"\\092");
                } else if(c <= 32){
                    char tmp[3];
                    sprintf(tmp, "\\%03d", c);
                    strAddCharArray(&s,tmp);
                } else{
                    strAddChar(&s,c);
                }
            }
            printf("WRITE string@%s\n", s.str);
            free(s.str);
            break;
        case IDENTIFIER:
            printf("WRITE %s@%s\n", get_frame(), name);
            break;
        default:
            break;
    }
}

/**
 * Uloží hodnotu ze zasobnikuna globalni promene result
 */
void generate_pop_to_result(){
    printf("# Pop \n");
    printf("POPS GF@$$result\n");
}

/**
 * Print hodnoty v globalni promene result
 */
void generate_print_result(){
    printf("WRITE GF@$$result\n");

}

/**
 * Zacatek IF (generuje se až po zpracovani vstupni podminky)
 * @param num - číslo IF
 */
void generate_if(int num) {
    printf("# If start \n");
    printf("JUMPIFEQ IF_%d_ELSE GF@$$result bool@false\n",num);
}

/**
 * Musi se vydy volat!! Kdyz neexistuje else vola se s false
 * @param num - číslo IF
 * @param else_statment - true pokud existuje else statment, false pokud ne
 */
void generate_else(int num, bool else_statment) {
    printf("# else \n");
    printf("JUMP IF_%d_END\n",num);

    printf("LABEL IF_%d_ELSE\n",num);
    if(else_statment == false){
        printf("JUMP IF_%d_END\n",num);
    }
}


/**
 * Komec IF
 * @param num - číslo IF
 */
void generate_if_else_end(int num) {
    printf("# End If\n");
    printf("LABEL IF_%d_END\n",num);
}


/**
 * Zacatek WHILE (Generuje se před zpracovanim podminky)
 * @param num - číslo WHILE
 */
void generate_while_condition_check(int num) {
    printf("# While condition\n");
    printf("LABEL WHILE_%d_CONDITION\n",num);
}

/**
 * Zacatek těla funkce WHILE
 * @param num - číslo WHILE
 */
void generate_while_start(int num) {
    printf("# While start\n");
    printf("JUMPIFEQ WHILE_%d_END GF@$$result bool@false\n",num);
}

/**
 * Konec WHILE
 * @param num - číslo WHILE
 */
void generate_while_end(int num){
    printf("JUMP WHILE_%d_CONDITION\n",num);
    printf("# While end\n");
    printf("LABEL WHILE_%d_END\n",num);
}
