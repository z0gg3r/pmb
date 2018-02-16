#ifndef FILE_H
#define FILE_H

#include "../include/html.h"
#include "interface.h"

void
new_database(GtkWidget*, gpointer);

void
open_database(GtkWidget*, gpointer);

void
import_database(GtkWidget*, gpointer);

void
export_html_page(GtkWidget*, gpointer);

#endif
