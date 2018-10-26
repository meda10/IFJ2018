#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "scaner.h"
#include "parser.h"

int main() {

    FILE *source_file = open_file("/home/drankou/Desktop/IFJ/project/IFJ2018/source");

    int result = parse();
    printf("%d\n", result);

    if(fclose(source_file) == EOF){
        fprintf(stderr, "Internl Error: %s\n", strerror(errno));
        return INTERNAL_ERROR;
    }

}