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


/* ------------------------------------------------------------------------------------ */

void generate_start(){
    label_number = 0;
    if_num = 0;
    if_else_num = 0;
    if_end_num = 0;
    inScope = false;

    printf(".IFJcode18\n");

    //printf("DEFVAR GF@$$input_prompt\n");
    //printf("MOVE GF@$$input_prompt string@?\\032\n");

    printf("DEFVAR GF@$$var_1\n");
    printf("DEFVAR GF@$$var_2\n");
    printf("DEFVAR GF@$$var_3\n");
    printf("DEFVAR GF@$$var_4\n");

    printf("DEFVAR GF@$$result\n");

    printf("JUMP $$main\n");
}

void generate_main(){
    printf("\n# Main\n");

    printf("LABEL $$main\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
}

void generate_main_end(){
    printf("\n#Main end\n");

    printf("POPFRAME\n");
    printf("CLEARS\n");
}

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

char* change_string(char *str){
    string s;
    strInit(&s);
    for (int i = 0; i < strlen(str); ++i) {
        char c = str[i];
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
    char result[s.length];
    strcpy(result,s.str);
    free(s.str);
    //return s.str;
    //return result;
}




void generate_concat(){
    printf("POPS GF@$$var_4\n");
    printf("POPS GF@$$var_3\n");
    printf("CONCAT GF@$$var_1 GF@$$var_3 GF@$$var_4\n");
    printf("PUSHS GF@$$var_1\n");
}

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


void generate_print(int type, char* name) {
    switch (type) {
        case INTEGER_TYPE:
            printf("WRITE int@%d\n", string_To_Int(name));
            break;
        case DOUBLE_TYPE:
            printf("WRITE float@%a\n", string_to_Double(name));
            break;
        case STRING_TYPE:
            printf("WRITE string@%s\n", name);
            break;
        case IDENTIFIER:
            printf("WRITE %s@%s\n", get_frame(), name);
            break;
        default:
            break;
    }
}

void generate_pop_to_result(){
    printf("# Pop \n");
    printf("POPS GF@$$result\n");
}

void generate_print_result(){
    printf("WRITE GF@$$result\n");

}

void generate_if(int num) {
    printf("# If start \n");
    printf("JUMPIFEQ IF_%d_ELSE GF@$$result bool@false\n",num);
}

/**
 * Musi se vydy volat!! Kdyz neexistuje else vola se s false
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


void generate_if_else_end(int num) {
    printf("# End If\n");
    printf("LABEL IF_%d_END\n",num);
}

