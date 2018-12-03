#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>

#include "tokens.h"
#include "scaner.h"
#include "parser.h"
#include "symtable.h"
#include "stack.h"
#include "precedence.h"
#include "generator.h"
#include "error.h"

token_t *token, *next_token;
bool read_token;
BTNode current_LTS;
extern BTNode *root_GTS;
extern BTNode *main_local_TS;
extern BTNode temp_node;
string actual_variable;
string actual_function;
char** actual_params;
int actual_params_number;

int parse(){
  int result;
  token = make_new_token();
  next_token = make_new_token();

  strInit(&actual_variable);
  strInit(&actual_function);
  actual_params = make_array();
  actual_params_number = 0;	
  current_LTS = *main_local_TS; //set current LTS to main body of program
  
  if (get_next_token(token) == RET_ERR)
     result = RET_ERR;
  else
     result = PROGRAM();
  return result;
}

int PROGRAM(){
	int result;
	switch(token->type){
		case DEF:
		case IF:
		case WHILE:
		case IDENTIFIER:
		case STRING_TYPE:
		//case BOOL_TYPE:
		case INTEGER_TYPE:
		case DOUBLE_TYPE:
		case NIL:
		case INPUTS:
		case INPUTI:
		case INPUTF:
		case PRINT:
		case LENGTH:
		case SUBSTR:
		case ORD:
		case CHR:
		case OPENNING_BRACKET:
		case NOT:
		case EOL:
			result = FUNC_DEF();					//1.<PROGRAM> -> <FUNC_DEF> <STL> <PROGRAM>
			if (result != SYNTAX_OK) return result;

			result = STL();
			if (result != SYNTAX_OK) return result;
			
			return PROGRAM();
		case ENDOFFILE:
			return SYNTAX_OK;
	}
	return SYNTAX_ERR;
}

int FUNC_DEF(){
	int result;
	switch(token->type){
		case DEF:									//4.<FUNC_DEF> -> def ID (<PARAMS>) EOL <STL> end
			get_next_token(token);
			if (token->type != IDENTIFIER) return SYNTAX_ERR;

			current_LTS = B_tree_search_local_table(*root_GTS, token->string.str);

			//check if function's name differ from variables' in main body
			temp_node = B_tree_search(*main_local_TS, token->string.str);
			if (temp_node != NULL){
				errors_exit(SEMANTIC_ERROR_UNDEFINED_VARIABLE, "undefined variable.");
			}

			//get function node in global TS and set initialized to true
			//indicating that parser already pass this function's definition
			temp_node = B_tree_search(*root_GTS, token->string.str);
			temp_node->data.initialized = true;

			get_next_token(token);
			if (token->type != OPENNING_BRACKET) return SYNTAX_ERR;
			
			get_next_token(token);
			result = PARAMS();
			if (result != SYNTAX_OK) return result;
			
			if (token->type != CLOSING_BRACKET) return SYNTAX_ERR;
			
			get_next_token(token);
			if (token->type != EOL) return SYNTAX_ERR;
			
			get_next_token(token);
			result = STL();
			if (result != SYNTAX_OK) return result;
			
			if (token->type != END) return SYNTAX_ERR;
			//change context to main function local table
			current_LTS = *main_local_TS;

			get_next_token(token);
			return SYNTAX_OK;
		case IF:
		case WHILE:
		case IDENTIFIER:
		case STRING_TYPE:
		case INTEGER_TYPE:
		case DOUBLE_TYPE:
		case NIL:
		case INPUTS:
		case INPUTI:
		case INPUTF:
		case PRINT:
		case LENGTH:
		case SUBSTR:
		case ORD:
		case CHR:
		case OPENNING_BRACKET:
		case NOT:
		case ENDOFFILE:
		case EOL:
			return SYNTAX_OK;						//3.<FUNC_DEF> -> eps
	}
	return SYNTAX_ERR;
}

int PARAMS(){
	switch(token->type){
		case IDENTIFIER:
			get_next_token(token);					//6. <PARAMS> -> ID <NEXT_PARAM>
			return NEXT_PARAM();
		case CLOSING_BRACKET:
			return SYNTAX_OK;						//5. <PARAMS> -> eps
	}
	return SYNTAX_ERR;
}

int NEXT_PARAM(){
	switch(token->type){
		case COMMA:
			get_next_token(token);
			if (token->type != IDENTIFIER) return SYNTAX_ERR;		//8. <NEXT_PARAM> -> , ID <NEXT_PARAM>
			
			get_next_token(token);
			return NEXT_PARAM();
		case CLOSING_BRACKET:
			return SYNTAX_OK;						//7. <NEXT_PARAM> -> eps
	}
	return SYNTAX_ERR;	
}

