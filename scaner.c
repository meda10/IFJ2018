#include <strings.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>

#include "scaner.h"
#include "tokens.h"
#include "error.h"

FILE *source_file = NULL;
int line = 1;
//int internal_error = 0;
char *content;
int len = 0;
int position = 0;
bool std = true;

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
        "EXP_IDENTIFIER", // pomocny token na precedencni analyzu
        "EXP_INTEGER",// pomocny token na precedencni analyzu
        "EXP_DOUBLE",// pomocny token na precedencni analyzu
        "EXP_STRING",// pomocny token na precedencni analyzu
        "START_OF_RULE" // pomocny token na precedencni analyzu, znaci < zacatek pravidla v zasobniku

};

//void set_error(){
//    internal_error = INTERNAL_ERROR;
//}
/*
int return_code(){
    internal_error = return_code_string();
    if(internal_error == INTERNAL_ERROR){
        return INTERNAL_ERROR;
    } else{
        return RET_OK;
    }
}
*/

char get_next_char(){
    if(std){
        if(position == len){
            return EOF;
        }
        char c = content[position];
        position++;
        return c;
    } else{
        char c = (char) fgetc(source_file);

        return c;
    }
}

void free_buffer(){
    free(content);
}

/**
 * @author - oneself, Matthew Flaschen
 * https://stackoverflow.com/questions/2496668/how-to-read-the-standard-input-into-string-variable-until-eof-in-c
 * @param name
 * @return
 */
void read_from_stdin(){
    std = true;

    char buffer[BUF_SIZE];
    size_t contentSize = 1;
    content = malloc(sizeof(char) * BUF_SIZE);
    if(content == NULL){
        errors_exit(INTERNAL_ERROR,"Internl Error\n");
    }
    content[0] = '\0';
    while(fgets(buffer, BUF_SIZE, stdin)) {
        char *old = content;
        contentSize += strlen(buffer);
        content = realloc(content, contentSize);
        if(content == NULL){
            free(old);
            errors_exit(INTERNAL_ERROR,"Internl Error\n");
        }
        strcat(content, buffer);
    }
    if(ferror(stdin)){
        free(content);
        errors_exit(INTERNAL_ERROR,"Internl Error\n");
    }
    len = (int)strlen(content);
}

void read_again(){
    position = 0;
}


