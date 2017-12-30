#ifndef GPMB_H
#define GPMB_H

#define _GNU_SOURCE 

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/tree.h"

typedef struct gpmb_options 
{
	/* colors */
	char* id_fg;
	char* name_fg;
	char* url_fg;
	char* comment_fg;
	char* tag_fg;
} gpmb_options;

/* -- options -- */
void 
read_options();

/* -- copy the selected bookmark url to clipboard -- */
void
copy_to_clipboard();

/* -- quit -- */
void 
destroy(); 	

/* -- create a tag combo box -- */
GtkWidget*
tag_box_new();

/* -- gtk interface -- */
int 
gtk_interface(int, char*[]);

/* globals */
extern gpmb_options* 	opts;
extern sqlite3* 	db;
extern GtkTreeModel* 	model;
extern GtkTreeStore* 	bookmarks;

/* selected path and column in tree view */
extern GtkTreePath* 		selected_path;
extern GtkTreeViewColumn* 	selected_column;

#endif

