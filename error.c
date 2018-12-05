#include <stdio.h>
#include <zconf.h>
#include <stdlib.h>
#include "error.h"

/**
 * Zpracuje error vypíše chybu na STDERR a ukončí prgram
 * @param err - typ erroru
 * @param msg - zpráva
 */
void errors_exit (int err, const char *msg) {
    fprintf(stderr, "ERROR (%d): %s", err,msg);
    exit(err);
}