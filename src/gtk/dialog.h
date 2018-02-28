#ifndef DIALOG_H
#define DIALOG_H

#include "interface.h"

/* -- generate entries for add/edit/delete bookmarks -- */
GtkWidget**
entries(gboolean);

GtkWidget*
grid_new();

/* collect bookmarks from selected_path */
void
collect_bookmark(GtkTreeIter, bookmark_list*);

GdkPixbuf*
favicon_decode(bookmark*);

void
set_url_favicon(GtkWidget*);

void
get_all_favicons(GtkWidget* button);

/* -- get full path of selected bookmark/directory -- */
char*
get_full_path(bookmark*);

/* -- cancel -- */
void 
close_window(GtkWidget*, gpointer);

/* -- set up dialog windows -- */
GtkWidget*
dialog_new(char*, gpointer);

#endif

