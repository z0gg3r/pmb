#ifndef FILE_H
#define FILE_H

#include "../include/html.h"
#include "interface.h"

void
open_database(GtkWidget*);

void
import_database(GtkWidget*);

void
export_html_page(GtkWidget*);

void
selective_import_window(GtkWidget*);

void
selective_export_window(GtkWidget*);

#endif
