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
#include "options.h"
#include "searchbox.h"
#include "tagbox.h"

/* globals */
extern sqlite3* 		db;
extern GtkTreeModel* 		model;
extern GtkTreeStore* 		bookmarks;
extern GtkWidget* 		treeview;
extern GtkWidget* 		tool_box;
extern GtkWidget* 		main_box;
extern GtkTreeSelection*	selection;

/* -- quit -- */
void 
destroy(); 	

/* -- gtk interface -- */
int 
gtk_interface(int, char*[]);

#endif
