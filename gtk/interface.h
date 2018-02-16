#ifndef INTERFACE_H
#define INTERFACE_H
#define _GNU_SOURCE 

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/tree.h"
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
extern GtkWidget* 	info_label;
extern GtkWidget* 	spinner;

/* -- quit -- */
void 
destroy(); 	

/* -- gtk interface -- */
int 
gtk_interface(int, char*[]);

#endif
