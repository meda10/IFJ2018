#include <string.h>
#include <stdio.h>

#include "tokens.h"
#include "scaner.h"
#include "parser.h"


token_t *token;

int PROGRAM(){
	int result;
	printf("PROGRAM\n");
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
			result = FUNC_DEF();					//1.<PROGRAM> -> <FUNC_DEF> <STL> <PROGRAM>
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
	printf("FUNC_DEF\n");
	print_token(token);
	switch(token->type){
		case DEF:									//4.<FUNC_DEF> -> def ID (<PARAMS>) EOL <STL> end
			get_next_token(token);
			if (token->type != IDENTIFIER) return SYNTAX_ERR;
			
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
			return SYNTAX_OK;						//3.<FUNC_DEF> -> eps
	}
	return SYNTAX_ERR;
}

int STL(){
	int result;
	printf("STL\n");
	print_token(token);
	switch(token->type){
		case DEF:
		case END:
		case ELSE:
		case ENDOFFILE:
		case EOL:
			return SYNTAX_OK;						//9. <STL> ->eps
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
			result = S();							//10. <STL> -> <S> <STL>
			if (result != SYNTAX_OK) return result;
			return STL();
	}
	return SYNTAX_ERR;
}

int S(){
	//TODO
	return SYNTAX_OK;
}

int PARAMS(){
	print_token(token);
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
	int result;
	print_token(token);
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

int parse(){
  int result;
  token = make_new_token();
  if (get_next_token(token) == RET_ERR)
     result = RET_ERR;
  else
     result = PROGRAM();
  return result;
}
