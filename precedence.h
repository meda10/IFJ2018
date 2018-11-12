#ifndef precedence_H
#define precedence_H

#define PREC_TABE_SIZE 9

typedef enum {L_P, E_P, G_P, N_P } tPrec; //LESS , EQUAL, GREATER , NOTHING


     //		   0		1		 2			 3		  4     5	  6 	 7		8		 9
     //       */       +-    < <= > >=     == !=      (     )     ID    not     $		EXP
typedef enum {MUL_P, ADD_P,    LEG_P,      EN_P,     LB_P, RB_P, ID_P, NOT_P , DOL_P, EXP_P } tPrec_op;

tPrec prec_table[ PREC_TABE_SIZE ][ PREC_TABE_SIZE ];

/*
 * Funkce na pushnuti dolaru na zacatek zasobniku
 */
void push_dolar(stack_t* s);


/*
 * Funkce na pushnuti znaku E(EXPRESSION) na zacatek zasobniku
 */
void push_E(stack_t* s);

/*
 * Funkce najde a vrati ukazatel na start of rule (znacka < )
 */
item_stack_t* find_StartOfRule(stack_t* s);

/*
 * Funkce prevadi type tokenu na operator precedencni tabulky, abych ho mohl najit v precedencni tabulce
 */
tPrec_op token_to_prec(token_t t);

/*
 * Funkce vymaze zasobnik od zacatku po znak < (start of rule)
 */
void delete_rule(stack_t* s, item_stack_t* start_rule);


/*
 * Funkce prevadi pravidlo na neterminal E 
 */
int rule(stack_t* s, item_stack_t* start_rule);


/*
 * Funkce resi precedencni analyzu
 */
int precedence();

#endif