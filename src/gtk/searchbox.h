#ifndef SEARCHBOX_H
#define SEARCHBOX_H

#include "interface.h"

GtkWidget*
search_box_new(GtkWidget*);

void
search_entry_key_press(GtkWidget* window, GdkEventKey* e, GtkWidget* s_window);

#endif
