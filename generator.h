#ifndef SCANNER_GENERATOR_H
#define SCANNER_GENERATOR_H

#include <stdbool.h>
#include "string.h"
#include "symtable.h"
#include "scaner.h"

#define INT_E 1
#define DOUBLE_E 2
#define STRING_E 3
#define VARIABLE_E 4


#define G_TYPE_PLUS 1
#define G_TYPE_MNUS 2
#define G_TYPE_DIV 3
#define G_TYPE_IDIV 4
#define G_TYPE_MUL 5


int label_number;
bool inScope;


void startGenerating();
void variable_declare(int type, char* name);
char *get_new_label();



void generate_start();
void generate_main();
void generate_main_end();
void generate_push(int type, char* name);
void generate_mathemeatical_operations(int type);
void generate_print(int type, char* name);
void generate_print_result();
void generate_variable_assign(int expresion_type,char* variable_name, char* variable_value);
void gensr(char* name);
void data_conversion();


#endif //SCANNER_GENERATOR_H

