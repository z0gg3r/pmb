#ifndef TREEVIEW_H
#define TREEVIEW_H

#include "gpmb.h"

/* -- create tree view -- */
GtkWidget* 
tree_view(GtkWidget*);

/* -- get data of current selected bookmark -- */
bookmark* 
get_data(GtkTreePath*); 

/* -- read database -- */
void 
read_database(GtkWidget*, gpointer**); 

/* fill the char** arg with url's inside the current
   selected directory of tree view */
void
collect_directory_url(GtkTreeIter, char**, int*, int*);

#endif
