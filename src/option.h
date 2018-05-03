#ifndef OPTION_H
#define OPTION_H

#include <stdlib.h>
#include <string.h>
#include "error.h"

typedef struct cl_option cl_option;
typedef struct cl_option_list cl_option_list;

/* -- create an option structure's */
cl_option*
option_new(int(*)(char*), char*);

/* -- destroy option structure's */
int
destroy_option(cl_option*);

/* -- create option list's */
cl_option_list*
option_list_new();

/* -- destroy option list's */
int
destroy_option_list(cl_option_list*);

/* -- add option to option list */
int
option_list_add(cl_option_list*, cl_option*);

/* used to exec parsed options */
void
exec_option(cl_option_list*);

#endif
