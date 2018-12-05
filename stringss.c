/*
 * FIT VUT, IFJ 2018
 * Authors:
 *    Petr Medek, xmedek07
 */

#include <string.h>
#include <malloc.h>
#include <errno.h>

#include "stringss.h"
#include "error.h"


const size_t ALOCATION_SIZE = 20;

char **make_array(){
    char **pvowels = (char **) malloc(50 * sizeof(char *));
    return pvowels;
}

void strInit(string *s) {
    if ((s->str = (char*) malloc(ALOCATION_SIZE)) == NULL){
        errors_exit(INTERNAL_ERROR,"Internl Error\n");
        return;
    }
    s->str[0] = '\0';
    s->length = 0;
    s->allocSize = ALOCATION_SIZE;
}

void strFree(string *s) {
    free(s->str);
}

void strClear(string *s) {
    s->str[0] = '\0';
    s->length = 0;
}

void delete_string(string *s){
    if(s != NULL){
        free(s->str);
        s->str = NULL;
        s->allocSize = 0;
        s->length = 0;
    }
}

void strAddCharArray(string *s1, char *arr){
    for(int i = 0; i < (int)strlen(arr);i ++){
        strAddChar(s1,arr[i]);
    }
}

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
