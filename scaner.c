#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#include "scaner.h"
#include "tokens.h"

const char* reservedWords[] = {
        "def",
        "do",
        "else",
        "end",
        "if",
        "not",
        "nil",
        "then",
        "when",
        "inputs",
        "inputi",
        "inputf",
        "print",
        "length",
        "substr",
        "ord",
        "chr",
        "while",
};

const char* names[] = {
        "DEF",
        "DO",
        "ELSE",
        "END",
        "IF",
        "NOT",
        "NIL",
        "THEN",
        "WHEN",
        "INPUTS",
        "INPUTI",
        "INPUTF",
        "PRINT",
        "LENGTH",
        "SUBSTR",
        "ORD",
        "CHR",
        "while",
        "OPENNING_BRACKET",
        "CLOSING_BRACKET",
        "IDENTIFIER",
        "SEMICOLON",
        "INTEGER_TYPE",
        "STRING_TYPE",
        "DOUBLE_TYPE",
        "LESS",
        "GREATER",
        "LESS_OR_EQUAL",
        "GREATER_OR_EQUAL",
        "EQUAL",
        "NOT_EQUAL",
        "PLUS",
        "MINUS",
        "ASTERISK",
        "COMMA",
        "DIVISION",
        "INT_DIVISION",
        "DOUBLE_EQUAL",
        "EOL",
        "ENDOFFILE",
        "DOL",  // pomocny token na precedencni analyzu
        "EXP",  // pomocny token na precedencni analyzu
        "START_OF_RULE" // pomocny token na precedencni analyzu, znaci < zacatek pravidla v zasobniku

};

FILE *source_file = NULL;
int line = 1;
int internal_error = 0;

void set_error(){
    internal_error = INTERNAL_ERROR;
}

int return_code(){
    internal_error = return_code_string();
    if(internal_error == INTERNAL_ERROR){
        return INTERNAL_ERROR;
    } else{
        return RET_OK;
    }
}

FILE *open_file(const char *name) {
    if(strcasecmp("stdin",name) == 0){
        source_file = stdin;
        return source_file;
    }
    source_file = fopen(name, "r");
    if(source_file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        set_error();
        return source_file;
    }
    return source_file;
}

int is_key_word(string *s) {
    for(int i = 0; i < KEY_WORDS_END; i++){
        if(strcasecmp(reservedWords[i], s->str ) == 0){
            return i;
        }
    }
    return IDENTIFIER;
}

int string_To_Int(string *s) {
    const char *string = s->str;
    char *endptr;
    int n = strtol(string, &endptr, 10);
    if (endptr == string) {
        fprintf(stderr, "Internl Error: %s\n", strerror(errno));
        set_error();
        return 0;
    }
    return n;
}

double string_to_Double(string *s){
    const char *string = s->str;
    char *stopstring = NULL;
    double n = strtod(string, &stopstring);
    if(stopstring == string){
        fprintf(stderr,"String to double error");
        set_error();
        return 0.0;
    }
    return n;
}

token_t* make_new_token(){
    token_t *token = malloc(sizeof(token_t));
    memset(token,0,sizeof(token_t));
    token->type = -1;
    return token;
}

void free_token(token_t *token){
    if(&token != NULL){
        if(token->type < KEY_WORDS_END || token->type == IDENTIFIER || token->type == STRING_TYPE || token->type == DOUBLE_TYPE || token->type == INTEGER_TYPE){
            delete_string(&token->string);
        }
        free(token);
        token = NULL;
    }
}

void clear_token(token_t *token){
    if(token->type < KEY_WORDS_END || token->type == IDENTIFIER || token->type == STRING_TYPE || token->type == DOUBLE_TYPE || token->type == INTEGER_TYPE){
        delete_string(&token->string);
    }
    token->type = -1;
    token->line = -1;
}

void print_token(token_t *token){
    printf("TOKEN:\n");
    printf("   Type: %s -> %d \n",names[token->type],token->type);
    printf("   Line: %d\n",token->line);
    if(token->type < KEY_WORDS_END || token->type == IDENTIFIER || token->type == STRING_TYPE){
        printf("   STRING: ");
    } else if(token->type == INTEGER_TYPE){
        printf("   INTEGER: ");
    } else if(token->type == DOUBLE_TYPE){
        printf("   DOUBLE: ");
    } else{
        return;
    }
    int i = 0;
    while (token->string.str[i] != '\0'){
        printf("%c",token->string.str[i]);
        i++;
    }
    printf("\n      ALLOC: %d",token->string.allocSize);
    printf("\n      LENGTH: %d",token->string.length);
    printf("\n");
    printf("\n");
}

