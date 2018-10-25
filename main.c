#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "scaner.h"

int main() {

    FILE *source_file = open_file("/home/petr/CLionProjects/IFJ2018/source");

    for(int i = 0; i < 500; i++) {

        token_t *token = make_new_token();
        int return_value = get_next_token(token);

        if (return_value != 0 ){
            return return_value;
        }

        print_token(token);
        if(token->type == 38){
            free_token(token);
            break;
        }
        free_token(token);
    }

    if(fclose(source_file) == EOF){
        fprintf(stderr, "Internl Error: %s\n", strerror(errno));
        return INTERNAL_ERROR;
    }

}