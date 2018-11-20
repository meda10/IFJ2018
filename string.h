#ifndef SCANNER_string_H
#define SCANNER_string_H

#define RET_OK 0
#define RET_ERR 1
#define INTERNAL_ERROR 99

typedef struct {
    char* str;		// misto pro dany retezec ukonceny znakem '\0'
    int length;		// skutecna delka retezce
    int allocSize;	// velikost alokovane pameti
} string;

void set_error_string();
int return_code_string();
void strAddCharArray(string *s1, char *arr);
void strInit(string *s);
void strFree(string *s);
void strClear(string *s);
void strAddChar(string *s1, char c);
void delete_string(string *s);
void strCopyString(string *s1, string *s2);

#endif
