#ifndef tokens
#define tokens

#define DEF 0
#define DO 1
#define ELSE 2
#define END 3
#define IF 4
#define NOT 5
#define NIL 6
#define THEN 7
#define WHEN 8
#define INPUTS 9
#define INPUTI 10
#define INPUTF 11
#define PRINT 12
#define LENGTH 13
#define SUBSTR 14
#define ORD 15
#define CHR 16
#define OPENNING_BRACKET 17
#define CLOSING_BRACKET 18
#define IDENTIFIER 19
#define SEMICOLON 20
#define INTEGER_TYPE 21
#define STRING_TYPE 22
#define DOUBLE_TYPE 23
#define LESS 24
#define GREATER 25
#define LESS_OR_EQUAL 26
#define GREATER_OR_EQUAL 27
#define EQUAL 28
#define NOT_EQUAL 29
#define PLUS 30
#define MINUS 31
#define ASTERISK 32
#define COMMA 33
#define DIVISION 34
#define INT_DIVISION 35
#define DOUBLE_EQUAL 36
#define EOL 37
#define ENDOFFILE 38

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
};

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
};

#endif
