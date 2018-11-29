#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "error.h"
#include "tokens.h"
#include "scaner.h"
#include "parser.h"
#include "stack.h"
#include "stringss.h"
#include "precedence.h"
#include "generator.h"
#include "symtable.h"


extern token_t *token;
extern bool read_token;
extern token_t *next_token;
extern BTNode current_LTS;
extern BTNode *root_GTS;

tPrec prec_table[ PREC_TABE_SIZE ][ PREC_TABE_SIZE ] = 
//       */   |	   +-	|< <= > >=| == !=  |    (    |    )    |   ID    |   $
{	{    G_P  ,    G_P  ,    G_P  ,   G_P  ,    L_P  ,    G_P  ,    L_P  ,  G_P },
	{    L_P  ,    G_P  ,    G_P  ,   G_P  ,    L_P  ,    G_P  ,    L_P  ,  G_P },
	{    L_P  ,    L_P  ,    N_P  ,   G_P  ,    L_P  ,    G_P  ,    L_P  ,  G_P },
	{    L_P  ,    L_P  ,    L_P  ,   N_P  ,    L_P  ,    G_P  ,    L_P  ,  G_P },
	{    L_P  ,    L_P  ,    L_P  ,   L_P  ,    L_P  ,    E_P  ,    L_P  ,  N_P },
	{    G_P  ,    G_P  ,    G_P  ,   G_P  ,    N_P  ,    G_P  ,    N_P  ,  G_P },
	{    G_P  ,    G_P  ,    G_P  ,   G_P  ,    N_P  ,    G_P  ,    N_P  ,  G_P },  
	{    L_P  ,    L_P  ,    L_P  ,   L_P  ,    L_P  ,    N_P  ,    L_P  ,  N_P } };
	


/*
 * Funkce na pushnuti dolaru na zacatek zasobniku
 */
void push_dolar(stack_t* s){
	token_t t;
    t.type = DOL;
    t.line = 1; // jen kvuli validnimu pouziti print_token
    stackPush (s, t);
}


/*
 * Funkce na pushnuti znaku E(EXPRESSION) na zacatek zasobniku
 */
void push_E(stack_t* s, token_t token){
	token_t t;
	switch(token.type){
		case IDENTIFIER:
		case EXP_IDENTIFIER:
			t.type = EXP_IDENTIFIER;
			break;
		case INTEGER_TYPE:
		case EXP_INTEGER:
			t.type = EXP_INTEGER;
			break;
		case STRING_TYPE:
		case EXP_STRING:
			t.type = EXP_STRING;
			break;
		case DOUBLE_TYPE:
		case EXP_DOUBLE:
			t.type = EXP_DOUBLE;
			break;
	}

    t.line = token.line;
    strInit(&t.string);
    strCopyString(&t.string, &token.string);

    stackPush (s, t);
}

/*
 * Funkce najde a vrati ukazatel na start of rule (znacka < )
 */
item_stack_t* find_start_of_rule(stack_t* s){
		//pokud je prazdny vraci NULL
	item_stack_t* actual = s->top;

	while (actual != NULL && actual->token.type != START_OF_RULE )
		actual = actual->next;

	if (actual == NULL)
		return NULL;

	return actual;

}


/*
 * Funkce prevadi type tokenu na operator precedencni tabulky, abych ho mohl najit v precedencni tabulce
 */
tPrec_op token_to_prec(token_t t){
	
	switch(t.type){
		case ASTERISK:
		case DIVISION:
			return MUL_P;

		case PLUS:
		case MINUS:
		 	return ADD_P;

		case LESS:
		case LESS_OR_EQUAL:
		case GREATER:
		case GREATER_OR_EQUAL:
			return LEG_P;

		case DOUBLE_EQUAL:
		case NOT_EQUAL:
			return EN_P;

		case OPENNING_BRACKET:
			return LB_P;

		case CLOSING_BRACKET:
			return RB_P;

		case IDENTIFIER:
		case INTEGER_TYPE:
		case STRING_TYPE:
		case DOUBLE_TYPE:
			return ID_P;

		case DOL:
		case EOL:
		case ENDOFFILE:
		case THEN:
		case DO:
			return DOL_P;

		case EXP_IDENTIFIER:
		case EXP_INTEGER:
		case EXP_DOUBLE:
		case EXP_STRING:
			return EXP_P;
		
		default:
			return OTHER_P;

	}
}


