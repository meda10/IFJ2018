//
// Created by petr on 18.11.18.
//

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
    sprintf(reg, "%%label_number_%d", label_number++);
    return reg; //todo malloc
}

char *get_frame() {
    char *frameTP = (char *) malloc(30 * sizeof(char));
    if (!inScope) {
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
    inScope = true;

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

void generateDataConversion() {
    printf("# Data Conversion\n");
    printf("POPS GF@$$var_3\n");
    printf("POPS GF@$$var_4\n");

    printf("TYPE GF@$$var_1 GF@$$var_3\n");
    printf("TYPE GF@$$var_2 GF@$$var_4\n");

    char *conversionLabel = get_new_label();

    printf("JUMPIFEQ %s GF@$$var_1 GF@$$var_2\n", conversionLabel);
    printf("JUMPIFEQ %sN GF@$$var_1 string@float\n", conversionLabel);

    printf("INT2FLOAT GF@$$var_3 GF@$$var_3\n");

    printf("LABEL %sN\n", conversionLabel);
    printf("JUMPIFEQ %s GF@$$var_2 string@float\n", conversionLabel);
    printf("INT2FLOAT GF@$$var_4 GF@$$var_4\n");

    printf("LABEL %s\n", conversionLabel);
    printf("PUSHS GF@$$var_4\n");
    printf("PUSHS GF@$$var_3\n");
}

void generate_variable_assign(int expresion_type,char* variable_name, char* variable_value) {
    printf("# Variable assign\n");
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
            printf("MOVE %s@%s string@%s\n", get_frame(), variable_name, variable_value);
            break;
        case VARIABLE_E:
            //if ((var1->data.type == INTEGER_TYPE && var2->data.type == DOUBLE_TYPE) || (var1->data.type == DOUBLE_TYPE && var2->data.type == INTEGER_TYPE)) {

            //todo konvert
            /*
            char *hReg = get_new_label();
            printf("DEFVAR %s@%s\n", frame, hReg);
            generateImplicitConversion(getWholeRegisterName(hReg, frame), getWholeRegisterName(expression->op.variableExp->data.name, getVarFrame()), expression->op.variableExp->data.type, var->data.type);
            printf("MOVE %s@%s %s@%s\n", get_frame(), var->data.name, frame, hReg);
            */
            //} else {
            printf("MOVE %s@%s %s@%s\n", get_frame(), variable_name, get_frame(),variable_value);
            //}
            break;

        default:
            break;
    }

}

void generate_push(int type, char* name) {
    printf("# Push\n");
    double a = 0;
    switch(type){
        case INTEGER_TYPE:
            printf("PUSHS int@%s\n", name);
            break;
        case DOUBLE_TYPE:
            a = string_to_Double(name);
            printf("PUSHS float@%a\n",a);
            break;
        case STRING_TYPE:
            printf("PUSHS string@%s\n",name); //todo
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
    generateDataConversion();
    printf("# Mathematical operations\n");
    switch (type){
        case G_TYPE_PLUS:
            printf("ADDS\n");
            break;
        case G_TYPE_DIV:
            printf("DIVS\n");
            break;
        case G_TYPE_MNUS:
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

void gensr(char* name){
    printf("POPS GF@$$result\n");
}

void generate_print_result(){
    printf("WRITE GF@$$result\n");

}