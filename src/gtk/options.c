#include "options.h"

GtkWidget* 	id_fg 		= NULL;
GtkWidget* 	name_fg		= NULL;
GtkWidget* 	url_fg 		= NULL;
GtkWidget* 	comment_fg 	= NULL;
gpmb_options* 	opts 		= NULL;
char* 		config_file	= NULL;

void 
set_options() 
{

	const char* 	home 		= secure_getenv("HOME");	
	char*		conf_dir 	= ".config/pmb";
	char*		config_filename = "gpmb.conf";

	config_file 	= calloc(1, 
				(strlen(home) * sizeof(char)
				+ strlen(config_filename) * sizeof(char)
				+ strlen(conf_dir) * sizeof(char) + 3) * sizeof(char));

	snprintf(config_file, strlen(config_file) - 1, "%s/%s/%s", home
			,conf_dir, config_filename);

	opts = malloc(sizeof(gpmb_options));

	if(opts) 
	{
		opts->database_file	= database_file;
		opts->tree_lines	= "true";
		opts->id_fg 		= "#00ff00";
		opts->name_fg 		= "#FD4040";
		opts->url_fg 		= "#4094FD";
		opts->comment_fg 	= "#ff00ff";
		opts->tag_fg 		= "#f0a243";
	}
}

void
write_config()
{
	FILE* 	fp = fopen(config_file, "w");

	if(fp)
	{
		fprintf(fp, "database=%s\n", opts->database_file);
		fprintf(fp, "tree_lines=%s\n", opts->tree_lines);
		fprintf(fp, "id_fg=%s\n", opts->id_fg);
		fprintf(fp, "name_fg=%s\n", opts->name_fg);
		fprintf(fp, "url_fg=%s\n", opts->url_fg);
		fprintf(fp, "comment_fg=%s\n", opts->comment_fg);
		fclose(fp);
	}
}

void
read_config() 
{
	set_options();

	int 	st;
	FILE* 	fp = fopen(config_file, "r");

	if(fp)
	{
		int 	size 	   = 1;
		char*	option 	   = calloc(size, sizeof(char));
		char*	option_bkp = strdup(option);

		while(!feof(fp))
		{
			st = fgetc(fp);

			if(st == '\n')
			{
				if(!(strcmp(option, "\0")) 	/* blank line */
				||(option[0] == ' ')
				||(option[0] == '\t')
				||(option[0] == '#'))		/* comment */
					goto new_option;

				char* str = strsep(&option, "=");

				if(!(strcmp(str, "database")))
					opts->database_file 	= strsep(&option, "=");

				else if(!(strcmp(str, "tree_lines")))
					opts->tree_lines 	= strsep(&option, "=");

				else if(!(strcmp(str, "id_fg")))
					opts->id_fg 		= strsep(&option, "=");

				else if(!(strcmp(str, "name_fg")))
					opts->name_fg 		= strsep(&option, "=");

				else if(!(strcmp(str, "url_fg")))
					opts->url_fg 		= strsep(&option, "=");

				else if(!(strcmp(str, "comment_fg")))
					opts->comment_fg 	= strsep(&option, "=");

				else
				{
					printf("unknown option: %s\n", str);
					//exit(EXIT_FAILURE);
				}

				new_option:
				free(option_bkp);
				size 		= 1;
				option 		= calloc(size, sizeof(char));
				option_bkp 	= strdup(option);
			}
			else
			{
				option[size - 1] = st;
				option 		 = realloc(option, ++size * sizeof(char));
				option[size - 1] = '\0';
				option_bkp	 = strdup(option);
			}
		}

		free(option_bkp);
		fclose(fp);
	}
	else
		write_config();
}

static void
set_colors()
{
	GdkRGBA color;

	gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(id_fg), &color);
	opts->id_fg 		= gdk_rgba_to_string(&color);
	g_object_set(cell_renderer_id, "foreground", opts->id_fg, NULL);

	gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(name_fg), &color);
	opts->name_fg 		= gdk_rgba_to_string(&color);
	g_object_set(cell_renderer_name, "foreground", opts->name_fg, NULL);

	gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(url_fg), &color);
	opts->url_fg 		= gdk_rgba_to_string(&color);
	g_object_set(cell_renderer_url, "foreground", opts->url_fg, NULL);

	gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(comment_fg), &color);
	opts->comment_fg 	= gdk_rgba_to_string(&color);
	g_object_set(cell_renderer_comment, "foreground", opts->comment_fg, NULL);

	read_database(NULL, NULL);
}

static void
apply_settings(GtkWidget* button)
{
	set_colors();
	write_config();
}

