#ifndef precedence_H
#define precedence_H

#define PREC_TABE_SIZE 8

// KOnstatnty pro semantiku
#define MUL_S 1 
#define DIV_S 2
#define PLUS_S 3
#define MINUS_S 4
#define LESS_S 5
#define LE_S 6
#define GREATER_S 7
#define GE_S 8
#define EQUAL_S 9
#define NOT_EQUAL_S 10

typedef enum {L_P, E_P, G_P, N_P } tPrec; //LESS , EQUAL, GREATER , NOTHING


     //		   0		1		 2			 3		  4     5	  6 	 7		 8      9
     //       */       +-    < <= > >=     == !=      (     )     ID     $		EXP     
typedef enum {MUL_P, ADD_P,    LEG_P,      EN_P,     LB_P, RB_P, ID_P , DOL_P, EXP_P, OTHER_P } tPrec_op;

tPrec prec_table[ PREC_TABE_SIZE ][ PREC_TABE_SIZE ];

/*
 * Funkce na pushnuti dolaru na zacatek zasobniku
 */
void push_dolar(stack_t* s);


/*
 * Funkce na pushnuti znaku E(EXPRESSION) na zacatek zasobniku
 */
void push_E(stack_t* s, token_t token);

/*
 * Funkce najde a vrati ukazatel na start of rule (znacka < )
 */
item_stack_t* find_StartOfRule(stack_t* s);

/*
 * Funkce prevadi type tokenu na operator precedencni tabulky, abych ho mohl najit v precedencni tabulce
 */
tPrec_op token_to_prec(token_t t);

/*
 * Funkce zjistuje jestli je promnema jiz declarovana a nema nazev jako funkce
 */
int declared_variable(token_t variable);

/*
 * Funkce na kontrolu semantiky a generovani kodu
 */
int semantic(token_t op1, token_t op2, int operator, int* result_type);


/*
 * Funkce vymaze zasobnik od zacatku po znak < (start of rule) a vlozi <E> na zasobnik 
 */
void delete_rule(stack_t* s, item_stack_t* start_rule, token_t token , int* result_type);

/*
 * Funkce prevadi pravidlo na neterminal E 
 */
int rule(stack_t* s, item_stack_t* start_rule);

/*
 * Funkce vola get_next_token pokud uz neni novy token v next_token
 */
void get_next_token_prec();

/*
 * Funkce resi precedencni analyzu
 */
int precedence();

#endif