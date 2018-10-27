#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "parser.h"
#include "scaner.h"

int main() {

    FILE *source_file = open_file("/home/petr/CLionProjects/IFJ2018/source");

    int result = parse();
    printf("%d\n", result);

    /**
     * Jenom Scaner - cteni jednotlivych tokenu
     */
/*
    for(int i = 0; i < 500; i++){
        token_t *token = make_new_token();
        if (get_next_token(token) == RET_ERR)
            return RET_ERR;
        else
            print_token(token);

        if(token->type == 39){
            free_token(token);
            return 0;
        } else{
            free_token(token);
        }

    }
*/

    if(fclose(source_file) == EOF){
        fprintf(stderr, "Internl Error: %s\n", strerror(errno));
        return INTERNAL_ERROR;
    }

}