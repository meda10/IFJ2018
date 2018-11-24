#ifndef scaner_H
#define scaner_H
#include "stringss.h"

#define KEY_WORDS_END 18
#define BUF_SIZE 1024

/*
 * Token
 */
typedef struct token{
    int type;
    int line;
    string string;
}token_t;

/*
 * states of  finite automaton
 */
typedef enum state{
    STATE_START,
    STATE_STRING_TYPE,
    STATE_INTEGER_TYPE,
    STATE_DOUBLE_TYPE,
    STATE_IDENTIFIER,
    STATE_LESS_OR_EQUAL,
    STATE_GREATER_OR_EQUAL,
    STATE_DOUBLE_TYPE_EXPONENT,
    STATE_COMMENT,
    STATE_BLOCK_COMMENT_OR_EQUAL,
    STATE_BLOCK_COMMENT
}tState;

void free_buffer();
char get_next_char();
void read_again();

/*
 * Function will set STDIN as input
 */
void read_from_stdin();

/*
 * Function opens file
 */
FILE *open_file(const char *name);

/*
 * Function prints token (for debug)
 */
void print_token(token_t *token);

/*
 * Function checks, if the identifier which has been found is keyword or not
 */
int is_key_word(string *s);

/*
 * Function convert string to integer
 */
int string_To_Int(char *s);

/*
 * Function convert string to double
 */
double string_to_Double(char *s);

/*
 * Function create new token and initialize token->type to -1
 */
token_t* make_new_token();

/*
 * Function finds next token and fill structure token
 */
int get_next_token(token_t *token);
/*
 * Function will free alocated memory
 */
void free_token(token_t *token);
/*
 * Function will clear token
 */
void clear_token(token_t *token);


#endif //SCANNER_scaner_H
