#ifndef TREEVIEW_H
#define TREEVIEW_H

#include "interface.h"

extern GtkCellRenderer*	g_cell_renderer_text;
extern GtkCellRenderer*	g_cell_renderer_id;
extern GtkCellRenderer*	g_cell_renderer_name;
extern GtkCellRenderer*	g_cell_renderer_url;
extern GtkCellRenderer*	g_cell_renderer_comment;
extern GtkWidget* g_treeview;
extern GtkTreeStore* g_bookmarks;
extern GtkTreeSelection* g_selection;
extern GtkTreeModel* g_model;
extern GtkIconTheme* theme;
extern GdkPixbuf* folder_icon;
extern GdkPixbuf* star_icon;


/* selected path in tree view */
extern GtkTreePath* g_selected_path;

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
get_bookmark_from_row(GtkTreePath*); 

/* -- read database -- */
void 
read_database(GtkWidget*, GtkWidget*); 

#endif