int declared_variable(token_t variable){
	
	BTNode tmp_node, tmp2_node;
	tmp_node = B_tree_search(current_LTS, variable.string.str);
	tmp2_node = B_tree_search(*root_GTS, variable.string.str);

	if( (tmp_node == NULL) || (tmp2_node != NULL) )
		return SEM_ERR;

	return SEM_OK;

}


/*
 * Funkce na kontrolu semantiky a generovani kodu
 */
int semantic(token_t op1, token_t op2, int operator, int* result_type){

	int type_operation;
	bool relation_operator = false;
	bool is_string = false;

	switch(operator){
		case MUL_S:
			type_operation = G_TYPE_MUL;
			break;
		case DIV_S:
			type_operation = G_TYPE_IDIV;
			break;
		case PLUS_S:
			type_operation = G_TYPE_PLUS;
			break;
		case MINUS_S:		
			type_operation = G_TYPE_MINUS;
			break;
		case LESS_S:
			type_operation = G_TYPE_LESS;
			relation_operator = true;
			break;
		case LE_S:
			type_operation = G_TYPE_LESS_OR_EQUAL;
			relation_operator = true;
			break;
		case GREATER_S:
			type_operation = G_TYPE_GREATER;
			relation_operator = true;
			break;
		case GE_S:
			type_operation = G_TYPE_GREATER_OR_EQUAL;
			relation_operator = true;
			break;
		case EQUAL_S:
			type_operation = G_TYPE_EQUAL;
			relation_operator = true;
			break;
		case NOT_EQUAL_S:
			type_operation = G_TYPE_NOT_EQUAL;
			relation_operator = true;
			break;
		}
	
	if(op1.type == EXP_INTEGER && op2.type == EXP_INTEGER){
		*result_type = EXP_INTEGER;
	}

	else if(op1.type == EXP_DOUBLE && op2.type == EXP_INTEGER){
		
		if (type_operation == G_TYPE_IDIV)
			type_operation = G_TYPE_DIV;

		generate_stack_1_to_float();

		*result_type = EXP_DOUBLE;		
	}

	else if(op1.type == EXP_INTEGER && op2.type == EXP_DOUBLE){
		
		if (type_operation == G_TYPE_IDIV)
			type_operation = G_TYPE_DIV;
		
		generate_stack_2_to_float();

		*result_type = EXP_DOUBLE;	
	}

	else if (op1.type == EXP_DOUBLE && op2.type == EXP_DOUBLE){

		if (type_operation == G_TYPE_IDIV)
			type_operation = G_TYPE_DIV;

		*result_type = EXP_DOUBLE;
	}

	else if(op1.type == EXP_STRING && op2.type == EXP_STRING){

		is_string = true;
		*result_type = EXP_STRING;
	}

	else if(op1.type == EXP_IDENTIFIER && (op2.type >= EXP_INTEGER && op2.type <= EXP_STRING )){
		
		 if (declared_variable(op1) == SEM_ERR)
		 	return SEM_ERR;

		switch(op2.type){

			case EXP_INTEGER:
				// compare op1 with INTEGER (pokud je op1 FLOAT tak zmenit op2 na FLOAT)
				generate_compare_variable_2_with_int();
				break;

			case EXP_DOUBLE:
				// compare op1 with FLOAT a INTEGER(kdyztak predelat na FLOAT)
				generate_compare_variable_2_with_float();
				//*result_type = EXP_DOUBLE;
				break;

			case EXP_STRING:
				//compare op1 with string
				is_string = true;
				generate_compare_variable_2_with_string();
				break;
		}

		*result_type = EXP_IDENTIFIER;
	}

	else if((op1.type >= EXP_INTEGER && op1.type <= EXP_STRING ) && op2.type == EXP_IDENTIFIER ){
		
		if (declared_variable(op2) == SEM_ERR)
		 	return SEM_ERR;

		switch(op1.type){

			case EXP_INTEGER:
				// compare op2 with INTEGER (pokud je op2 FLOAT tak zmenit op1 na FLOAT)
				generate_compare_variable_1_with_int();
				break;
			
			case EXP_DOUBLE:
				// compare op2 with FLOAT a INTEGER(kdyztak predelat na FLOAT)
				generate_compare_variable_1_with_float();
				break;
			
			case EXP_STRING:
				//compare op2 with string
				is_string = true;
				generate_compare_variable_1_with_string();
				break;			
		}
		
		*result_type = EXP_IDENTIFIER;

	}

	else if(op1.type == EXP_IDENTIFIER && op2.type == EXP_IDENTIFIER){

		if (declared_variable(op1) == SEM_ERR || declared_variable(op2) == SEM_ERR)
		 	return SEM_ERR;

		generate_compare_variable_with_variable();
		*result_type = EXP_IDENTIFIER;

	}

	else{
		// == nebo != se muze pouzit i na ostatni typy
		if ((type_operation == G_TYPE_EQUAL) || (type_operation == G_TYPE_NOT_EQUAL)){
			generate_comparative_operations(type_operation);
			*result_type = EXP_BOOLEAN;
			return SEM_OK;
		}
		else
			return SEM_RUNTIME_ERR;
	}	

	if(relation_operator){
		generate_comparative_operations(type_operation);
		*result_type = EXP_BOOLEAN;
	}
	else{
		if(!is_string){
			generate_mathemeatical_operations(type_operation);
		}
		else{
			if(type_operation == G_TYPE_PLUS)
				generate_concat();
			else
				return SEM_RUNTIME_ERR;

		}
		
	}


	return SEM_OK;

}