int get_next_token(token_t *token) {
    //printf("------> READING NEXT TOKEN \n");

    if(token->type != -1){
        clear_token(token);
    }

    char c;
    token->line = line;
    tState state = STATE_START;
    string *s = &(token->string);

    while (1) {
        //printf("STATE> %d | char %c\n",state,c);
        c = (char) fgetc(source_file);
        switch (state) {
            case (STATE_START) :
                if (isspace(c)) {
                    if (c == 10) {
                        token->line = line;
                        token->type = EOL;
                        line++;
                        return return_code();
                    }
                    continue;
                }
                switch(c){
                    case EOF :
                        token->line = line;
                        token->type = ENDOFFILE;
                        return return_code();
                    case '(' :
                        token->type = OPENNING_BRACKET;
                        return return_code();
                    case ')' :
                        token->type = CLOSING_BRACKET;
                        return return_code();
                    case '+' :
                        token->type = PLUS;
                        return return_code();
                    case '-' :
                        token->type = MINUS;
                        return return_code();
                    case '*' :
                        token->type = ASTERISK;
                        return return_code();
                    case ';' :
                        token->type = SEMICOLON;
                        return return_code();
                    case '#' :
                        state = STATE_COMMENT; //todo EOF
                        break;
                    case '"' :
                        strInit(s);
                        strAddChar(s,c);
                        state = STATE_STRING_TYPE;
                        break;
                    case '/' :
                        token->type = DIVISION;
                        return return_code();
                    case '\\' :
                        token->type = INT_DIVISION;
                        return return_code();
                    case ',' :
                        token->type = COMMA;
                        return return_code();
                    case '<' :
                        state = STATE_LESS_OR_EQUAL;
                        break;
                    case '>' :
                        state = STATE_GREATER_OR_EQUAL;
                        break;
                    case '=':
                        state = STATE_BLOCK_COMMENT_OR_EQUAL;
                        break;
                    case '!' :;
                        c = (char) fgetc(source_file);
                        if(c == '='){
                            token->type = NOT_EQUAL;
                            return return_code();
                        } else{
                            fprintf(stderr,"Unexpected char_%c_\n",c);
                            return RET_ERR;
                        }
                    default:
                        if(isdigit(c)){
                            strInit(s);
                            strAddChar(s,c);
                            state =  STATE_INTEGER_TYPE;
                            break;
                        }
                        if((islower(c) && isalpha(c)) || c == '_'){
                            strInit(s);
                            strAddChar(s,c);
                            state = STATE_IDENTIFIER;
                            break;
                        } else{
                            fprintf(stderr, "Unexpected char_%c_\n",c);
                            delete_string(s);
                            return RET_ERR;
                        }
                }
                break;
            case (STATE_IDENTIFIER):
                if(isalnum(c) || c == '_' ){
                    strAddChar(s,c);
                    break;
                }else if(c == '!' || c == '?') {
                    strAddChar(s,c);
                    c = (char) fgetc(source_file);
                    if(isspace(c)){
                        token->type = is_key_word(s);
                        return return_code();
                    }
                }else if(c != EOF) { //
                    token->type = is_key_word(s);
                    if(ungetc(c,source_file) == EOF){
                        fprintf(stderr,"Internal error\n");
                        delete_string(s);
                        return INTERNAL_ERROR;
                    }
                    return return_code();
                } else{
                    token->type = is_key_word(s);
                    return return_code();
                }
            case (STATE_INTEGER_TYPE):
                if(isdigit(c)){
                    strAddChar(s,c);
                    break;
                } else if(c == '.'){
                    strAddChar(s,c);
                    c = (char) fgetc(source_file);
                    if(isdigit(c)) {
                        strAddChar(s, c);
                        state = STATE_DOUBLE_TYPE;
                        break;
                    }else{
                        delete_string(s);
                        fprintf(stderr, "Unexpected char_%c_ After . have to be sequence of digits\n",c);
                        return RET_ERR;
                    }
                } else if(c == 'e' || c == 'E'){
                    strAddChar(s,c);
                    c = (char) fgetc(source_file);
                    if(c == '+' || c == '-' ) {
                        strAddChar(s, c);
                        c = (char) fgetc(source_file);
                        if(isdigit(c)){
                            strAddChar(s, c);
                            state = STATE_DOUBLE_TYPE_EXPONENT;
                            break;
                        }else{
                            delete_string(s);
                            fprintf(stderr, "Unexpected char_%c_ After +/- have to be sequence of digits",c);
                            return RET_ERR;
                        }
                    } else if(isdigit(c)){
                        strAddChar(s, c);
                        state = STATE_DOUBLE_TYPE_EXPONENT;
                        break;
                    }else{
                        delete_string(s);
                        fprintf(stderr, "Unexpected char_%c_ After e have to be +/- or sequence of digits\n",c);
                        return RET_ERR;
                    }
                } else if(c != EOF){
                    if(ungetc(c,source_file) == EOF){
                        delete_string(s);
                        fprintf(stderr,"Internal error\n");
                        return INTERNAL_ERROR;
                    }
                    //int n = string_To_Int(s);
                    //delete_string(s);
                    //token->data_t.integer_data = n;
                    token->type = INTEGER_TYPE;
                    return return_code();
                } else{
                    //int n = string_To_Int(s);
                    //delete_string(s);
                    //token->data_t.integer_data = n;
                    token->type = INTEGER_TYPE;
                    return return_code();
                }
            case (STATE_DOUBLE_TYPE):
                if(isdigit(c)){
                    strAddChar(s,c);
                    break;
                } else if(c == 'e' || c == 'E'){
                    strAddChar(s,c);
                    c = (char) fgetc(source_file);
                    if(c == '+' || c == '-'){
                        strAddChar(s,c);
                        c = (char) fgetc(source_file);
                        if(isdigit(c)){
                            strAddChar(s,c);
                            state = STATE_DOUBLE_TYPE_EXPONENT;
                            break;
                        } else{
                            fprintf(stderr, "Unexpected char_%c_ After +/- have to be sequence of digits",c);
                            delete_string(s);
                            return RET_ERR;
                        }
                    } else if(isdigit(c)){
                        strAddChar(s,c);
                        state = STATE_DOUBLE_TYPE_EXPONENT;
                        break;
                    } else{
                        fprintf(stderr, "Unexpected char_%c_ After e have to be +/- or sequence of digits\n",c);
                        delete_string(s);
                        return RET_ERR;
                    }
                }else if(isalpha(c)){
                    fprintf(stderr, "Unexpected char %c\n",c);
                    delete_string(s);
                    return RET_ERR;
                } else if(c != EOF){
                    if(ungetc(c,source_file) == EOF){
                        fprintf(stderr,"Internal error\n");
                        delete_string(s);
                        return INTERNAL_ERROR;
                    }
                    //double n = string_to_Double(s);
                    //delete_string(s);
                    //token->data_t.double_data = n;
                    token->type = DOUBLE_TYPE;
                    return return_code();
                }else{
                    //double n = string_to_Double(s);
                    //delete_string(s);

                    //token->data_t.double_data = n;
                    token->type = DOUBLE_TYPE;
                    return return_code();
                }

            case (STATE_DOUBLE_TYPE_EXPONENT):
                if(isdigit(c)){
                    strAddChar(s,c);
                    break;
                }else if(isalpha(c)){
                    fprintf(stderr, "Unexpected char %c\n",c);
                    delete_string(s);
                    return RET_ERR;
                } else if(c != EOF){
                    if(ungetc(c,source_file) == EOF){
                        fprintf(stderr,"Internal error\n");
                        delete_string(s);
                        return INTERNAL_ERROR;
                    }
                    //double n = string_to_Double(s);
                    //delete_string(s);
                    //token->data_t.double_data = n;
                    token->type = DOUBLE_TYPE;
                    return return_code();
                } else{
                    //double n = string_to_Double(s);
                    //delete_string(s);
                    //token->data_t.double_data = n;
                    token->type = DOUBLE_TYPE;
                    return return_code();
                }
            case (STATE_LESS_OR_EQUAL):
                if(c == '='){
                    token->type = LESS_OR_EQUAL;
                    return return_code();
                }else if (c != EOF){
                    if(ungetc(c,source_file) == EOF){
                        fprintf(stderr,"Internal error");
                        return INTERNAL_ERROR;
                    }
                    token->type = LESS;
                    return return_code();
                } else{
                    token->type = LESS;
                    return return_code();
                }
            case (STATE_GREATER_OR_EQUAL):
                if(c == '='){
                    token->type = GREATER_OR_EQUAL;
                    return return_code();
                } else if (c != EOF){
                    if(ungetc(c,source_file) == EOF){
                        fprintf(stderr,"Internal error");
                        return INTERNAL_ERROR;
                    }
                    token->type = GREATER;
                    return return_code();
                } else{
                    token->type = GREATER;
                    return return_code();
                }
            case (STATE_BLOCK_COMMENT):
                //printf("AAAA= %d | char %c -> %d LINE = %d\n",state,c,c,line);
                if(isspace(c)) {
                    if (c == 10) {
                        line++;
                        c = (char) fgetc(source_file); //todo line count
                        if (c == '=') {
                            c = (char) fgetc(source_file);
                            if (c == 'e') {
                                c = (char) fgetc(source_file);
                                if (c == 'n') {
                                    c = (char) fgetc(source_file);
                                    if (c == 'd') {
                                        c = (char) fgetc(source_file);
                                        if (isspace(c)) {
                                            //printf("BCCC= %d | char %c -> %d\n",state,c,c);
                                            while (c != 10) {
                                                c = (char) fgetc(source_file);
                                                //printf("BC= %d | char %c\n",state,c);
                                                if (c == EOF) {
                                                    token->line = line;
                                                    token->type = ENDOFFILE;
                                                    return return_code();
                                                }
                                                if(c == 10){
                                                    line++;
                                                }
                                            }
                                            state = STATE_START;
                                            break;
                                        } else if (c == EOF) {
                                            token->line = line;
                                            token->type = ENDOFFILE;
                                            return return_code();
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if (c == EOF) {
                    printf("COMMENT BLOCK FAIL\n"); //todo
                    token->line = line;
                    token->type = ENDOFFILE;
                    return return_code();
                }
                continue;
            case (STATE_STRING_TYPE):
                if(c == '"') {
                    strAddChar(s,c);
                    token->type = STRING_TYPE;
                    return return_code();
                } else if(c == '\\'){
                    strAddChar(s,c);
                    c = (char) fgetc(source_file);
                    if(c == 'n' || c == 's' || c == 't' || c == '"' || c == '\\'){          //if(c == 'n' | c == '"' | c == 't' | c == '\\'){
                        strAddChar(s, c);
                        break;
                    }else if(c == 'x'){
                        strAddChar(s,c);
                        c = (char) fgetc(source_file);
                        if(isxdigit(c)){
                            strAddChar(s, c);
                            break; //todo hex digit 2
                        }
                    }else{
                        fprintf(stderr, "Unexpected string \\%c\n",c);
                        delete_string(s);
                        return RET_ERR;
                    }
                } else if(c >= 31){
                    strAddChar(s,c);
                    break;
                } else if(c != EOF){
                    if(ungetc(c,source_file) == EOF){
                        delete_string(s);
                        fprintf(stderr,"Internal error\n");
                        return INTERNAL_ERROR;
                    }
                    fprintf(stderr, "Unexpected string_%c_\n",c);
                    delete_string(s);
                    return RET_ERR;             //error not a string
                } else{
                    fprintf(stderr, "Unexpected string_%c_\n",c);
                    delete_string(s);
                    return RET_ERR;
                }
            case (STATE_BLOCK_COMMENT_OR_EQUAL):
                //printf("STATE> %d | char %c",state,c);
                if(c == '='){
                    token->type = DOUBLE_EQUAL;
                    return return_code();
                } else if(c == 'b'){
                    c = (char) fgetc(source_file);
                    if(c == 'e'){
                        c = (char) fgetc(source_file);
                        if(c == 'g'){
                            c = (char) fgetc(source_file);
                            if(c == 'i'){
                                c = (char) fgetc(source_file);
                                if(c == 'n'){
                                    c = (char) fgetc(source_file);
                                    if(isspace(c)){
                                        //printf("GGGGGG\n");
                                        if(c == 10){
                                            line++;
                                        }
                                        state = STATE_BLOCK_COMMENT;
                                        break;
                                        //todo start block comment
                                    }
                                }
                            }
                        }
                    }
                    token->type = GREATER_OR_EQUAL;
                    return return_code();
                } else if (c != EOF){
                    if(ungetc(c,source_file) == EOF){
                        fprintf(stderr,"Internal error\n");
                        return INTERNAL_ERROR;
                    }
                    token->type = EQUAL;
                    return return_code();
                } else{
                    token->type = EQUAL;
                    return return_code();
                }
            case (STATE_COMMENT):
                if(c == 10){
                    token->type = EOL;
                    line++;
                    return return_code();
                }else{
                    continue;
                }
        }
    }
}