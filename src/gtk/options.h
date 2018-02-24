#ifndef OPTIONS_H
#define OPTIONS_H

#include "interface.h"
#include "dialog.h"

typedef struct gpmb_options 
{
	char* database_file;
	char* tree_lines;

	/* colors */
	char* 			id_fg;
	char* 			name_fg;
	char* 			url_fg;
	char* 			comment_fg;
	char* 			tag_fg;
	const char* 	id_font;
	const char* 	name_font;
	const char* 	url_font;
	const char* 	comment_font;
} gpmb_options;

extern gpmb_options* 	opts;

/* -- options -- */
void
read_config(); 

/* -- the option window -- */
void 
options_window(GtkWidget*);

#endif