/*
 * Funkce vymaze zasobnik od zacatku po znak < (start of rule) a vlozi <E> na zasobnik 
 */
void delete_rule(stack_t* s, item_stack_t* start_rule, token_t token , int* result_type){
	
	token_t t;
	
	if( token.type == IDENTIFIER || token.type == STRING_TYPE || token.type == INTEGER_TYPE || token.type == DOUBLE_TYPE ||
		(token.type >= EXP_IDENTIFIER && token.type <= EXP_BOOLEAN )){
		
		if(result_type == NULL){

			switch(token.type){
				case IDENTIFIER:
				case EXP_IDENTIFIER:
					t.type = EXP_IDENTIFIER;
					break;
				case INTEGER_TYPE:
				case EXP_INTEGER:
					t.type = EXP_INTEGER;
					break;
				case STRING_TYPE:
				case EXP_STRING:
					t.type = EXP_STRING;
					break;
				case DOUBLE_TYPE:
				case EXP_DOUBLE:
					t.type = EXP_DOUBLE;
					break;
				case EXP_BOOLEAN:
					t.type = EXP_BOOLEAN;
					break;
			}

		}else
			t.type = *result_type;
			
	    t.line = token.line;
	
	    if (t.type != EXP_BOOLEAN ){
	    	strInit(&t.string);
	    	strCopyString(&t.string, &token.string);
	    }
	    
	}


	item_stack_t* actual = s->top;
	
	while (actual != start_rule){
		
		stackPop(s);
		actual = s->top;
	
	}

	stackPop(s);

	if( token.type == IDENTIFIER || token.type == STRING_TYPE || token.type == INTEGER_TYPE || token.type == DOUBLE_TYPE ||
		(token.type >= EXP_IDENTIFIER && token.type <= EXP_BOOLEAN )){
	
	    stackPush (s, t);

	    if (t.type != EXP_BOOLEAN ){
			delete_string(&t.string);
		}
	}
		
}


/*
 * Funkce prevadi pravidlo na neterminal E 
 */
