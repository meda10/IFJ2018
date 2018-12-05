/*
 * FIT VUT, IFJ 2018
 * Authors:
 *    Aliaksandr Drankou, xdrank00
 *    Petr Medek, xmedek07
 *    Jan Sladky, xsladk09 
 */

#ifndef SCANNER_GENERATOR_H
#define SCANNER_GENERATOR_H

#include <stdbool.h>
#include "stringss.h"
#include "symtable.h"
#include "scaner.h"

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
int label_number;

int get_new_label_number();

void generate_start();
void generate_main();
void generate_main_end();

void generate_inputi();
void generate_inputs();
void generate_inputf();
void generate_length();
void generate_concat();
void generate_print();
void generate_ord();

void generate_compare_variable_1_with_float();
void generate_compare_variable_1_with_int();
void generate_compare_variable_1_with_string();

void generate_compare_variable_2_with_float();
void generate_compare_variable_2_with_int();
void generate_compare_variable_2_with_string();

int generate_compare_variable_with_variable(int operator_type);

void generate_stack_1_to_float();
void generate_stack_1_to_int();
void generate_stack_2_to_float();
void generate_stack_2_to_int();

void generate_push(int type, char* name);
void generate_mathemeatical_operations(int type, int label_number);
void generate_print();
void generate_print_result();
void generate_variable_assign(int expresion_type,char* variable_name, char* variable_value);
void generate_pop_to_result();
void data_conversion();

void generate_if(int num);
void generate_else(int num);
void generate_jump_to_if_else_end(int num);
void generate_if_else_end(int num);

void generate_while_condition_check(int num);
void generate_while_start(int num);
void generate_while_end(int num);

void variable_declare(char* name);
void generate_pop_to_variable(char* name);

void generate_function_start(char *name);
void generate_function_end(char *name);
void generate_function_return(char *name);

void generate_TF_for_function_args();
void generate_assign_arguments_to_function(int expresion_type, int num, char *value);
void generate_function_call(char* name);

void generate_retval_to_var(char *name);
void generate_read_function_params(int num, char *name);

void generate_free_memory();
char free_labels();

#endif //SCANNER_GENERATOR_H

