#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#include "tokens.h"
#include "scaner.h"
#include "stack.h"


/*
 * Vrací ukazatel na nove vytvoreny zasobnik, pokud se nepodari allokovat vraci NULL
 */
stack_t* stackInit () {
	stack_t* s = (stack_t*) malloc(sizeof(stack_t));
	if (s == NULL)
		return NULL;
	else {
		s->top = NULL;
		return s;
	}
}


/*
 * Vrací nenulovou hodnotu, pokud je zásobník prázdný, jinak vrací hodnotu 0.
 */
int stackEmpty (stack_t* s) {
	// vracim 1 nebo 0 jestli je zabovnik prazdny nebo ne
	return ((s->top == NULL) ? 1 :  0);
}


/* 
 * Vrací prvek z vrcholu zásobníku. Pokud je prazdny vraci NULL
 */
item_stack_t* stackTop (stack_t* s) {
	return s->top;
}

/* 
 * Vrací prvni terminal z vrcholu zásobníku. Pokud je prazdny vraci NULL
 */
item_stack_t* stackTopTerm (stack_t* s) {
	if(s->top == NULL)
		return NULL;
	else if (s->top->token.type < EXP)
		return s->top;

	item_stack_t* actual = s->top->next;

	while (actual != NULL && actual->token.type >= EXP )
		actual = actual->next;

	if (actual == NULL)
		return NULL;

	return actual;
}


/*
 * Odstraní prvek z vrcholu zásobníku.
 */
void stackPop (stack_t* s) {
	//pokud neni prazdny odstrani prvek z vrcholu
	if (!stackEmpty(s)){
		item_stack_t* temp = s->top;
		s->top = s->top->next;
		if(s->top != NULL)
			s->top->previous = NULL;
		free(temp);
	}	
}


/* 
 * Vloží znak na vrchol zásobníku. Pokud se nepodari allokovat pole tak vracime INTERNAL_ERROR
 */
int stackPush (stack_t* s, token_t token) {
	//inkramentujeme top a do pole ukladame terminal, pokud je plny zvetsime pole o 20 polezek
		
		item_stack_t* new = (item_stack_t*) malloc(sizeof(item_stack_t));
		if (new == NULL)
			return INTERNAL_ERROR;
		else{
			
			new->token = token;
			new->next = s->top;
			new->previous = NULL;
			if (s->top != NULL)
				s->top->previous = new;
			s->top = new;
			return 0;
		}
	
}

/* 
 * Vloží znak START_OF_RULE < pred nejvyssi terminal. Pokud se nepodari allokovat pole tak vracime INTERNAL_ERROR
 */
int stackPushBeforeTerm(stack_t* s, item_stack_t* term){

	item_stack_t* new = (item_stack_t*) malloc(sizeof(item_stack_t));
		if (new == NULL)
			return INTERNAL_ERROR;
		else{

			token_t token;
    		token.type = START_OF_RULE;
    		token.line = 1; // jen kvuli validnimu pouziti print_token

			new->token = token;
			new->next = term;
			new->previous = term->previous;

			if (term->previous != NULL)
				term->previous->next = new;
			else 
				s->top = new;
			
			term->previous = new;
			return 0;		
		}
	
}

/*
 * Uvolni alokovany zasobnik
 */
void stackFree (stack_t* s){

	item_stack_t* temp;
	while (!stackEmpty(s)){
		temp = s->top; 
		s->top = s->top->next;
		free(temp);
	}
		free(s);
}

/*
 * Funkce na vypis zasobniku
 */
void printStack(stack_t* s){ 
	item_stack_t* top = stackTop(s);
	if (stackEmpty(s)){
		 printf("\n\n ********* Zasobnik je prazdny*******\n\n");
		 return;
	}

	item_stack_t* last;

   // printf("\n\n ********* Vypis od vrcholu zasobniku ********\n\n"); 
    while (top != NULL) { 
       // print_token(&top->token); 
    //	printf(" %s ",names[top->token.type]);
        last = top;
        top = top->next;
    } 
   // printf("\n");
  
    printf("\n\n ********* Vypis v opacnem smeru ********\n\n"); 
    while (last != NULL) { 
        //print_token(&last->token); 
        printf(" %s ",names[last->token.type]);
        last = last->previous; 
    } 
    printf("\n");
} 
