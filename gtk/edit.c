#include "edit.h"

static void
edit_bookmark(GtkWidget* button, gpointer** args) 
{
	char* name 	= (char*)gtk_entry_get_text(GTK_ENTRY(args[0]));
	char* url 	= (char*)gtk_entry_get_text(GTK_ENTRY(args[1]));
	char* comment 	= (char*)gtk_entry_get_text(GTK_ENTRY(args[2]));
	char* tag 	= (char*)gtk_entry_get_text(GTK_ENTRY(args[3]));

	if(url) 
	{
		bookmark_list* bl = bookmark_db_search(db, URL, url);
		
		if(bl) 
		{
			char** result = bookmark_list_return_next(bl);

			if(result[0]) 
			{
				int id = strtol(result[0], NULL, 10);

				if(name)
					bookmark_db_edit(db, id, 0, name);

				if(url)
					bookmark_db_edit(db, id, 1, url);

				if(comment)
					bookmark_db_edit(db, id, 2, comment);

				if(tag)
					bookmark_db_edit(db, id, 3, tag);

				read_database(NULL, NULL);
				free(result);
			}

			bookmark_list_destroy(bl);
		}
	}

	close_window(NULL, args[4]);
	g_free(args);
}

static void
edit_directory(GtkWidget* button, gpointer** args) 
{
	GtkTreeIter 	iter, child;
	directory_url*	dir_url	= directory_url_new();

	if(gtk_tree_model_get_iter(GTK_TREE_MODEL(model), &iter, selected_path))
	{
		if(gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(model), &child, &iter, 0))
			collect_directory_url(child, dir_url);
	}

	directory_url_position_first(dir_url);

	for(int i = 0; i < directory_url_size(dir_url) - 1; ++i)
	{
		char* 		url	= directory_url_next(dir_url);
		bookmark_list* 	bl 	= bookmark_db_search(db, URL, url);
		char** 		result	= NULL;
		char* 		tag 	= (char*)gtk_entry_get_text(GTK_ENTRY(args[0]));

		printf("--%s\n", url);

		if(bl) 
		{
			result = bookmark_list_return_next(bl);

			if(result[0]) 
			{
				int id = strtol(result[0], NULL, 10);
				bookmark_db_edit(db, id, 3, tag);
				free(result);
				read_database(NULL, NULL);
			}

			bookmark_list_destroy(bl);
		}
	}

	directory_url_destroy(dir_url);
	close_window(NULL, args[1]);
	g_free(args);
}

static void
edit_bookmark_window(bookmark* b, gpointer main_window) 
{
	GtkWidget* 	window 	= dialogs("Edit bookmark", main_window);
	GtkWidget** 	e 	= entries(TRUE);

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

	GtkWidget** edit_bookmark_args = g_new(GtkWidget*, 5);
	edit_bookmark_args[0] = e[1];
	edit_bookmark_args[1] = e[3];
	edit_bookmark_args[2] = e[5];
	edit_bookmark_args[3] = e[7];
	edit_bookmark_args[4] = window;

	/* button */
	GtkWidget* edit_button = gtk_button_new_with_mnemonic("_Edit");
	g_signal_connect(edit_button, "clicked", G_CALLBACK(edit_bookmark)
		,edit_bookmark_args);

	GtkWidget* cancel_button = gtk_button_new_with_mnemonic("_Cancel");
	g_signal_connect(cancel_button, "clicked", G_CALLBACK(close_window)
		,window);

	/* tag box */
	GtkWidget* tag_box = tag_box_new();
	g_signal_connect(tag_box, "changed", G_CALLBACK(tag_entry_set_text)
		,e[7]); 

	/* grid */
	GtkWidget* grid = gtk_grid_new();
	gtk_grid_set_column_spacing(GTK_GRID(grid), 2);
	gtk_grid_set_row_spacing(GTK_GRID(grid), 2);
	gtk_grid_set_column_homogeneous(GTK_GRID(grid), 1);

	gtk_grid_attach(GTK_GRID(grid), e[0] 		,0,  0, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), e[1] 		,20, 0, 50, 1);
	gtk_grid_attach(GTK_GRID(grid), e[2] 		,0,  1, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), e[3] 		,20, 1, 50, 1);
	gtk_grid_attach(GTK_GRID(grid), e[4] 		,0,  2, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), e[5] 		,20, 2, 50, 1);
	gtk_grid_attach(GTK_GRID(grid), e[6] 		,0,  3, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), e[7] 		,20, 3, 50, 1);
	gtk_grid_attach(GTK_GRID(grid), tag_box 	,20, 4, 50, 1);
	gtk_grid_attach(GTK_GRID(grid), edit_button 	,20, 5, 20, 10);
	gtk_grid_attach(GTK_GRID(grid), cancel_button 	,40, 5, 20, 10);

	g_free(e);
	gtk_container_add(GTK_CONTAINER(window), grid);
	gtk_widget_show_all(GTK_WIDGET(window));
}

static void
edit_directory_window(bookmark* b, gpointer main_window)
{
	GtkWidget* window = dialogs("Edit directory", main_window);

	GtkWidget* name_entry_label = gtk_label_new("Name");
	gtk_widget_set_halign(GTK_WIDGET(name_entry_label), GTK_ALIGN_START);

	GtkWidget* name_entry = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(name_entry), "name");

	if(b && bookmark_id(b))
	{
		gtk_entry_set_text(GTK_ENTRY(name_entry), bookmark_id(b));
		free(b);
		b = NULL;
	}

	GtkWidget** edit_directory_args = g_new(GtkWidget*, 2);
	edit_directory_args[0] = name_entry;
	edit_directory_args[1] = window;

	/* button */
	GtkWidget* edit_button = gtk_button_new_with_mnemonic("_Edit");
	g_signal_connect(edit_button, "clicked", G_CALLBACK(edit_directory)
		,edit_directory_args);

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
	gtk_grid_attach(GTK_GRID(grid), edit_button 		,20, 5, 20, 10);
	gtk_grid_attach(GTK_GRID(grid), cancel_button 		,40, 5, 20, 10);

	gtk_container_add(GTK_CONTAINER(window), grid);
	gtk_widget_show_all(GTK_WIDGET(window));
}

void
edit(GtkWidget* button, gpointer main_window)
{
	bookmark* b = get_data(NULL);

	if(b)
	{
		if(strlen(bookmark_url(b)) > 1)
			edit_bookmark_window(b, main_window);
		else
			edit_directory_window(b, main_window);
	}
}