int STL(){
	int result;
	switch(token->type){
		case DEF:
		case END:
		case ELSE:
		case ENDOFFILE:
			return SYNTAX_OK;						//9. <STL> ->eps
		case IF:
		case WHILE:
		case IDENTIFIER:
		case STRING_TYPE:
		case INTEGER_TYPE:
		case DOUBLE_TYPE:
		case NIL:
		case INPUTS:
		case INPUTI:
		case INPUTF:
		case PRINT:
		case LENGTH:
		case SUBSTR:
		case ORD:
		case CHR:
		case OPENNING_BRACKET:
		case NOT:
		case EOL:
			result = S();							//10. <STL> -> <S> <STL>
			if (result != SYNTAX_OK) return result;

			return STL();
	}
	return SYNTAX_ERR;
}

int S(){
	int result;
	switch(token->type){
		case IF:									//12. <S> -> if <E> then EOL <STL> else EOL <STL> end
			get_next_token(token);
			result = E();	//precedencni			
			if (result != SYNTAX_OK) return result;

			if (token->type != THEN) return SYNTAX_ERR;

			get_next_token(token);
			if (token->type != EOL) return SYNTAX_ERR;

			get_next_token(token);
			result = STL();
			if (result != SYNTAX_OK) return result;

			if (token->type != ELSE) return SYNTAX_ERR;						
			
			get_next_token(token);
			if (token->type != EOL) return SYNTAX_ERR;

			get_next_token(token);
			result = STL();
			if (result != SYNTAX_OK) return result;	

			if (token->type != END) return SYNTAX_ERR;

			get_next_token(token); 
			return SYNTAX_OK;
		case WHILE:										//13. <S> -> while <E> do EOL <STL> end
			get_next_token(token);
			result = E();	
			if (result != SYNTAX_OK) return result;
													
			if (token->type != DO) return SYNTAX_ERR;						
			
			get_next_token(token);
			if (token->type != EOL) return SYNTAX_ERR;

			get_next_token(token);
			result = STL();
			if (result != SYNTAX_OK) return result;	

			if (token->type != END) return SYNTAX_ERR;

			get_next_token(token);
			return SYNTAX_OK;

		case EOL:									//11. <S> -> eps
			get_next_token(token);
			return SYNTAX_OK;
		case IDENTIFIER:
		case STRING_TYPE:
		case DOUBLE_TYPE:
		case INTEGER_TYPE:
		case NIL:
		case INPUTS:
		case INPUTI:
		case INPUTF:
		case PRINT:
		case LENGTH:
		case SUBSTR:
		case ORD:
		case CHR:
		case NOT:
		case OPENNING_BRACKET:					 	//14. <S> -> <ASS> <VALUE>
			result = ASS();
			if (result != SYNTAX_OK) return result;

			return VALUE();

	}
	return SYNTAX_ERR;
}

int ASS(){
	switch(token->type){
		case STRING_TYPE:
		case DOUBLE_TYPE:
		case INTEGER_TYPE:
		case NIL:
		case INPUTS:
		case INPUTI:
		case INPUTF:
		case PRINT:
		case LENGTH:
		case SUBSTR:
		case ORD:
		case CHR:
		case NOT:
		case OPENNING_BRACKET:						//16. <ASS> -> eps
			return SYNTAX_OK;
		case IDENTIFIER:							//16. <ASS> -> eps or 15. <ASS> -> ID =										
			get_next_token(next_token);
			read_token = true;
			if (next_token->type == EQUAL){	//variable initialization/assignment

				//check if variable's name not function's
				temp_node = B_tree_search(*root_GTS, token->string.str);
				//"initialized" checked to be sure that function is located 
				//before variable in the source code.
				if (temp_node != NULL && temp_node->data.initialized){
					errors_exit(SEMANTIC_ERROR_UNDEFINED_VARIABLE, "undefined variable.");
				}

				//check if variable is already in function's local TS
				temp_node = B_tree_search(current_LTS, token->string.str);
				//if not, insert it to current function LTS 
				if (temp_node == NULL){
					create_node(&current_LTS, token->string.str, 0, 0, NULL, false, true, false, true, &current_LTS);
				}

				variable_declare(token->string.str);
				strFree(&actual_variable);
				strInit(&actual_variable);
				strAddCharArray(&actual_variable, token->string.str);

				read_token = false;
				get_next_token(token);
				return SYNTAX_OK;
			}else
				return SYNTAX_OK;
	}	
	return SYNTAX_ERR;
}


int VALUE(){
	switch(token->type){
		case STRING_TYPE:
		case DOUBLE_TYPE:
		case INTEGER_TYPE:
		case NIL:
		case NOT:
		case OPENNING_BRACKET:						//17. <VALUE> -> <E>
			return E(); //precedencni
		case INPUTS:
		case INPUTI:
		case INPUTF:
		case PRINT:
		case LENGTH:
		case SUBSTR:
		case ORD:
		case CHR:
			return FUNC_CALL();
		case IDENTIFIER:							//18. <VALUE> -> <FUNC_CALL> or 17.<VALUE> -> <E> 
			//check if it is a function
			temp_node = B_tree_search(*root_GTS, token->string.str);
			if (temp_node != NULL && temp_node->data.is_function == true){
				return FUNC_CALL();
			}else{
				return E();		
			}
	}	
	return SYNTAX_ERR;
}

