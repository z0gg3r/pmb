#include "gtk/gpmb.h"
#include "gtk/treeview.h"
#include "gtk/keypress.h"
#include "gtk/menubar.h"
#include "gtk/toolbox.h"
#include "gtk/dialog.h"
#include "gtk/add.h"
#include "gtk/delete.h"
#include "gtk/edit.h"
#include "gtk/options.h"

/* globals */
gpmb_options* 		opts 		= NULL;
sqlite3* 		db 		= NULL;
GtkTreeModel* 		model 		= NULL;
GtkTreeStore* 		bookmarks	= NULL;

/* selected path and column in tree view */
GtkTreePath* 		selected_path 	= NULL;
GtkTreeViewColumn* 	selected_column = NULL;

int 
main(int argc, char *argv[]) 
{
	const char* 	home = secure_getenv("HOME");	
	char* 		file = calloc((strlen(home) + strlen(DATABASE)
				 + 2), sizeof(char));

	if(file) 
	{
		snprintf(file, strlen(file) - 1, "%s/%s", home
			,DATABASE);

		db = bookmark_db_open(file);
		free(file);
	}
	else
		exit(EXIT_FAILURE);

	read_options();
	short res = gtk_interface(argc, argv);

	free(opts);
	bookmark_db_close(db);
	return res;
}

void 
destroy() 
{
	gtk_main_quit();
}

void 
read_options() 
{
	opts = malloc(sizeof(gpmb_options));

	if(opts) 
	{
		opts->id_fg 		= "#00ff00";
		opts->name_fg 		= "#ff0000";
		opts->url_fg 		= "#0000ff";
		opts->comment_fg 	= "#ff00ff";
		opts->tag_fg 		= "#f0a243";
	}
}

void
copy_to_clipboard()
{
	GtkClipboard* 	clip	= gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
	GtkClipboard* 	primary	= gtk_clipboard_get(GDK_SELECTION_PRIMARY);
	bookmark* 	b 	= get_data(NULL);
	char*		url	= bookmark_url(b);

	if(url)
	{
		gtk_clipboard_set_text(clip, url, -1);
		gtk_clipboard_set_text(primary, url, -1);
	}

	bookmark_destroy(b);
}

GtkWidget*
tag_box_new()
{
	GtkWidget* 	tag_box 	= gtk_combo_box_text_new();
	bookmark_list*	bl		= bookmark_db_query(db, 0, NULL);
	bookmark*	b		= NULL;
	char*		last_tag	= NULL;

	while((b = bookmark_list_return_next_bookmark(bl)))
	{
		char* tag = bookmark_tag(b);

		if((last_tag && strcmp(last_tag, tag)) 
		|| (!last_tag))
		{
			last_tag = bookmark_tag(b);

			gtk_combo_box_text_append
				(GTK_COMBO_BOX_TEXT(tag_box)
					,NULL
					,last_tag);
		}
	}

	bookmark_list_destroy(bl);
	
	GtkTreeIter iter;	
	GtkTreeModel* tag_model = gtk_combo_box_get_model(GTK_COMBO_BOX(tag_box));
	gtk_tree_model_get_iter_first(GTK_TREE_MODEL(tag_model), &iter);
	gtk_combo_box_set_active_iter(GTK_COMBO_BOX(tag_box), &iter);

	return tag_box;
}

GtkWidget*
search_box_new(GtkWidget* search_entry)
{
	/* search entry */
	GtkWidget* search_entry_label = gtk_label_new("Search:");
	gtk_widget_set_hexpand(GTK_WIDGET(search_entry), 1);

	/* search button */
	GtkWidget* search_button = gtk_button_new_with_mnemonic("_Go");

	GtkWidget** read_database_args = g_new(GtkWidget*, 1);
	read_database_args[0] = search_entry;

	g_signal_connect(GTK_WIDGET(search_button), "clicked"
		,G_CALLBACK(read_database), read_database_args);

	g_signal_connect(GTK_WIDGET(search_entry), "activate"
		,G_CALLBACK(read_database), read_database_args);

	/* search box */
	GtkWidget* search_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
	gtk_container_add(GTK_CONTAINER(search_box), search_entry_label);
	gtk_container_add(GTK_CONTAINER(search_box), search_entry);
	gtk_container_add(GTK_CONTAINER(search_box), search_button);

	return search_box;
}

void
search_entry_key_press(GtkWidget* window, GdkEventKey* e, GtkWidget* s_window)
{
	if(!strcmp((gdk_keyval_name(e->keyval)), "Escape")) 
		gtk_widget_grab_focus(GTK_WIDGET(s_window));
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

	/* tree store */
	bookmarks = gtk_tree_store_new(
		6
		,G_TYPE_STRING		//bookmark id
		,G_TYPE_STRING		//bookmark name
		,G_TYPE_STRING		//bookmark url
		,G_TYPE_STRING		//bookmark comment
		,G_TYPE_STRING		//tag
		,GDK_TYPE_PIXBUF	//directory icon
		,G_TYPE_STRING		//directory tag
		,-1);	

	/* tree view */
	GtkWidget* search_entry	= gtk_entry_new();
	GtkWidget* t_view 	= tree_view(search_entry);

	/* scrolled window for bookmark_view */
	GtkWidget* s_window 	= gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add(GTK_CONTAINER(s_window), t_view);

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

	/* read database and append to tree */
	read_database(NULL, NULL);

	/* add v_box into window */
	gtk_container_add(GTK_CONTAINER(bookmark_window), v_box);
	gtk_widget_show_all(bookmark_window);

	/* focus tree view */
	gtk_widget_grab_focus(GTK_WIDGET(t_view));

	GtkTreeIter iter;
	gtk_tree_model_get_iter_first(GTK_TREE_MODEL(model), &iter);
	GtkTreePath* path = gtk_tree_model_get_path(GTK_TREE_MODEL(model)
		,&iter);
	gtk_tree_view_set_cursor(GTK_TREE_VIEW(t_view), path, NULL, 0);

	/* sink */
	g_object_unref(bookmarks);

	gtk_main();
	return 0;
}

