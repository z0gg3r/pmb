#include "delete.h"

static void
delete_bookmark(GtkWidget* button, gpointer** args) 
{
	char*  url 	= (char*)gtk_entry_get_text(GTK_ENTRY(args[0]));
	char** result	= NULL;

	if(url) 
	{
		bookmark_list* bl = bookmark_db_search(db, URL, url);
		
		if(bl) 
		{
			result = bookmark_list_return_next(bl);

			if(result[0]) 
			{
				int id = strtol(result[0], NULL, 10);
				bookmark_db_delete(db, id);
				read_database(NULL, NULL);
				free(result);
			}

			bookmark_list_destroy(bl);
		}
	}

	close_window(NULL, args[1]);
	g_free(args);
}

static void
delete_directory(GtkWidget* button, gpointer window)
{
	GtkTreeIter 	iter, child;
	int		size		= 1;
	char**		urls 		= calloc(size, sizeof(char*));
	int		index 		= 0;

	if(gtk_tree_model_get_iter(GTK_TREE_MODEL(model), &iter, selected_path))
	{
		if(gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(model), &child, &iter, 0))
			collect_directory_url(child, urls, &index, &size);
	}

	for(int i = 0; i < size - 1; ++i)
	{
		bookmark_list* 	bl 	= bookmark_db_search(db, URL, urls[i]);
		char** 		result	= NULL;

		if(bl) 
		{
			result = bookmark_list_return_next(bl);

			if(result[0]) 
			{
				int id = strtol(result[0], NULL, 10);
				bookmark_db_delete(db, id);
				free(result);
			}

			bookmark_list_destroy(bl);
		}

		if(urls[i])
			free(urls[i]);
	}

	if(urls)
		free(urls);

	gtk_tree_store_remove(GTK_TREE_STORE(bookmarks), &iter);
	close_window(NULL, window);
}

static void
delete_bookmark_window(bookmark* b, gpointer main_window) 
{
	GtkWidget* 	window 	= dialogs("Delete bookmark", main_window);
	GtkWidget** 	e 	= entries(FALSE);

	if(b) 
	{
		if(bookmark_name(b))
			gtk_entry_set_text(GTK_ENTRY(e[1]), bookmark_name(b));

		if(bookmark_url(b))
			gtk_entry_set_text(GTK_ENTRY(e[3]), bookmark_url(b));

		if(bookmark_comment(b))
			gtk_entry_set_text(GTK_ENTRY(e[5])
				,bookmark_comment(b));

		if(bookmark_tag(b))
			gtk_entry_set_text(GTK_ENTRY(e[7]), bookmark_tag(b));

		free(b);
		b = NULL;
	}

	GtkWidget** delete_bookmark_args = g_new(GtkWidget*, 2);
	delete_bookmark_args[0] = e[3];
	delete_bookmark_args[1] = window;

	/* button */
	GtkWidget* delete_button = gtk_button_new_with_mnemonic("_Delete");
	g_signal_connect(delete_button, "clicked", G_CALLBACK(delete_bookmark)
		,delete_bookmark_args);

	GtkWidget* cancel_button = gtk_button_new_with_mnemonic("_Cancel");
	g_signal_connect(cancel_button, "clicked", G_CALLBACK(close_window)
		,window);

	/* grid */
	GtkWidget* grid = gtk_grid_new();
	gtk_grid_set_column_spacing(GTK_GRID(grid), 2);
	gtk_grid_set_row_spacing(GTK_GRID(grid), 2);
	gtk_grid_set_column_homogeneous(GTK_GRID(grid), 1);

	gtk_grid_attach(GTK_GRID(grid), e[0] 		,0,  0, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), e[1]		,20, 0, 50, 1);
	gtk_grid_attach(GTK_GRID(grid), e[2] 		,0,  1, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), e[3]		,20, 1, 50, 1);
	gtk_grid_attach(GTK_GRID(grid), e[4] 		,0,  2, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), e[5]		,20, 2, 50, 1);
	gtk_grid_attach(GTK_GRID(grid), e[6] 		,0,  3, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), e[7]		,20, 3, 50, 1);
	gtk_grid_attach(GTK_GRID(grid), delete_button	,20, 4, 20, 10);
	gtk_grid_attach(GTK_GRID(grid), cancel_button 	,40, 4, 20, 10);

	g_free(e);
	gtk_container_add(GTK_CONTAINER(window), grid);
	gtk_widget_show_all(GTK_WIDGET(window));
	gtk_widget_grab_focus(GTK_WIDGET(cancel_button));
}

static void
delete_directory_window(bookmark* b, gpointer main_window)
{
	GtkWidget* window = dialogs("Delete directory", main_window);

	GtkWidget* name_entry_label = gtk_label_new("Name");
	gtk_widget_set_halign(GTK_WIDGET(name_entry_label), GTK_ALIGN_START);

	GtkWidget* name_entry = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(name_entry), "name");
	gtk_editable_set_editable(GTK_EDITABLE(name_entry), FALSE);

	if(b && bookmark_id(b))
	{
		gtk_entry_set_text(GTK_ENTRY(name_entry), bookmark_id(b));
		free(b);
		b = NULL;
	}

	/* button */
	GtkWidget* delete_button = gtk_button_new_with_mnemonic("_Delete");
	g_signal_connect(delete_button, "clicked", G_CALLBACK(delete_directory)
		,window);

	GtkWidget* cancel_button = gtk_button_new_with_mnemonic("_Cancel");
	g_signal_connect(cancel_button, "clicked", G_CALLBACK(close_window)
		,window);

	/* grid */
	GtkWidget* grid = gtk_grid_new();
	gtk_grid_set_column_spacing(GTK_GRID(grid), 2);
	gtk_grid_set_row_spacing(GTK_GRID(grid), 2);
	gtk_grid_set_column_homogeneous(GTK_GRID(grid), 1);

	gtk_grid_attach(GTK_GRID(grid), name_entry_label 	,0,  0, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), name_entry 		,20, 0, 50, 1);
	gtk_grid_attach(GTK_GRID(grid), delete_button 		,20, 5, 20, 10);
	gtk_grid_attach(GTK_GRID(grid), cancel_button 		,40, 5, 20, 10);

	gtk_container_add(GTK_CONTAINER(window), grid);
	gtk_widget_show_all(GTK_WIDGET(window));
	gtk_widget_grab_focus(GTK_WIDGET(cancel_button));
}

void
delete(GtkWidget* button, gpointer main_window)
{
	bookmark* b = get_data(NULL);

	if(b)
	{
		if(strlen(bookmark_url(b)) > 1)
			delete_bookmark_window(b, main_window);
		else
			delete_directory_window(b, main_window);
	}
}
