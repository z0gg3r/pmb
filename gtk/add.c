#include "add.h"

static void 
add_bookmark(GtkWidget* button, gpointer** args) 
{
	char* name 	= NULL;
	char* url 	= NULL;
	char* comment 	= "None";
	char* tag 	= "None";

	if(gtk_entry_get_text_length(GTK_ENTRY(args[0])))
		name 	= (char*)gtk_entry_get_text(GTK_ENTRY(args[0]));

	if(gtk_entry_get_text_length(GTK_ENTRY(args[1])))
		url 	= (char*)gtk_entry_get_text(GTK_ENTRY(args[1]));

	if(gtk_entry_get_text_length(GTK_ENTRY(args[2])))
		comment	= (char*)gtk_entry_get_text(GTK_ENTRY(args[2]));

	if(gtk_entry_get_text_length(GTK_ENTRY(args[3])))
		tag 	= (char*)gtk_entry_get_text(GTK_ENTRY(args[3]));

	if(name && url) 
	{
		bookmark* b = bookmark_new(name, url, comment, tag);
		bookmark_db_write(b, db);
		bookmark_destroy(b);

		read_database(NULL, NULL);
		close_window(NULL, args[4]);
		g_free(args);

		read_database(NULL, NULL);
	}
}

void 
add_window(GtkWidget* button, gpointer main_window) 
{
	GtkWidget* 	window 	= dialogs("Add bookmark", main_window);
	GtkWidget** 	e 	= entries(TRUE);
	bookmark* 	b 	= get_data(NULL);

	if(b) 
	{
		if(strlen(bookmark_tag(b)) > 1)
			gtk_entry_set_text(GTK_ENTRY(e[7]), bookmark_tag(b));
		else if(bookmark_id(b))
			gtk_entry_set_text(GTK_ENTRY(e[7]), bookmark_id(b));

		free(b);
		b = NULL;
	}

	GtkWidget** add_bookmark_args = g_new(GtkWidget*, 5);
	add_bookmark_args[0] = e[1];
	add_bookmark_args[1] = e[3];
	add_bookmark_args[2] = e[5];
	add_bookmark_args[3] = e[7];
	add_bookmark_args[4] = window;

	/* button */
	GtkWidget* add_button = gtk_button_new_with_mnemonic("_Add");
	g_signal_connect(add_button, "clicked", G_CALLBACK(add_bookmark)
		,add_bookmark_args);

	GtkWidget* cancel_button = gtk_button_new_with_mnemonic("_Cancel");
	g_signal_connect(cancel_button, "clicked", G_CALLBACK(close_window)
		,window);

	/* tag box */
	GtkWidget* tag_box = tag_box_new();
	g_signal_connect(tag_box, "changed", G_CALLBACK(tag_entry_set_text)
		,e[7]); 

	/* grid */
	GtkWidget *grid = gtk_grid_new();
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
	gtk_grid_attach(GTK_GRID(grid), add_button 	,20, 5, 20, 10);
	gtk_grid_attach(GTK_GRID(grid), cancel_button 	,40, 5, 20, 10);

	g_free(e);
	gtk_container_add(GTK_CONTAINER(window), grid);
	gtk_widget_show_all(GTK_WIDGET(window));
}

