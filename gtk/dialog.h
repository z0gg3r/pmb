#ifndef DIALOG_H
#define DIALOG_H

#include "gpmb.h"

/* -- generate entries for add/edit/delete bookmarks -- */
GtkWidget**
entries(gboolean);

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

