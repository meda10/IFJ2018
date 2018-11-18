//
// Created by petr on 18.11.18.
//

#ifndef SCANNER_GENERATOR_H
#define SCANNER_GENERATOR_H

#include <stdbool.h>
#include "string.h"
#include "symtable.h"

#define PRINT_S 1
#define RETURN_S 2
#define WHILE_S 3
#define VARIABLE_ASSIGN_S 4
#define VARIABLE_DECLARE_S 1
#define BUILD_IN_FUNCTOON_S 1
#define ASSIGN_FUNCTION_S 1
#define FUNCTION_DEFINITION_S 1
#define IF_S 1
#define INPUT_S 1
#define SCOPE_S 1

bool inScope;



void startGenerating();
void generate_print(int type, string *value) ;
void variable_declare(int type, string *name, bool used);


#endif //SCANNER_GENERATOR_H
