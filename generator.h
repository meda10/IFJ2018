#ifndef SCANNER_GENERATOR_H
#define SCANNER_GENERATOR_H

#include <stdbool.h>
#include "stringss.h"
#include "symtable.h"
#include "scaner.h"

#define INT_E 1
#define DOUBLE_E 2
#define STRING_E 3
#define VARIABLE_E 4

//Matematicke operace
#define G_TYPE_PLUS 1
#define G_TYPE_MINUS 2
#define G_TYPE_DIV 3
#define G_TYPE_IDIV 4
#define G_TYPE_MUL 5

//Porovnavani
#define G_TYPE_LESS 6
#define G_TYPE_LESS_OR_EQUAL 7
#define G_TYPE_GREATER 8
#define G_TYPE_GREATER_OR_EQUAL 9
#define G_TYPE_EQUAL 10
#define G_TYPE_NOT_EQUAL 11


#define MAX_INSTRUCTION_LEN 150

string instrukce;

char *arr_free[1] ;
int arr_free_pos;

int label_number;
int if_num;
int if_else_num;
int if_end_num;
bool inScope;

char free_labels();
void startGenerating();
void variable_declare(char* name);
char *get_new_label();

void generate_inputi();
void generate_inputs();
void generate_inputf();
void generate_length();
void generate_ord();

void generate_stack_1_to_float();
void generate_stack_1_to_int();
void generate_stack_2_to_float();
void generate_stack_2_to_int();

void generate_start();
void generate_free_memory();
void generate_main();
void generate_main_end();
void generate_push(int type, char* name);
void generate_mathemeatical_operations(int type);
void generate_print();
void generate_print_result();
void generate_variable_assign(int expresion_type,char* variable_name, char* variable_value);
void generate_pop_to_result();
void data_conversion();

void generate_concat();

void generate_if(int num);
void generate_else(int num, bool else_statment);
void generate_if_else_end(int num);

void generate_comparative_operations(int type);



void generate_while_condition_check(int num);
void generate_while_start(int num);
void generate_while_end(int num);
void generate_pop_to_variable(char* name);


void generate_assign_arguments_to_function(int expresion_type, int num, char *value);
void generate_read_function_params(int num, char *name);
void generate_function_call(char* name);
void generate_TF_for_function_args();
void generate_function_return_value_assign_to_var(char *name);
void generate_function_start(char *name);
void generate_function_end(char *name);
void generate_function_return(char *name);

#endif //SCANNER_GENERATOR_H

