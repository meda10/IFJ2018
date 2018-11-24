#include <string.h>
#include <malloc.h>
#include <errno.h>

#include "stringss.h"


const size_t ALOCATION_SIZE = 20;
int internal_error_string = 0;

void set_error_string(){
    internal_error_string = INTERNAL_ERROR;
}

int return_code_string(){
    if(internal_error_string == INTERNAL_ERROR){
        return INTERNAL_ERROR;
    } else{
        return RET_OK;
    }
}

void strInit(string *s) {
    if ((s->str = (char*) malloc(ALOCATION_SIZE)) == NULL){
        fprintf(stderr, "Internl Error: %s\n", strerror(errno));
        set_error_string();
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
    for(int i = 0; i < strlen(arr);i ++){
        strAddChar(s1,arr[i]);
    }
}

void strAddChar(string *s1, char c) {
    if (s1->length + 1 >= s1->allocSize){
        if ((s1->str = (char*) realloc(s1->str, s1->length + ALOCATION_SIZE)) == NULL){
            fprintf(stderr, "Internl Error: %s\n", strerror(errno));
            set_error_string();
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
            fprintf(stderr, "Internl Error: %s\n", strerror(errno));
            set_error_string();
            return;
        }
        s1->allocSize = newLength + 1;
    }
    strcpy(s1->str, s2->str);
    s1->length = newLength;
}
