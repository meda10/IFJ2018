#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "tokens.h"
#include "scaner.h"
#include "parser.h"


token_t *token, *next_token;
bool read_token;

int PROGRAM(){
	int result;
	printf("PROGRAM\n");
	print_token(token);
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
		case OPENNING_BRACKET:
		case NOT:
		case EOL:
			result = FUNC_DEF();					//1.<PROGRAM> -> <FUNC_DEF> <STL> <PROGRAM>
			if (result != SYNTAX_OK) return result;

			result = STL();
			if (result != SYNTAX_OK) return result;
			
			//get_next_token(token);
			return PROGRAM();
		case ENDOFFILE:
			return SYNTAX_OK;
	}
	return SYNTAX_ERR;
}

int FUNC_DEF(){
	printf("FUNC_DEF\n");
	print_token(token);
	int result;
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

			get_next_token(token);
			return SYNTAX_OK;
		case IF:
		case WHILE:
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

int PARAMS(){
	printf("PARAMS\n");
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
	printf("NEXT_PARAM\n");
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

int STL(){
	printf("STL\n");
	print_token(token);
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
		//case BOOL_TYPE:
		case INTEGER_TYPE:
		case DOUBLE_TYPE:
		case NIL:
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
	printf("S\n");
	print_token(token);
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
			result = E();	//precedencni			
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
		case NOT:									//14. <S> -> <ASS> <VALUE>
			result = ASS();
			if (result != SYNTAX_OK) return result;

			return VALUE();				
		case CLOSING_BRACKET:
			return SYNTAX_OK;						//5. <PARAMS> -> eps
	}
	return SYNTAX_ERR;
}

int ASS(){
	printf("ASS\n");
	print_token(token);
	switch(token->type){
		case STRING_TYPE:
		case DOUBLE_TYPE:
		case INTEGER_TYPE:
		case NIL:
		case NOT:									//16. <ASS> -> eps
			return SYNTAX_OK;
		case IDENTIFIER:							//16. <ASS> -> eps or 15. <ASS> -> ID =							
			get_next_token(next_token);
			read_token = true;
			if (next_token->type == EQUAL){
				read_token = false;
				get_next_token(token);
				return SYNTAX_OK;
			}else
				return SYNTAX_OK;
	}	
	return SYNTAX_ERR;
}


int VALUE(){
	printf("VALUE\n");
	print_token(token);
	switch(token->type){
		case STRING_TYPE:
		case DOUBLE_TYPE:
		case INTEGER_TYPE:
		case NIL:
		case NOT:									//17. <VALUE> -> <E>
			//give next_token to E() as second token
			return E(); //precedencni
		case IDENTIFIER:							//18. <VALUE> -> <FUNC_CALL> or 17.<VALUE> -> <E> 
			//if ID in SYMTABLE_FUNC (check if it is a function)
			if (token->data_t.string.str[0] == 97){	//for testing purpose "a" is function, else id
				return FUNC_CALL();
			}else{
				return E();	
			}	
	}	
	return SYNTAX_ERR;
}

int FUNC_CALL(){
	printf("FUNC_CALL\n");
	print_token(token);
	if(token->type == IDENTIFIER){					//19.<FUNC_CALL> -> ID <INPUT_PARAMS>
		if (read_token){
			*token = *next_token;
			read_token = false;
		}
		else
			get_next_token(token);
		return INPUT_PARAMS();
	} else
		return SYNTAX_ERR;
}

int INPUT_PARAMS(){
	printf("INPUT_PARAMS\n");
	print_token(token);
	int result;
	switch(token->type){
		case IF:
		case WHILE:
		case EOL:
		case ENDOFFILE:								//21. <INPUT_PARAMS> -> eps
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
	printf("TERM\n");
	print_token(token);
	switch(token->type){
		case IDENTIFIER:
		case STRING_TYPE:
		case INTEGER_TYPE:
		case DOUBLE_TYPE:
		//case BOOL_TYPE:
		case NIL:						//25. - 30.
			get_next_token(token);
			return SYNTAX_OK;
	}						
	return SYNTAX_ERR;
}

int NEXT_TERM(){
	printf("NEXT_TERM\n");
	print_token(token);
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
	printf("E\n");
	print_token(token);
	//zpracuj token, dalsi token neni treba volat pokud je v next_token
	if (read_token){
		*token = *next_token;
		read_token = false;
	}
	else
		get_next_token(token);
	return SYNTAX_OK;
}

int parse(){
  int result;
  token = make_new_token();
  next_token = make_new_token();
  if (get_next_token(token) == RET_ERR)
     result = RET_ERR;
  else
     result = PROGRAM();
  return result;
}