static GtkWidget*
color_page()
{
	GdkRGBA id;
	GdkRGBA name;
	GdkRGBA url;
	GdkRGBA comment;

	gdk_rgba_parse(&id, opts->id_fg);
	gdk_rgba_parse(&name, opts->name_fg);
	gdk_rgba_parse(&url, opts->url_fg);
	gdk_rgba_parse(&comment, opts->comment_fg);

	id_fg 		= gtk_color_button_new_with_rgba(&id);
	name_fg		= gtk_color_button_new_with_rgba(&name);
	url_fg 		= gtk_color_button_new_with_rgba(&url);
	comment_fg 	= gtk_color_button_new_with_rgba(&comment);

	GtkWidget* id_label		= gtk_label_new("Dir / Id");
	GtkWidget* name_label		= gtk_label_new("Name");
	GtkWidget* url_label		= gtk_label_new("Url");
	GtkWidget* comment_label	= gtk_label_new("Comment");

	GtkWidget* grid = grid_new();

	gtk_grid_attach(GTK_GRID(grid), id_label 	,0,  0, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), id_fg 		,40, 0, 50, 1);
	gtk_grid_attach(GTK_GRID(grid), name_label 	,0,  1, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), name_fg 	,40, 1, 50, 1);
	gtk_grid_attach(GTK_GRID(grid), url_label	,0,  2, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), url_fg 		,40, 2, 50, 1);
	gtk_grid_attach(GTK_GRID(grid), comment_label 	,0,  3, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), comment_fg 	,40, 3, 50, 1);

	return grid;
}

static GtkWidget*
settings_page()
{
	/* database entry */
	GtkWidget* database_label = gtk_label_new("Database");
	//gtk_widget_set_halign(GTK_WIDGET(database_label), GTK_ALIGN_START);

	GtkWidget* database_file_entry = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(database_file_entry), "database...");
	gtk_entry_set_text(GTK_ENTRY(database_file_entry), opts->database_file);

	/* tree lines */
	GtkWidget* tree_lines	= gtk_check_button_new_with_label("Tree lines");

	if(gtk_tree_view_get_enable_tree_lines(GTK_TREE_VIEW(treeview)))
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tree_lines), TRUE);

	GtkWidget* grid 	= grid_new();

	gtk_grid_attach(GTK_GRID(grid), database_label 		,0,  0, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), database_file_entry	,40, 0, 50, 1);
	gtk_grid_attach(GTK_GRID(grid), tree_lines		,0,  1, 50, 1);

	return grid;
}

void
options_window(GtkWidget* button) 
{
	GtkWidget* window 		= dialogs("Options", gpmb_window);

	/* settings page */
	GtkWidget* page_settings_l 	= gtk_label_new("Settings");
	GtkWidget* page_settings_b 	= gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
	GtkWidget* settings_grid	= settings_page();
	gtk_container_add(GTK_CONTAINER(page_settings_b), settings_grid);

	/* color page */
	GtkWidget* page_color_l 	= gtk_label_new("Colors");
	GtkWidget* page_color_b 	= gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
	GtkWidget* color_grid 		= color_page();
	gtk_container_add(GTK_CONTAINER(page_color_b), color_grid);

	/* buttons */
	GtkWidget* apply_button 	= gtk_button_new_with_mnemonic("_Apply");
	g_signal_connect(GTK_WIDGET(apply_button), "clicked", G_CALLBACK(apply_settings)
			,NULL);

	GtkWidget* cancel_button	= gtk_button_new_with_mnemonic("_Cancel");
	g_signal_connect(GTK_WIDGET(cancel_button), "clicked", G_CALLBACK(close_window)
		,window);

	/* button box */
	GtkWidget* button_box  		= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	gtk_box_pack_end(GTK_BOX(button_box), cancel_button, FALSE, FALSE, 1);
	gtk_box_pack_end(GTK_BOX(button_box), apply_button, FALSE, FALSE, 1);

	/* main box */
	GtkWidget* main_box  		= gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);

	/* notebook */
	GtkWidget* notebook 		= gtk_notebook_new();
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page_settings_b
				,page_settings_l);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page_color_b
				,page_color_l);

	gtk_box_pack_start(GTK_BOX(main_box), notebook, TRUE, TRUE, 1);
	gtk_box_pack_end(GTK_BOX(main_box), button_box, FALSE, FALSE, 1);
	gtk_container_add(GTK_CONTAINER(window), main_box);

	gtk_widget_show_all(GTK_WIDGET(window));
	gtk_spinner_start(GTK_SPINNER(spinner));
}

