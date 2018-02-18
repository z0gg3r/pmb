#ifndef EDIT_H
#define EDIT_H
#define _GNU_SOURCE 	/* strsep */

#include <string.h>
#include "interface.h"

/* -- edit wrapper -- */
void
edit(GtkWidget*);

void
rename_directory_wrapper(GtkWidget*);

#endif
