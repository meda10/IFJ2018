/*
 * FIT VUT, IFJ 2018
 * Authors:
 *    Petr Medek, xmedek07
 */

#include <stdio.h>
#include <zconf.h>
#include <stdlib.h>
#include "error.h"

void errors_exit (int err, const char *msg) {
    fprintf(stderr, "ERROR (%d): %s", err,msg);
    exit(err);
}