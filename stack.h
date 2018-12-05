/*
 * FIT VUT, IFJ 2018
 * Authors:
 *    Jan Sladky, xsladk09
 */

#ifndef stack_H
#define stack_H

// pomocne tokeny pro precedencni analyzu
#define DOL 40 // hodnota DOLAR
#define EXP_IDENTIFIER 41 // hodnota EXPRESSION
#define EXP_INTEGER 42 // hodnota EXPRESSION
#define EXP_DOUBLE 43 // hodnota EXPRESSION
#define EXP_STRING 44 // hodnota EXPRESSION
#define EXP_BOOLEAN 45 // hodnota EXPRESSION
#define START_OF_RULE 46 // hodnota zacatku pravidla u precedencni analyzy

typedef struct item {
	token_t token;         //  jeden prvek (data) zasobniku 
	struct item* next;			// ukazatel na dalsi prvek zasobniku
	struct item* previous;		// ukazatel na predchozi prvek zasobniku
} item_stack_t;

typedef struct stack {
	item_stack_t* top;         //  jeden prvek (data) zasobniku
} stack_t;


/*
 * Vrací ukazatel na nove vytvoreny zasobnik, pokus se nepodari allokovat vraci NULL
 */
stack_t* stackInit ();

/*
 * Vrací nenulovou hodnotu, pokud je zásobník prázdný, jinak vrací hodnotu 0.
 */
int stackEmpty ( stack_t* s );

/* 
 * Vrací prvek z vrcholu zásobníku. Pokud je prazdny vraci NULL
 */
item_stack_t* stackTop ( stack_t* s );

/* 
 * Vrací prvni terminal z vrcholu zásobníku. Pokud je prazdny vraci NULL
 */
item_stack_t* stackTopTerm (stack_t* s);

/*
 * Odstraní prvek z vrcholu zásobníku.
 */
void stackPop ( stack_t* s );

/* 
 * Vloží znak na vrchol zásobníku. Pokud se nepodari allokovat pole tak vracime INTERNAL_ERROR
 */
int stackPush ( stack_t* s, token_t token);

/* 
 * Vloží znak < pred nejvyssi terminal. Pokud se nepodari allokovat pole tak vracime INTERNAL_ERROR
 */
int stackPushBeforeTerm(stack_t* s, item_stack_t* term);

/*
 * Uvolni alokovany zasobnik
 */
void stackFree (stack_t* s);

/*
 * Funkce na vypis zasobniku
 */
void printStack(stack_t* s);


#endif