#include "options.h"

void 
read_options() 
{
	opts = malloc(sizeof(gpmb_options));

	if(opts) 
	{
		opts->id_fg 		= "#00ff00";
		opts->name_fg 		= "#FD4040";
		opts->url_fg 		= "#4094FD";
		opts->comment_fg 	= "#ff00ff";
		opts->tag_fg 		= "#f0a243";
	}
}

void
options_window(GtkWidget* button, gpointer main_window) 
{
	GtkWidget* window = dialogs("Options", main_window);

	/* page 1 */
	GtkWidget* page_color_l = gtk_label_new("Colors");
	GtkWidget* page_color_b = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);

	GdkRGBA id;
	GdkRGBA name;
	GdkRGBA url;
	GdkRGBA comment;

	gdk_rgba_parse(&id, opts->id_fg);
	gdk_rgba_parse(&name, opts->name_fg);
	gdk_rgba_parse(&url, opts->url_fg);
	gdk_rgba_parse(&comment, opts->comment_fg);

	GtkWidget* id_fg 	= gtk_color_button_new_with_rgba(&id);
	GtkWidget* name_fg	= gtk_color_button_new_with_rgba(&name);
	GtkWidget* url_fg 	= gtk_color_button_new_with_rgba(&url);
	GtkWidget* comment_fg 	= gtk_color_button_new_with_rgba(&comment);

	GtkWidget* id_label		= gtk_label_new("Dir / Id");
	GtkWidget* name_label		= gtk_label_new("Name");
	GtkWidget* url_label		= gtk_label_new("Url");
	GtkWidget* comment_label	= gtk_label_new("Comment");

	GtkWidget* color_grid = gtk_grid_new();
	gtk_grid_set_column_spacing(GTK_GRID(color_grid), 2);
	gtk_grid_set_row_spacing(GTK_GRID(color_grid), 2);
	gtk_grid_set_column_homogeneous(GTK_GRID(color_grid), 1);

	gtk_grid_attach(GTK_GRID(color_grid), id_label 		,0,  0, 30, 1);
	gtk_grid_attach(GTK_GRID(color_grid), id_fg 		,40, 0, 50, 1);
	gtk_grid_attach(GTK_GRID(color_grid), name_label 	,0,  1, 30, 1);
	gtk_grid_attach(GTK_GRID(color_grid), name_fg 		,40, 1, 50, 1);
	gtk_grid_attach(GTK_GRID(color_grid), url_label		,0,  2, 30, 1);
	gtk_grid_attach(GTK_GRID(color_grid), url_fg 		,40, 2, 50, 1);
	gtk_grid_attach(GTK_GRID(color_grid), comment_label 	,0,  3, 30, 1);
	gtk_grid_attach(GTK_GRID(color_grid), comment_fg 	,40, 3, 50, 1);

	//gtk_grid_attach(GTK_GRID(color_grid), edit_button 	,20, 5, 20, 10);
	//gtk_grid_attach(GTK_GRID(color_grid), cancel_button 	,40, 5, 20, 10);

	gtk_container_add(GTK_CONTAINER(page_color_b), color_grid);

	/* page 2 */
	GtkWidget* page_2_label = gtk_label_new("page 2");
	GtkWidget* page_2_box 	= gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);

	GtkWidget* test_2_button = gtk_button_new_with_label("test 2");

	gtk_container_add(GTK_CONTAINER(page_2_box), test_2_button);

	/* notebook */
	GtkWidget* notebook = gtk_notebook_new();
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page_color_b, page_color_l);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page_2_box, page_2_label);

	gtk_container_add(GTK_CONTAINER(window), notebook);
	gtk_widget_show_all(GTK_WIDGET(window));
}

