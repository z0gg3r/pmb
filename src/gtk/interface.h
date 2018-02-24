#ifndef INTERFACE_H
#define INTERFACE_H
#define _GNU_SOURCE 

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../tree.h"
#include "../favicon.h"
#include "treeview.h"
#include "keypress.h"
#include "menubar.h"
#include "toolbox.h"
#include "dialog.h"
#include "add.h"
#include "delete.h"
#include "edit.h"
#include "file.h"
#include "options.h"
#include "searchbox.h"
#include "tagbox.h"

/* globals */
extern char*		database_file;
extern sqlite3* 	db;
extern GtkWidget* 	main_box;
extern GtkWidget* 	info_box;
extern GtkWidget* 	info_label;
extern GtkWidget* 	search_entry;
extern GtkWidget* 	spinner;
extern GtkWidget* 	s_window;
extern GtkWidget* 	gpmb_window;

/* -- quit -- */
void 
destroy(); 	

/* hide widget */
void
widget_hide(GtkWidget*, char*);

/* -- gtk interface -- */
int 
gtk_interface(int, char*[]);

#endif
