#ifndef OPTIONS_H
#define OPTIONS_H

#define NAME_OPTIONS_SIZE 32

#include "interface.h"
#include "dialog.h"

extern char* name_options_l[NAME_OPTIONS_SIZE][3];

/* -- options -- */
void
read_config(); 

void
destroy_options();

/* -- the option window -- */
void 
options_window(GtkWidget*);

#endif
