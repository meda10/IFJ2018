#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#include "tokens.h"
#include "scaner.h"
#include "parser.h"
#include "stack.h"
#include "string.h"
#include "precedence.h"

token_t *token;

tPrec prec_table[ PREC_TABE_SIZE ][ PREC_TABE_SIZE ] = 
//       */   |	   +-	|< <= > >=| == !=  |    (    |    )    |   ID    |    not    |   $
{	{    G_P  ,    G_P  ,    G_P  ,   G_P  ,    L_P  ,    G_P  ,    L_P  ,    L_P  ,    G_P },
	{    L_P  ,    G_P  ,    G_P  ,   G_P  ,    L_P  ,    G_P  ,    L_P  ,    L_P  ,    G_P },
	{    L_P  ,    L_P  ,    N_P  ,   G_P  ,    L_P  ,    G_P  ,    L_P  ,    L_P  ,    G_P },
	{    L_P  ,    L_P  ,    L_P  ,   N_P  ,    L_P  ,    G_P  ,    L_P  ,    L_P  ,    G_P },
	{    L_P  ,    L_P  ,    L_P  ,   L_P  ,    L_P  ,    E_P  ,    L_P  ,    L_P  ,    N_P },
	{    G_P  ,    G_P  ,    G_P  ,   G_P  ,    N_P  ,    G_P  ,    N_P  ,    G_P  ,    G_P }, // u not ma jeste nevim je tam zatim GN
	{    G_P  ,    G_P  ,    G_P  ,   G_P  ,    N_P  ,    G_P  ,    N_P  ,    G_P  ,    G_P }, // u not ma jeste nevim je tam zatim GN
	{    G_P  ,    G_P  ,    G_P  ,   G_P  ,    L_P  ,    G_P  ,    L_P  ,    L_P  ,    G_P },  
	{    L_P  ,    L_P  ,    L_P  ,   L_P  ,    L_P  ,    N_P  ,    L_P  ,    L_P  ,    N_P } };
	


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
void push_E(stack_t* s){
	token_t t;
    t.type = EXP;
    t.line = 1; // jen kvuli validnimu pouziti print_token
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

		case NOT:
			return NOT_P;

		case DOL:
		case EOL:
		case ENDOFFILE:
			return DOL_P;

		case EXP:
			return EXP_P;
		//default:
		//	return DOL_P;

	}
}

/*
 * Funkce vymaze zasobnik od zacatku po znak < (start of rule)
 */
void delete_rule(stack_t* s, item_stack_t* start_rule){
	item_stack_t* actual = s->top;
	while (actual != start_rule){
		stackPop(s);
		actual = s->top;
	}
	stackPop(s);
}


/*
 * Funkce prevadi pravidlo na neterminal E 
 */
int rule(stack_t* s, item_stack_t* start_rule){
	item_stack_t* rule = start_rule->previous;
	int operator;
	switch(rule->token.type){
		// pokud pravidlo zacina ID
		case IDENTIFIER:
		case INTEGER_TYPE:
		case STRING_TYPE:
		case DOUBLE_TYPE:
			delete_rule(s, start_rule);
			push_E(s);
			return 12;				// 12. <E> -> ID

		// pokud pravidlo zacina (	
		case OPENNING_BRACKET:
			if (rule->previous == NULL)
				return 0; // error
			rule = rule->previous;
			if (rule->token.type == EXP){
				if (rule->previous == NULL)
					return 0; // error
				rule = rule->previous;
				if(rule->token.type == CLOSING_BRACKET){
					delete_rule(s, start_rule);
					push_E(s);
					return 11;			//11. <E> -> (<E>)
				}
				else 
					return 0; //error
			}
			else
				return 0; //error

		// pokud pravidlo zacina <E>
		case EXP:
			if (rule->previous == NULL)
				return 0;
			rule = rule->previous;
			switch(rule->token.type){
				//1.  <E> -> <E> * <E> nebo 
				case ASTERISK:
					operator = 1;
					break;
				//2.  <E> -> <E> / <E>
				case DIVISION:
					operator = 2;
					break;
				//3.  <E> -> <E> + <E>
				case PLUS:
					operator = 3;
					break;
				//4.  <E> -> <E> - <E>
				case MINUS:
					operator = 4;
					break;
				//5.  <E> -> <E> < <E>
				case LESS:
					operator = 5;
					break;
				//6.  <E> -> <E> <= <E>
				case LESS_OR_EQUAL:
					operator = 6;
					break;				
				//7.  <E> -> <E> > <E>
				case GREATER:
					operator = 7;
					break;
				//8.  <E> -> <E> >= <E>
				case GREATER_OR_EQUAL:
					operator = 8;
					break;
				//9.  <E> -> <E> == <E>
				case DOUBLE_EQUAL:
					operator = 9;
					break;
				//10. <E> -> <E> != <E>
				case NOT_EQUAL:
					operator = 10;
					break;
				default:
					operator = 0; //error
					break;
			}
			if (rule->previous == NULL)
				return 0;
			rule = rule->previous;
			if (rule->token.type == EXP){
				delete_rule(s, start_rule);
				push_E(s);
				return operator;
			}
			else 
				return 0; //error

		default:
			return 0; //error
	}
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
		// get_next_token(token); // to by se melo volat pred touto funkci

						/*// jen pro testovani
						printStack(s);*/

		a = token_to_prec(stackTopTerm(s)->token);
		b = token_to_prec(*token);

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
				get_next_token(token);
				break;

			// pokud je v precedencni tabulce <	
			case L_P:
						/*// jen na testovani
						printf("<\n");*/
				stackPushBeforeTerm(s, stackTopTerm (s));
				stackPush (s, *token);

				get_next_token(token);
				break;

			// pokud je v precedencni tabulce > 
			case G_P:
						/*// jen na testovani
						printf(">\n");*/
				if ((start_rule = find_start_of_rule(s)) == NULL){
					stackFree(s);
					return SYNTAX_ERR;
				}
				if ((use_rule = rule(s, start_rule)) == 0){
					stackFree(s);
					return SYNTAX_ERR;
				}
				printf(" Pouzito pravidlo: %d\n", use_rule);
				break;

			// pokud je v tabulce prazdne policko -> chyba
			case N_P:
				stackFree(s);
				return SYNTAX_ERR;
				break;
		}
	}
}