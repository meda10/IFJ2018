#include <string.h>
#include <stdio.h>

#include "tokens.h"
#include "scaner.h"
#include "parser.h"


token_t *token;

int PROGRAM(){
	int result;
	print_token(token);
	switch(token->type){
		case DEF:
		case IF:
		//case WHILE:
		case IDENTIFIER:
		case STRING_TYPE:
		//case BOOL_TYPE:
		case INTEGER_TYPE:
		case DOUBLE_TYPE:
		case NIL:
		case OPENNING_BRACKET:
		case NOT:
		case EOL:
			result = FUNC_DEF();				//<PROGRAM> -> <FUNC_DEF> <STL> <PROGRAM>
			if (result != SYNTAX_OK) return result;

			get_next_token(token);
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
	print_token(token);
	switch(token->type){
		case DEF:									//<FUNC_DEF> -> def ID (<PARAMS>) EOL <STL> end
			get_next_token(token);
			if (token->type != IDENTIFIER) return SYNTAX_ERR;
			
			get_next_token(token);
			if (token->type != OPENNING_BRACKET) return SYNTAX_ERR;
			
			get_next_token(token);
			result = PARAMS();
			if (result != SYNTAX_OK) return result;		//	!!!TODO!!!	napevno ->eps, get_next_token se nevola
			
			if (token->type != CLOSING_BRACKET) return SYNTAX_ERR;
			
			get_next_token(token);
			if (token->type != EOL) return SYNTAX_ERR;
			
			get_next_token(token);
			result = STL();
			if (result != SYNTAX_OK) return result;		//	!!!TODO!!!	napevno ->eps, get_next_token se nevola
			
			if (token->type != END) return SYNTAX_ERR;

			return SYNTAX_OK;
		case IF:
		//case WHILE:
		case IDENTIFIER:
		case STRING_TYPE:
		//case BOOL_TYPE:
		case INTEGER_TYPE:
		case DOUBLE_TYPE:
		case NIL:
		case OPENNING_BRACKET:
		case NOT:
		case ENDOFFILE:
		case EOL:
			return SYNTAX_OK;
	}
	return SYNTAX_ERR;
}

int STL(){
	return SYNTAX_OK;
}

int PARAMS(){
	return SYNTAX_OK;
}

int parse(){
  int result;
  token = make_new_token();
  if (get_next_token(token) == RET_ERR)
     result = RET_ERR;
  else
     result = PROGRAM();
  return result;
}
