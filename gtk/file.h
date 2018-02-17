#ifndef FILE_H
#define FILE_H

#include "../include/html.h"
#include "interface.h"

void
open_database(GtkWidget*, gpointer);

void
import_database(GtkWidget*, gpointer);

void
export_html_page(GtkWidget*, gpointer);

void
selective_import_window(GtkWidget*, gpointer);

void
selective_export_window(GtkWidget*, gpointer);

#endif
