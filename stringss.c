#include <string.h>
#include <malloc.h>
#include <errno.h>

#include "stringss.h"
#include "error.h"


const size_t ALOCATION_SIZE = 20;
/**
 * Vztvoří array charů
 * @return array
 */
char **make_array(){
    char **pvowels = (char **) malloc(50 * sizeof(char *));
    return pvowels;
}

/**
 * Incializace stringu
 * @param s string
 */
void strInit(string *s) {
    if ((s->str = (char*) malloc(ALOCATION_SIZE)) == NULL){
        errors_exit(INTERNAL_ERROR,"Internl Error\n");
        return;
    }
    s->str[0] = '\0';
    s->length = 0;
    s->allocSize = ALOCATION_SIZE;
}

/**
 * Uvolnení paměti stringu
 * @param s string
 */
void strFree(string *s) {
    free(s->str);
}

/**
 * Vymazání dat ze stringu
 * @param s string
 */
void strClear(string *s) {
    s->str[0] = '\0';
    s->length = 0;
}

/**
 * Smazání stringu
 * @param s string
 */
void delete_string(string *s){
    if(s != NULL){
        free(s->str);
        s->str = NULL;
        s->allocSize = 0;
        s->length = 0;
    }
}

/**
 * Přidání více znaků naráz do stringi
 * @param s1 string
 * @param arr pole znaků
 */
void strAddCharArray(string *s1, char *arr){
    for(int i = 0; i < (int)strlen(arr);i ++){
        strAddChar(s1,arr[i]);
    }
}

/**
 * přidání 1 znaku do stringu
 * @param s1 string
 * @param c znak
 */
void strAddChar(string *s1, char c) {
    if (s1->length + 1 >= s1->allocSize){
        if ((s1->str = (char*) realloc(s1->str, s1->length + ALOCATION_SIZE)) == NULL){
            errors_exit(INTERNAL_ERROR,"Internl Error\n");
            return;
        }
        s1->allocSize = s1->length + ALOCATION_SIZE;
    }
    s1->str[s1->length] = c;
    s1->length++;
    s1->str[s1->length] = '\0';
}

/**
 * Kopíruje jeden string do druhého
 * @param s1 string1
 * @param s2 string
 */
void strCopyString(string *s1, string *s2){
    int newLength = s2->length;
    if (newLength >= s1->allocSize) {
        if ((s1->str = (char*) realloc(s1->str, (size_t)newLength + 1)) == NULL){
            errors_exit(INTERNAL_ERROR,"Internl Error\n");
            return;
        }
        s1->allocSize = newLength + 1;
    }
    strcpy(s1->str, s2->str);
    s1->length = newLength;
}
