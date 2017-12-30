#ifndef TREEVIEW_H
#define TREEVIEW_H

#include "interface.h"

/* selected path and column in tree view */
extern GtkTreePath* 		selected_path;
extern GtkTreeViewColumn* 	selected_column;

typedef struct directory_url directory_url;

directory_url*
directory_url_new();

void
directory_url_destroy(directory_url*);

int
directory_url_size(directory_url*);

void
directory_url_position_first(directory_url* dir_url);

char*
directory_url_next(directory_url*);

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

/* fill the char** arg with url's inside the current
   selected directory of tree view */
void
collect_directory_url(GtkTreeIter, directory_url*);

#endif
