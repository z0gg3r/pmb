#include "interface.h"

/* globals */
gpmb_options* 		opts 		= NULL;
sqlite3* 		db 		= NULL;
GtkTreeModel* 		model 		= NULL;
GtkTreeStore* 		bookmarks	= NULL;

/* selected path and column in tree view */
GtkTreePath* 		selected_path 	= NULL;
GtkTreeViewColumn* 	selected_column = NULL;

void 
destroy() 
{
	gtk_main_quit();
}

int 
gtk_interface(int argc, char* argv[]) 
{
	gtk_init(&argc, &argv);

	/* set up the main window */
	GtkWidget* bookmark_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(bookmark_window), "Bookmarks");
	gtk_window_set_role(GTK_WINDOW(bookmark_window), "Bookmarks");
	gtk_window_set_default_size(GTK_WINDOW(bookmark_window)
		,800, 600);

	g_signal_connect(bookmark_window, "destroy", G_CALLBACK(destroy)
		,NULL);

	/* tree view */
	GtkWidget* search_entry	= gtk_entry_new();
	GtkWidget* t_view 	= tree_view(search_entry);

	/* scrolled window for bookmark_view */
	GtkWidget* s_window 	= gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add(GTK_CONTAINER(s_window), t_view);

	/* key press */
	GtkWidget** 	key_press_args 	= g_new(GtkWidget*, 4);
	key_press_args[0] = t_view;
	key_press_args[1] = s_window;
	key_press_args[2] = search_entry;
	key_press_args[3] = bookmark_window;

	g_signal_connect(s_window, "key-press-event"
		,G_CALLBACK(key_press), key_press_args);

	g_signal_connect(search_entry, "key-release-event"
		,G_CALLBACK(search_entry_key_press), t_view);

	/* menu bar */
	GtkWidget* menu_bar 	= menu_bar_new(bookmark_window);

	/* tool box */
	GtkWidget* tool_box 	= tool_box_new(bookmark_window);

	/* search box */
	GtkWidget* search_box 	= search_box_new(search_entry);

	/* add to v_box */
	GtkWidget* v_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
	gtk_container_add(GTK_CONTAINER(v_box), menu_bar);
	gtk_container_add(GTK_CONTAINER(v_box), tool_box);
	gtk_container_add(GTK_CONTAINER(v_box), search_box);
	gtk_box_pack_start(GTK_BOX(v_box), s_window, TRUE, TRUE, 1);

	/* add v_box into window */
	gtk_container_add(GTK_CONTAINER(bookmark_window), v_box);
	gtk_widget_show_all(bookmark_window);

	/* read database and append to tree */
	read_database(NULL, NULL);

	/* focus tree view */
	gtk_widget_grab_focus(GTK_WIDGET(t_view));

	/* focus n the first tree view item */
	GtkTreeIter iter;
	gtk_tree_model_get_iter_first(GTK_TREE_MODEL(model), &iter);

	GtkTreePath* path = gtk_tree_model_get_path(GTK_TREE_MODEL(model)
		,&iter);
	gtk_tree_view_set_cursor(GTK_TREE_VIEW(t_view), path, NULL, 0);
	gtk_tree_path_free(path);

	gtk_main();
	return 0;
}

