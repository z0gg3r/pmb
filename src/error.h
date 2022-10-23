#ifndef ERROR_H
#define ERROR_H

#include <stdlib.h>
#include <stdio.h>

#ifdef _USE_JEMALLOC
#include <jemalloc/jemalloc.h>
#endif


/* check for memory allocation error */
void check_oom(void *, char *);

#endif
