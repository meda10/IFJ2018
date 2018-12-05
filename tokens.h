/*
 * FIT VUT, IFJ 2018
 * Authors:
 *    Petr Medek, xmedek07
 */

#ifndef tokens_H
#define tokens_H

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
#define WHILE 17
#define OPENNING_BRACKET 18
#define CLOSING_BRACKET 19
#define IDENTIFIER 20
#define SEMICOLON 21
#define INTEGER_TYPE 22
#define STRING_TYPE 23
#define DOUBLE_TYPE 24
#define LESS 25
#define GREATER 26
#define LESS_OR_EQUAL 27
#define GREATER_OR_EQUAL 28
#define EQUAL 29
#define NOT_EQUAL 30
#define PLUS 31
#define MINUS 32
#define ASTERISK 33
#define COMMA 34
#define DIVISION 35
#define INT_DIVISION 36
#define DOUBLE_EQUAL 37
#define EOL 38
#define ENDOFFILE 39


#define EXPRESSION_TYPE 40



extern const char* names[];

extern const char* reservedWords[];
#endif
