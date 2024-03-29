/*
 * FIT VUT, IFJ 2018
 * Authors:
 *    Petr Medek, xmedek07
 */

#ifndef SCANNER_ERROR_H
#define SCANNER_ERROR_H



#define RETURN_OK 0
#define LEXYCAL_ERROR 1
#define SYNTAX_ERROR 2
#define SEMANTIC_ERROR_UNDEFINED_VARIABLE 3
#define SEMANTIC_ERROR_TYPE_COMPATIBILYTY 4
#define SEMANTIC_ERROR_FUNCTION_PARAMS 5
#define SEMANTIC_ERROR_OTHER 6
#define DIVISION_BY_ZERO 9
#define INTERNAL_ERROR 99

void errors_exit (int err, const char *msg);

#endif //SCANNER_ERROR_H