int FUNC_CALL(){
	switch(token->type){
		case INPUTS:
		case INPUTI:
		case INPUTF:
		case PRINT:
		case LENGTH:
		case SUBSTR:
		case ORD:
		case CHR:
		case IDENTIFIER:				//19.<FUNC_CALL> -> ID <INPUT_PARAMS>
			strFree(&actual_function);
			strInit(&actual_function);
			strAddCharArray(&actual_function,token->string.str);

			if (read_token){
				*token = *next_token;	//token after function ID
				read_token = false;
			}else
				get_next_token(token);
			
			generate_TF_for_function_args();
			return INPUT_PARAMS();
	}
	return SYNTAX_ERR;
}

int INPUT_PARAMS(){
	int result;
	switch(token->type){
		case IF:
		case WHILE:
		case EOL:
		case ENDOFFILE:
		case CLOSING_BRACKET:						//21. <INPUT_PARAMS> -> eps
			
			//check functions' call without parameters
			temp_node = B_tree_search(*root_GTS, actual_function.str);
			if (temp_node == NULL){
				errors_exit(SEMANTIC_ERROR_OTHER, "undefined function call.");
			}
			if (temp_node->data.params_number != 0){
				errors_exit(SEMANTIC_ERROR_FUNCTION_PARAMS, "wrong number of parameters in function call.");
			}

			generate_function_call(actual_function.str);
			return SYNTAX_OK;
		case IDENTIFIER:
		case STRING_TYPE:
		case INTEGER_TYPE:
		case DOUBLE_TYPE:
		case NIL:									//22. <INPUT_PARAMS> -> <TERM> <NEXT_TERM> 
			result = TERM();
			if (result != SYNTAX_OK) return result;

			return NEXT_TERM();
		case OPENNING_BRACKET:						//20.<INPUT_PARAMS> -> (<INPUT_PARAMS>)
			get_next_token(token);
			result = INPUT_PARAMS();
			if (result != SYNTAX_OK) return result;

			if (token->type != CLOSING_BRACKET) return SYNTAX_ERR;

			get_next_token(token);
			return SYNTAX_OK;
	}	
	return SYNTAX_ERR;
}

int TERM(){
	switch(token->type){
		case IDENTIFIER:
		case STRING_TYPE:
		case INTEGER_TYPE:
		case DOUBLE_TYPE:
		case NIL:						//25. - 30.
			actual_params[actual_params_number] = (char *) malloc(token->string.length + 1 * sizeof(char));
            strcpy(actual_params[actual_params_number], token->string.str);

			//printf("TOKEN:%s TYPE: %d\n", token->string.str, token->type);
			generate_assign_arguments_to_function(token->type, actual_params_number, actual_params[actual_params_number]);
			
			actual_params_number++;
			get_next_token(token);
			return SYNTAX_OK;
	}						
	return SYNTAX_ERR;
}

int NEXT_TERM(){
	int result;
	switch(token->type){
		case IF:
		case WHILE:
		case EOL:
		case ENDOFFILE:								
		case IDENTIFIER:
		case STRING_TYPE:
		case INTEGER_TYPE:
		case DOUBLE_TYPE:
		case NIL:
		case NOT:
		case CLOSING_BRACKET:
		case OPENNING_BRACKET:					//23. <NEXT_TERM> -> eps
			temp_node = B_tree_search(*root_GTS, actual_function.str);
			if (temp_node == NULL)
				errors_exit(SEMANTIC_ERROR_OTHER, "undefined function call.");
			//check if number of params for print function is at least one
			if ((strcmp(actual_function.str, "print") == 0) && (actual_params_number >= temp_node->data.params_number)){
				;
			} else if(temp_node->data.params_number != actual_params_number){
				errors_exit(SEMANTIC_ERROR_FUNCTION_PARAMS, "wrong number of parameters in function call.");
			} 
			// else{
			// 	printf("GG\n");
			// 	errors_exit(SEMANTIC_ERROR_FUNCTION_PARAMS, "wrong number of parameters in function call.");
			// }

			generate_function_call(actual_function.str);
			return SYNTAX_OK;
		case COMMA:								//24. <NEXT_TERM> -> , <TERM> <NEXT_TERM>	
			get_next_token(token);
			result = TERM();
			if (result != SYNTAX_OK) return result;

			return NEXT_TERM();
	}
	return SYNTAX_ERR;
}

int E(){
	int result;
	result = precedence();
	if(result != SYNTAX_OK)
		return SYNTAX_ERR;

	generate_pop_to_variable(actual_variable.str);
	return SYNTAX_OK;
}
