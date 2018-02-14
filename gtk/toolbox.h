#ifndef TOOLBOX_H
#define TOOLBOX_H

#include "interface.h"

extern GtkWidget* tool_box;

void
toolbox_hide(GtkWidget*, gpointer);

GtkWidget*
tool_box_new(GtkWidget*); 

#endif
