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


void startGenerating() {
    inScope = true;

    printf(".IFJcode18\n");

    //todo generovani vestavenych funkci
    printf("JUMP $$main\n");
    printf("LABEL $$main\n");
}

char *getVarFrame() {
    char *frameTP = (char *) malloc(30 * sizeof(char));
    if (!inScope) {
        strcpy(frameTP, "LF");
    } else {
        strcpy(frameTP, "GF");
    }
    return frameTP;
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
            printf("WRITE %s@%s\n", getVarFrame(), value->str);
            break;
        default:
            break;
    }
}


void variable_declare(int type, string *name, bool used) {
    if (used) {
        printf("DEFVAR %s@%s\n", getVarFrame(), name->str);
        switch (type) {
            case INTEGER_TYPE:
                printf("MOVE %s@%s int@0\n", getVarFrame(), name->str);
                break;
            case DOUBLE_TYPE:
                printf("MOVE %s@%s float@0\n", getVarFrame(), name->str);
                break;
            case STRING_TYPE:
                printf("MOVE %s@%s string@\n", getVarFrame(), name->str);
                break;
        }
    }
}
