#ifndef TREEVIEW_H
#define TREEVIEW_H

#include "interface.h"

extern GtkCellRenderer* 	cell_renderer_text;
extern GtkCellRenderer* 	cell_renderer_id;
extern GtkCellRenderer* 	cell_renderer_name;
extern GtkCellRenderer* 	cell_renderer_url;
extern GtkCellRenderer* 	cell_renderer_comment;
extern GtkWidget* 		treeview;
extern GtkTreeStore* 		bookmarks;
extern GtkTreeSelection*	selection;
extern GtkTreeModel* 		model;

/* selected path in tree view */
extern GtkTreePath* 		selected_path;

/* -- copy the selected bookmark url to clipboard -- */
void
copy_to_clipboard();

/* -- expand/collapse row -- */
//unsigned int recursive
void
row_expander(GtkTreePath*, unsigned int);

/* -- create tree view -- */
GtkWidget* 
tree_view(GtkWidget*);

/* -- get data of current selected bookmark -- */
bookmark* 
get_data(GtkTreePath*); 

/* -- read database -- */
void 
read_database(GtkWidget*, GtkWidget*); 

#endif