int rule(stack_t* s, item_stack_t* start_rule){

	item_stack_t* rule = start_rule->previous;
	int operator;
	int err_sem;

	switch(rule->token.type){
		// pokud pravidlo zacina ID
		case IDENTIFIER:
		case INTEGER_TYPE:
		case STRING_TYPE:
		case DOUBLE_TYPE:

			delete_rule(s, start_rule, rule->token, NULL);
			//push_E(s, E_rule.token);
			//return 12;				// 12. <E> -> ID
			return SYNTAX_OK;

		// pokud pravidlo zacina (	
		case OPENNING_BRACKET:
			
			if (rule->previous == NULL)
				return SYNTAX_ERR; // error
			
			rule = rule->previous;
			
			if ((rule->token.type >= EXP_IDENTIFIER) && (rule->token.type <= EXP_BOOLEAN)){
				
				if (rule->previous == NULL)
					return SYNTAX_ERR; // error
				
				rule = rule->previous;
				
				if(rule->token.type == CLOSING_BRACKET){
					
					delete_rule(s, start_rule, rule->next->token, NULL);
					//push_E(s, E_rule.token);
					//return 11;			//11. <E> -> (<E>)
					return SYNTAX_OK;
				}
				else
					return SYNTAX_ERR; //error
			}
			else
				return SYNTAX_ERR; //error

		// pokud pravidlo zacina <E>
		case EXP_IDENTIFIER:
		case EXP_INTEGER:
		case EXP_DOUBLE:
		case EXP_STRING:
		case EXP_BOOLEAN:

			if (rule->previous == NULL)
				return SYNTAX_ERR;
			rule = rule->previous;
			switch(rule->token.type){
				//1.  <E> -> <E> * <E>
				case ASTERISK:
					operator = MUL_S;
					break;
				//2.  <E> -> <E> / <E>
				case DIVISION:
					operator = DIV_S;
					break;
				//3.  <E> -> <E> + <E>
				case PLUS:
					operator = PLUS_S;
					break;
				//4.  <E> -> <E> - <E>
				case MINUS:
					operator = MINUS_S;
					break;
				//5.  <E> -> <E> < <E>
				case LESS:
					operator = LESS_S;
					break;
				//6.  <E> -> <E> <= <E>
				case LESS_OR_EQUAL:
					operator = LE_S;
					break;				
				//7.  <E> -> <E> > <E>
				case GREATER:
					operator = GREATER_S;
					break;
				//8.  <E> -> <E> >= <E>
				case GREATER_OR_EQUAL:
					operator = GE_S;
					break;
				//9.  <E> -> <E> == <E>
				case DOUBLE_EQUAL:
					operator = EQUAL_S;
					break;
				//10. <E> -> <E> != <E>
				case NOT_EQUAL:
					operator = NOT_EQUAL_S;
					break;
				default:
					operator = 0; //error
					break;
			}
			if (rule->previous == NULL)
				return SYNTAX_ERR;
			
			rule = rule->previous;
			if ((rule->token.type >= EXP_IDENTIFIER) && (rule->token.type <= EXP_BOOLEAN)){

				int result_type;

				if ((err_sem = semantic(rule->next->next->token, rule->token, operator, &result_type)) != SEM_OK ){
					return err_sem;
				}

				delete_rule(s, start_rule, rule->token, &result_type);
				//push_E(s, E_rule.token);
				//return operator;
				return SYNTAX_OK;
			}
			else
				return SYNTAX_ERR; //error

		default:
			return SYNTAX_ERR; //error
	}
}


/*
 * Funkce vola get_next_token pokud uz neni novy token v next_token
 */
void get_next_token_prec(){

	if (read_token){
		read_token = false; 
		*token = *next_token;
	}
	get_next_token(token);
}



/*
 * Funkce resi precedencni analyzu
 */
int precedence(){

	stack_t* s = stackInit ();
	push_dolar(s);

	tPrec_op a;
	tPrec_op b;

	tPrec prec;

	item_stack_t* start_rule;

	int use_rule;

	while(1){
		
					/*	// jen pro testovani
						printStack(s);
					*/	

		if ((a = token_to_prec(stackTopTerm(s)->token)) >= PREC_TABE_SIZE ){
			stackFree(s);
			return SYNTAX_ERR;
		}
		if ((b = token_to_prec(*token)) >= PREC_TABE_SIZE ){
			stackFree(s);
			return SYNTAX_ERR;
		}

		if(a == DOL_P && b == DOL_P){
			stackFree(s);
			return SYNTAX_OK;
		}
						/*
						// jen na testovani
						printf(" a: %d\n", a);
						printf(" b: %d\n", b);
						*/

		prec = prec_table[a][b];
		switch(prec){
			// pokud je v precedencni tabulce =
			case E_P:
						/*// jen na testovani	
						printf("=\n");*/
		
				stackPush(s, *token);
				get_next_token_prec();
				break;

			// pokud je v precedencni tabulce <	
			case L_P:
						/*// jen na testovani
						printf("<\n");*/
				stackPushBeforeTerm(s, stackTopTerm (s));
				stackPush (s, *token);
				
				// generuje code push
				if (token->type == IDENTIFIER || token->type == STRING_TYPE || token->type == INTEGER_TYPE || token->type == DOUBLE_TYPE)
					generate_push(token->type, token->string.str);
				
				get_next_token_prec();
				break;

			// pokud je v precedencni tabulce > 
			case G_P:
						/*// jen na testovani
						printf(">\n");*/
				if ((start_rule = find_start_of_rule(s)) == NULL){
					stackFree(s);
					return SYNTAX_ERR;
				}
				if ((use_rule = rule(s, start_rule)) != 0){
					stackFree(s);
					return use_rule;
				}
				break;

			// pokud je v tabulce prazdne policko -> chyba
			case N_P:
				stackFree(s);
				return SYNTAX_ERR;
				break;
		}
	}
}