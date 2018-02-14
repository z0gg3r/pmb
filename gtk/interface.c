#include "interface.h"

/* globals */
sqlite3* 		db 		= NULL;
GtkWidget*		main_box	= NULL;
GtkWidget*		info_label	= NULL;
GtkWidget*		spinner		= NULL;

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
	treeview 		= tree_view(search_entry);

	/* scrolled window for bookmark_view */
	GtkWidget* s_window 	= gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add(GTK_CONTAINER(s_window), treeview);

	/* key press */
	GtkWidget** 	key_press_args 	= g_new(GtkWidget*, 4);
	key_press_args[0] = treeview;
	key_press_args[1] = s_window;
	key_press_args[2] = search_entry;
	key_press_args[3] = bookmark_window;

	g_signal_connect(s_window, "key-press-event"
		,G_CALLBACK(key_press), key_press_args);

	g_signal_connect(search_entry, "key-release-event"
		,G_CALLBACK(search_entry_key_press), treeview);

	/* menu bar */
	GtkWidget* menu_bar 	= menu_bar_new(bookmark_window);

	/* tool box */
	tool_box 		= tool_box_new(bookmark_window);

	/* search box */
	GtkWidget* search_box 	= search_box_new(search_entry);

	/* info label */
	info_label		= gtk_label_new(" ");
	gtk_label_set_xalign(GTK_LABEL(info_label), 1);

	/* spinner */
	spinner 		= gtk_spinner_new();

	/* info box */
	GtkWidget* info_box 	= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	gtk_box_pack_start(GTK_BOX(info_box), info_label, TRUE, TRUE, 1);
	gtk_container_add(GTK_CONTAINER(info_box), spinner);

	/* add to main */
	main_box 		= gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
	gtk_container_add(GTK_CONTAINER(main_box), menu_bar);
	gtk_container_add(GTK_CONTAINER(main_box), tool_box);
	gtk_container_add(GTK_CONTAINER(main_box), search_box);
	gtk_box_pack_start(GTK_BOX(main_box), s_window, TRUE, TRUE, 1);
	gtk_container_add(GTK_CONTAINER(main_box), info_box);

	/* add main_box into window */
	gtk_container_add(GTK_CONTAINER(bookmark_window), main_box);
	gtk_widget_show_all(bookmark_window);

	/* read database and append to tree */
	read_database(NULL, NULL);

	/* focus tree view */
	gtk_widget_grab_focus(GTK_WIDGET(treeview));

	gtk_main();
	return 0;
}

