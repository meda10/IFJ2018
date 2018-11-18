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

/*
 * .IFJcode18
JUMP $$main
LABEL $$main
 */

int currentLabel;

void startGenerating() {

    inScope = true;

    printf(".IFJcode18\n");

  
    printf("JUMP $$main\n");
    printf("LABEL $$main\n");

}


char *getNewLabel() {
    char *newLabel = (char *) malloc(30 * sizeof(char));
    sprintf(newLabel, "%%WL%d", ++currentLabel);
    return newLabel;
}

char *getHelpRegister() {
    char *reg = (char *) malloc(30 * sizeof(char));
    sprintf(reg, "%%HR%d", currentHelpRegister++);
    return reg;
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


void generate_if(){

}


void generate_print(int type, string *value) {
    switch (type) {
        case INTEGER_TYPE:
            printf("WRITE int@%d\n", string_To_Int(value));
            break;
        case DOUBLE_TYPE:
            printf("WRITE float@%g\n", string_to_Double(value));
            break;
        case STRING_TYPE:
            printf("WRITE string@%s\n", value->str);
            break;
        case IDENTIFIER:
            printf("WRITE %s@%s\n", get_frame(), value->str);
            break;
        default:
            break;
    }
}


void variable_declare(int type, string *name, bool used) {
    if (used) {
        printf("DEFVAR %s@%s\n", get_frame(), name->str);
        double a = 0;
        switch (type) {
            case INTEGER_TYPE:
                printf("MOVE %s@%s int@0\n", get_frame(), name->str);
                break;
            case DOUBLE_TYPE:
                printf("MOVE %s@%s float@%a\n", get_frame(), name->str, a);
                break;
            case STRING_TYPE:
                printf("MOVE %s@%s string@\n", get_frame(), name->str);
                break;
        }
    }
}



void variable_assign(int expresion_type, BTNode var1, BTNode var2) {
    if (1) { // todo bool
        char *t = get_type(var1->data.type);
        switch (expresion_type) {
            case INT_E: {
                printf("MOVE %s@%s %s@%d\n", get_frame(), var1->data.name->str, t, string_To_Int(var2->data.name)); //todo %d
                break;
            }
            case DOUBLE_E: {
                printf("MOVE %s@%s %s@%a\n", get_frame(), var1->data.name->str, t, string_to_Double(var2->data.name));
                break;
            }
            case STRING_E:
                printf("MOVE %s@%s %s@%s\n", get_frame(), var1->data.name->str, t, var2->data.name->str);
                break;
            case VARIABLE_E:
                if ((var1->data.type == INTEGER_TYPE && var2->data.type == DOUBLE_TYPE) || (var1->data.type == DOUBLE_TYPE && var2->data.type == INTEGER_TYPE)) {

                    //todo konvert

                } else {
                    printf("MOVE %s@%s %s@%s\n", get_frame(), var1->data.name->str, get_frame(),var2->data.name->str);
                }
                break;

            default:
                break;
        }
    }
}