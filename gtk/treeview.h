#ifndef TREEVIEW_H
#define TREEVIEW_H

#include "interface.h"

/* selected path and column in tree view */
extern GtkTreePath* 		selected_path;
extern GtkTreeViewColumn* 	selected_column;

/* -- copy the selected bookmark url to clipboard -- */
void
copy_to_clipboard();

/* -- create tree view -- */
GtkWidget* 
tree_view(GtkWidget*);

/* -- get data of current selected bookmark -- */
bookmark* 
get_data(GtkTreePath*); 

/* -- read database -- */
void 
read_database(GtkWidget*, gpointer**); 

#endif
