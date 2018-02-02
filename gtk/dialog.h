#ifndef DIALOG_H
#define DIALOG_H

#include "interface.h"

/* -- generate entries for add/edit/delete bookmarks -- */
GtkWidget**
entries(gboolean);

/* collect bookmarks from selected_path */
void
collect_bookmark(GtkTreeIter, bookmark_list*);

/* used to set text from tag box */
void
tag_entry_set_text(GtkWidget*, GtkWidget*);

/* -- cancel -- */
void 
close_window(GtkWidget*, gpointer);

/* -- set up dialog windows -- */
GtkWidget*
dialogs(char*, gpointer);

#endif