FILE *open_file(const char *name) {
    std = false;
    source_file = fopen(name, "r");
    if(source_file == NULL) {
        errors_exit(INTERNAL_ERROR,"Error: could not open file\n");
        return NULL;
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

int string_To_Int(char *s) {
    const char *string = s;
    char *endptr;
    int n = (int)strtol(string, &endptr, 10);
    if (endptr == string) {
        errors_exit(INTERNAL_ERROR,"Internl Error\n");
        return 0;
    }
    return n;
}

double string_to_Double(char *s){
    const char *string = s;
    char *stopstring = NULL;
    double n = strtod(string, &stopstring);
    if(stopstring == string){
        errors_exit(INTERNAL_ERROR,"Internl Error");
        return 0.0;
    }
    return n;
}

token_t* make_new_token(){
    token_t *token = malloc(sizeof(token_t));
    if(token == NULL){
        errors_exit(INTERNAL_ERROR,"Internl Error\n");
    }
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
        c = get_next_char();
        switch (state) {
            case (STATE_START) :
                if (isspace(c)) {
                    if (c == 10) {
                        token->line = line;
                        token->type = EOL;
                        line++;
                        return SYNTAX_OK;
                    }
                    continue;
                }
                switch(c){
                    case EOF :
                        token->line = line;
                        token->type = ENDOFFILE;
                        return SYNTAX_OK;
                    case '(' :
                        token->type = OPENNING_BRACKET;
                        return SYNTAX_OK;
                    case ')' :
                        token->type = CLOSING_BRACKET;
                        return SYNTAX_OK;
                    case '+' :
                        token->type = PLUS;
                        return SYNTAX_OK;
                    case '-' :
                        token->type = MINUS;
                        return SYNTAX_OK;
                    case '*' :
                        token->type = ASTERISK;
                        return SYNTAX_OK;
                    case ';' :
                        token->type = SEMICOLON;
                        return SYNTAX_OK;
                    case '#' :
                        state = STATE_COMMENT;
                        break;
                    case '"' :
                        strInit(s);
                        strAddChar(s,c);
                        state = STATE_STRING_TYPE;
                        break;
                    case '/' :
                        token->type = DIVISION;
                        return SYNTAX_OK;
                    case '\\' :
                        token->type = INT_DIVISION;
                        return SYNTAX_OK;
                    case ',' :
                        token->type = COMMA;
                        return SYNTAX_OK;
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
                        c = get_next_char();
                        if(c == '='){
                            token->type = NOT_EQUAL;
                            return SYNTAX_OK;
                        } else{
                            errors_exit(LEXYCAL_ERROR,"Unexpected char\n");
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
                            delete_string(s);
                            errors_exit(LEXYCAL_ERROR,"Unexpected char\n");
                        }
                }
                break;
            case (STATE_IDENTIFIER):
                if(isalnum(c) || c == '_' ){
                    strAddChar(s,c);
                    break;
                }else if(c == '!' || c == '?') {
                    strAddChar(s,c);
                    c = get_next_char();
                    if(isspace(c)){
                        token->type = is_key_word(s);
                        return SYNTAX_OK;
                    }
                }else if(c != EOF) { //
                    token->type = is_key_word(s);
                    if(std){
                        position--;
                    } else{
                        if(ungetc(c,source_file) == EOF){
                            fprintf(stderr,"Internal error\n");
                            delete_string(s);
                            errors_exit(INTERNAL_ERROR,"Internl Error\n");
                        }
                    }
                    return SYNTAX_OK;
                } else{
                    token->type = is_key_word(s);
                    return SYNTAX_OK;
                }
            case (STATE_INTEGER_TYPE):
                if(isdigit(c)){
                    strAddChar(s,c);
                    break;
                } else if(c == '.'){
                    strAddChar(s,c);
                    c = get_next_char();
                    if(isdigit(c)) {
                        strAddChar(s, c);
                        state = STATE_DOUBLE_TYPE;
                        break;
                    }else{
                        delete_string(s);
                        errors_exit(LEXYCAL_ERROR,"Unexpected char\n");
                    }
                } else if(c == 'e' || c == 'E'){
                    strAddChar(s,c);
                    c = get_next_char();
                    if(c == '+' || c == '-' ) {
                        strAddChar(s, c);
                        c = get_next_char();
                        if(isdigit(c)){
                            strAddChar(s, c);
                            state = STATE_DOUBLE_TYPE_EXPONENT;
                            break;
                        }else{
                            delete_string(s);
                            errors_exit(LEXYCAL_ERROR,"Unexpected char\n");
                        }
                    } else if(isdigit(c)){
                        strAddChar(s, c);
                        state = STATE_DOUBLE_TYPE_EXPONENT;
                        break;
                    }else{
                        delete_string(s);
                        errors_exit(LEXYCAL_ERROR,"Unexpected char\n");
                    }
                } else if(c != EOF){
                    if(std){
                        position--;
                    } else{
                        if(ungetc(c,source_file) == EOF){
                            fprintf(stderr,"Internal error\n");
                            delete_string(s);
                            errors_exit(INTERNAL_ERROR,"Internl Error\n");
                        }
                    }
                    token->type = INTEGER_TYPE;
                    return SYNTAX_OK;
                } else{
                    token->type = INTEGER_TYPE;
                    return SYNTAX_OK;
                }
            case (STATE_DOUBLE_TYPE):
                if(isdigit(c)){
                    strAddChar(s,c);
                    break;
                } else if(c == 'e' || c == 'E'){
                    strAddChar(s,c);
                    c = get_next_char();
                    if(c == '+' || c == '-'){
                        strAddChar(s,c);
                        c = get_next_char();
                        if(isdigit(c)){
                            strAddChar(s,c);
                            state = STATE_DOUBLE_TYPE_EXPONENT;
                            break;
                        } else{
                            delete_string(s);
                            errors_exit(LEXYCAL_ERROR,"Unexpected char\n");
                        }
                    } else if(isdigit(c)){
                        strAddChar(s,c);
                        state = STATE_DOUBLE_TYPE_EXPONENT;
                        break;
                    } else{
                        delete_string(s);
                        errors_exit(LEXYCAL_ERROR,"Unexpected char\n");
                    }
                }else if(isalpha(c)){
                    delete_string(s);
                    errors_exit(LEXYCAL_ERROR,"Unexpected char\n");
                } else if(c != EOF){
                    if(std){
                        position--;
                    } else{
                        if(ungetc(c,source_file) == EOF){
                            fprintf(stderr,"Internal error\n");
                            delete_string(s);
                            errors_exit(INTERNAL_ERROR,"Internl Error\n");
                        }
                    }
                    token->type = DOUBLE_TYPE;
                    return SYNTAX_OK;
                }else{
                    token->type = DOUBLE_TYPE;
                    return SYNTAX_OK;
                }

            case (STATE_DOUBLE_TYPE_EXPONENT):
                if(isdigit(c)){
                    strAddChar(s,c);
                    break;
                }else if(isalpha(c)){
                    delete_string(s);
                    errors_exit(LEXYCAL_ERROR,"Unexpected char\n");
                } else if(c != EOF){
                    if(std){
                        position--;
                    } else{
                        if(ungetc(c,source_file) == EOF){
                            fprintf(stderr,"Internal error\n");
                            delete_string(s);
                            errors_exit(INTERNAL_ERROR,"Internl Error\n");
                        }
                    }
                    token->type = DOUBLE_TYPE;
                    return SYNTAX_OK;
                } else{
                    token->type = DOUBLE_TYPE;
                    return SYNTAX_OK;
                }
            case (STATE_LESS_OR_EQUAL):
                if(c == '='){
                    token->type = LESS_OR_EQUAL;
                    return SYNTAX_OK;
                }else if (c != EOF){
                    if(std){
                        position--;
                    } else{
                        if(ungetc(c,source_file) == EOF){
                            fprintf(stderr,"Internal error\n");
                            errors_exit(INTERNAL_ERROR,"Internl Error\n");
                        }
                    }
                    token->type = LESS;
                    return SYNTAX_OK;
                } else{
                    token->type = LESS;
                    return SYNTAX_OK;
                }
            case (STATE_GREATER_OR_EQUAL):
                if(c == '='){
                    token->type = GREATER_OR_EQUAL;
                    return SYNTAX_OK;
                } else if (c != EOF){
                    if(std){
                        position--;
                    } else{
                        if(ungetc(c,source_file) == EOF){
                            fprintf(stderr,"Internal error\n");
                            errors_exit(INTERNAL_ERROR,"Internl Error\n");
                        }
                    }
                    token->type = GREATER;
                    return SYNTAX_OK;
                } else{
                    token->type = GREATER;
                    return SYNTAX_OK;
                }
            case (STATE_BLOCK_COMMENT):
                if(isspace(c)) {
                    if (c == 10) {
                        line++;
                        c = get_next_char();; //todo line count
                        if (c == '=') {
                            c = get_next_char();
                            if (c == 'e') {
                                c = get_next_char();
                                if (c == 'n') {
                                    c = get_next_char();
                                    if (c == 'd') {
                                        c = get_next_char();
                                        if (isspace(c)) {
                                            //printf("BCCC= %d | char %c -> %d\n",state,c,c);
                                            while (c != 10) {
                                                c = get_next_char();
                                                //printf("BC= %d | char %c\n",state,c);
                                                if (c == EOF) {
                                                    token->line = line;
                                                    token->type = ENDOFFILE;
                                                    return SYNTAX_OK;
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
                                            return SYNTAX_OK;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if (c == EOF) {
                    token->line = line;
                    token->type = ENDOFFILE;
                    errors_exit(LEXYCAL_ERROR,"Unexpected char\n");
                }
                continue;
            case (STATE_STRING_TYPE):
                if(c == '"') {
                    strAddChar(s,c);
                    token->type = STRING_TYPE;
                    return SYNTAX_OK;
                } else if(c == '\\'){
                    strAddChar(s,c);
                    c = get_next_char();
                    if(c == 'n' || c == 's' || c == 't' || c == '"' || c == '\\'){          //if(c == 'n' | c == '"' | c == 't' | c == '\\'){
                        strAddChar(s, c);
                        break;
                    }else if(c == 'x'){
                        strAddChar(s,c);
                        c = get_next_char();
                        if(isxdigit(c)){
                            strAddChar(s, c);
                            break; //todo hex digit 2
                        }
                    }else{
                        delete_string(s);
                        errors_exit(LEXYCAL_ERROR,"Unexpected char\n");
                    }
                } else if(c >= 31){
                    strAddChar(s,c);
                    break;
                } else if(c != EOF){
                    if(std){
                        position--;
                    } else{
                        if(ungetc(c,source_file) == EOF){
                            delete_string(s);
                            fprintf(stderr,"Internal error\n");
                            errors_exit(INTERNAL_ERROR,"Internl Error\n");
                        }
                    }
                    delete_string(s);
                    errors_exit(LEXYCAL_ERROR,"Unexpected char\n");
                } else{
                    delete_string(s);
                    errors_exit(LEXYCAL_ERROR,"Unexpected char\n");
                }
            case (STATE_BLOCK_COMMENT_OR_EQUAL):
                //printf("STATE> %d | char %c",state,c);
                if(c == '='){
                    token->type = DOUBLE_EQUAL;
                    return SYNTAX_OK;
                } else if(c == 'b'){
                    c = get_next_char();
                    if(c == 'e'){
                        c = get_next_char();
                        if(c == 'g'){
                            c = get_next_char();
                            if(c == 'i'){
                                c = get_next_char();
                                if(c == 'n'){
                                    c = get_next_char();
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
                    return SYNTAX_OK;
                } else if (c != EOF){
                    if(std){
                        position--;
                    } else{
                        if(ungetc(c,source_file) == EOF){
                            fprintf(stderr,"Internal error\n");
                            errors_exit(INTERNAL_ERROR,"Internl Error\n");
                        }
                    }
                    token->type = EQUAL;
                    return SYNTAX_OK;
                } else{
                    token->type = EQUAL;
                    return SYNTAX_OK;
                }
            case (STATE_COMMENT):
                if(c == 10){
                    token->type = EOL;
                    line++;
                    return SYNTAX_OK;
                }else if(c == EOF){
                    token->type = ENDOFFILE;
                    line++;
                    return SYNTAX_OK;
                } else{
                    continue;
                }
        }
    }
}