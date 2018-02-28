#include "options.h"

gpmb_options* opts = NULL;
char* config_file = NULL;

void 
set_options() 
{
  const char* home = secure_getenv("HOME");	
  char*	conf_dir = ".config/pmb";
  char*	config_filename = "gpmb.conf";

  config_file = calloc
    (1, (strlen(home) + strlen(config_filename)
	 + strlen(conf_dir) + 2) * sizeof(char));

  snprintf(config_file, strlen(config_file) - 1, "%s/%s/%s", home
	   ,conf_dir, config_filename);

  opts = malloc(sizeof(gpmb_options));

  if(opts) 
    {
      opts->database_file = g_database_file;
      opts->tree_lines = "true";
      opts->download_favicon = "false";
      opts->id_fg = NULL;
      opts->name_fg = NULL;
      opts->url_fg = NULL;
      opts->comment_fg = NULL;
      opts->tag_fg = NULL;
      opts->id_font = NULL;
      opts->name_font = NULL;
      opts->url_font = NULL;
      opts->comment_font = NULL;
    }
}

void
write_config()
{
  FILE* fp = fopen(config_file, "w");

  if(fp)
    {
      if(opts->database_file)
	{
	  fprintf(fp, "database=%s\n", opts->database_file);
	}
		
      if(opts->tree_lines)
	{
	  fprintf(fp, "tree_lines=%s\n", opts->tree_lines);
	}

      if(opts->download_favicon)
	{
	  fprintf(fp, "download_favicon=%s\n", opts->download_favicon);
	}
	    
      if(opts->id_fg)
	{
	  fprintf(fp, "id_fg=%s\n", opts->id_fg);
	}

      if(opts->name_fg)
	{
	  fprintf(fp, "name_fg=%s\n", opts->name_fg);
	}

      if(opts->url_fg)
	{
	  fprintf(fp, "url_fg=%s\n", opts->url_fg);
	}

      if(opts->comment_fg)
	{
	  fprintf(fp, "comment_fg=%s\n", opts->comment_fg);
	}

      if(opts->id_font)
	{
	  fprintf(fp, "id_font=%s\n", opts->id_font);
	}

      if(opts->name_font)
	{
	  fprintf(fp, "name_font=%s\n", opts->name_font);
	}

      if(opts->url_font)
	{
	  fprintf(fp, "url_font=%s\n", opts->url_font);
	}

      if(opts->comment_font)
	{
	  fprintf(fp, "comment_font=%s\n", opts->comment_font);
	}

      fclose(fp);
    }
}

void
read_config() 
{
  set_options();

  int st;
  FILE* fp = fopen(config_file, "r");

  if(fp)
    {
      int size = 1;
      char* option = calloc(size, sizeof(char));
      char* option_bkp = strdup(option);

      while(!feof(fp))
	{
	  st = fgetc(fp);

	  if(st == '\n')
	    {
	      if(!(strcmp(option, "\0")) /* blank line */
		 ||(option[0] == ' ')
		 ||(option[0] == '\t')
		 ||(option[0] == '#')) /* comment */
		{
		  goto new_option;
		}

	      char* str = strsep(&option, "=");

	      if(!(strcmp(str, "database")))
		{
		  opts->database_file = strsep(&option, "=");
		}

	      else if(!(strcmp(str, "tree_lines")))
		{
		  opts->tree_lines = strsep(&option, "=");
		}
	      
	      else if(!(strcmp(str, "download_favicon")))
		{
		  opts->download_favicon = strsep(&option, "=");
		}

	      else if(!(strcmp(str, "id_fg")))
		{
		  opts->id_fg = strsep(&option, "=");
		}

	      else if(!(strcmp(str, "name_fg")))
		{
		  opts->name_fg = strsep(&option, "=");
		}

	      else if(!(strcmp(str, "url_fg")))
		{
		  opts->url_fg = strsep(&option, "=");
		}

	      else if(!(strcmp(str, "comment_fg")))
		{
		  opts->comment_fg = strsep(&option, "=");
		}

	      else if(!(strcmp(str, "id_font")))
		{
		  opts->id_font = strsep(&option, "=");
		}

	      else if(!(strcmp(str, "name_font")))
		{
		  opts->name_font = strsep(&option, "=");
		}

	      else if(!(strcmp(str, "url_font")))
		{
		  opts->url_font = strsep(&option, "=");
		}

	      else if(!(strcmp(str, "comment_font")))
		{
		  opts->comment_font = strsep(&option, "=");
		}

	      else
		{
		  printf("unknown option: %s\n", str);
		}

	    new_option:
	      free(option_bkp);
	      size = 1;
	      option = calloc(size, sizeof(char));
	      option_bkp = strdup(option);
	    }
	  else
	    {
	      option[size - 1] = st;
	      option = realloc(option, ++size * sizeof(char));
	      option[size - 1] = '\0';
	      option_bkp = strdup(option);
	    }
	}

      free(option_bkp);
      fclose(fp);
    }
  else
    {
      write_config();
    }
}

static void
apply_settings(GtkWidget* button)
{
  if(opts->id_fg)
    {
      g_object_set(g_cell_renderer_id, "foreground", opts->id_fg, NULL);
    }

  if(opts->name_fg)
    {
      g_object_set(g_cell_renderer_name, "foreground", opts->name_fg, NULL);
    }

  if(opts->url_fg)
    {
      g_object_set(g_cell_renderer_url, "foreground", opts->url_fg, NULL);
    }

  if(opts->comment_fg)
    {
      g_object_set(g_cell_renderer_comment, "foreground", opts->comment_fg
		   ,NULL);
    }

  if(opts->id_font)
    {
      g_object_set(g_cell_renderer_id, "font", opts->id_font, NULL);
    }

  if(opts->name_font)
    {
      g_object_set(g_cell_renderer_name, "font", opts->name_font, NULL);
    }

  if(opts->url_font)
    {
      g_object_set(g_cell_renderer_url, "font", opts->url_font, NULL);
    }

  if(opts->comment_font)
    {
      g_object_set(g_cell_renderer_comment, "font", opts->comment_font, NULL);
    }

  if(!(strcmp(opts->tree_lines, "true")))
    {
      gtk_tree_view_set_enable_tree_lines(GTK_TREE_VIEW(g_treeview), TRUE);
    }
  else
    {
      gtk_tree_view_set_enable_tree_lines(GTK_TREE_VIEW(g_treeview), FALSE);
    }

  write_config();
  read_database(NULL, NULL);
}

static void
select_font(GtkFontButton* button, gpointer name)
{
  if(!(strcmp(name, "id")))
    {
      opts->id_font = gtk_font_button_get_font_name
	(GTK_FONT_BUTTON(button));
    }

  if(!(strcmp(name, "name")))
    {
      opts->name_font = gtk_font_button_get_font_name
	(GTK_FONT_BUTTON(button));
    }

  if(!(strcmp(name, "url")))
    {
      opts->url_font = gtk_font_button_get_font_name
	(GTK_FONT_BUTTON(button));
    }

  if(!(strcmp(name, "comment")))
    {
      opts->comment_font = gtk_font_button_get_font_name
	(GTK_FONT_BUTTON(button));
    }
}

static void
select_color(GtkColorButton* button, gpointer name)
{
  GdkRGBA color;

  if(!(strcmp(name, "id")))
    {
      gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(button), &color);
      opts->id_fg = gdk_rgba_to_string(&color);
    }

  if(!(strcmp(name, "name")))
    {
      gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(button), &color);
      opts->name_fg = gdk_rgba_to_string(&color);
    }

  if(!(strcmp(name, "url")))
    {
      gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(button), &color);
      opts->url_fg = gdk_rgba_to_string(&color);
    }

  if(!(strcmp(name, "comment")))
    {
      gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(button), &color);
      opts->comment_fg = gdk_rgba_to_string(&color);
    }
}

static void
tree_lines_set(GtkToggleButton* button)
{
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button)))
    {
      opts->tree_lines = "true";
    }
  else
    {
      opts->tree_lines = "false";
    }
}

static void
favicons_set(GtkToggleButton* button)
{
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button)))
    {
      opts->download_favicon = "true";
    }
  else
    {
      opts->download_favicon = "false";
    }
}

static GtkWidget*
appearance_page()
{
  /* color buttons */
  GdkRGBA id;
  GdkRGBA name;
  GdkRGBA url;
  GdkRGBA comment;

  if(opts->id_fg)
    {
      gdk_rgba_parse(&id, opts->id_fg);
    }

  if(opts->name_fg)
    {
      gdk_rgba_parse(&name, opts->name_fg);
    }

  if(opts->url_fg)
    {
      gdk_rgba_parse(&url, opts->url_fg);
    }

  if(opts->comment_fg)
    {
      gdk_rgba_parse(&comment, opts->comment_fg);
    }

  GtkWidget* id_fg_button = gtk_color_button_new_with_rgba(&id);
  GtkWidget* name_fg_button = gtk_color_button_new_with_rgba(&name);
  GtkWidget* url_fg_button = gtk_color_button_new_with_rgba(&url);
  GtkWidget* comment_fg_button= gtk_color_button_new_with_rgba(&comment);

  g_signal_connect
    (GTK_WIDGET(id_fg_button), "color-set", G_CALLBACK(select_color)
     ,"id");

  g_signal_connect
    (GTK_WIDGET(name_fg_button), "color-set", G_CALLBACK(select_color)
     ,"name");

  g_signal_connect
    (GTK_WIDGET(url_fg_button), "color-set", G_CALLBACK(select_color)
     ,"url");

  g_signal_connect
    (GTK_WIDGET(comment_fg_button), "color-set", G_CALLBACK(select_color)
     ,"comment");

  /* font buttons */
  GtkWidget* id_font_button = gtk_font_button_new();
  GtkWidget* name_font_button = gtk_font_button_new();
  GtkWidget* url_font_button = gtk_font_button_new();
  GtkWidget* comment_font_button = gtk_font_button_new();

  if(opts->id_font)
    {
      gtk_font_button_set_font_name(GTK_FONT_BUTTON(id_font_button)
				    ,opts->id_font);
    }
  else
    {
      gtk_font_button_set_font_name(GTK_FONT_BUTTON(id_font_button)
				    ,"none");
    }

  if(opts->name_font)
    {
      gtk_font_button_set_font_name(GTK_FONT_BUTTON(name_font_button)
				    ,opts->name_font);
    }
  else
    {
      gtk_font_button_set_font_name(GTK_FONT_BUTTON(name_font_button)
				    ,"none");
    }

  if(opts->url_font)
    {
      gtk_font_button_set_font_name(GTK_FONT_BUTTON(url_font_button)
				    ,opts->url_font);
    }
  else
    {
      gtk_font_button_set_font_name(GTK_FONT_BUTTON(url_font_button)
				    ,"none");
    }

  if(opts->comment_font)
    {
      gtk_font_button_set_font_name(GTK_FONT_BUTTON(comment_font_button)
				    ,opts->comment_font);
    }
  else
    {
      gtk_font_button_set_font_name(GTK_FONT_BUTTON(comment_font_button)
				    ,"none");
    }

  g_signal_connect
    (GTK_WIDGET(id_font_button), "font-set", G_CALLBACK(select_font)
     ,"id");

  g_signal_connect
    (GTK_WIDGET(name_font_button), "font-set", G_CALLBACK(select_font)
     ,"name");

  g_signal_connect
    (GTK_WIDGET(url_font_button), "font-set", G_CALLBACK(select_font)
     ,"url");

  g_signal_connect
    (GTK_WIDGET(comment_font_button), "font-set", G_CALLBACK(select_font)
     ,"comment");

  GtkWidget* id_label = gtk_label_new("Dir / Id");
  GtkWidget* name_label= gtk_label_new("Name");
  GtkWidget* url_label = gtk_label_new("Url");
  GtkWidget* comment_label= gtk_label_new("Comment");

  GtkWidget* grid = grid_new();
  gtk_grid_attach(GTK_GRID(grid), id_label, 0,  0, 20, 1);
  gtk_grid_attach(GTK_GRID(grid), id_fg_button, 20, 0, 10, 1);
  gtk_grid_attach(GTK_GRID(grid), id_font_button, 30, 0, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), name_label, 0,  1, 20, 1);
  gtk_grid_attach(GTK_GRID(grid), name_fg_button, 20, 1, 10, 1);
  gtk_grid_attach(GTK_GRID(grid), name_font_button, 30, 1, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), url_label, 0,  2, 20, 1);
  gtk_grid_attach(GTK_GRID(grid), url_fg_button, 20, 2, 10, 1);
  gtk_grid_attach(GTK_GRID(grid), url_font_button, 30, 2, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), comment_label, 0,  3, 20, 1);
  gtk_grid_attach(GTK_GRID(grid), comment_fg_button, 20, 3, 10, 1);
  gtk_grid_attach(GTK_GRID(grid), comment_font_button, 30, 3, 30, 1);

  return grid;
}

static GtkWidget*
settings_page()
{
  /* database entry */
  GtkWidget* database_label = gtk_label_new("Database");
  //gtk_widget_set_halign(GTK_WIDGET(database_label), GTK_ALIGN_START);

  GtkWidget* database_file_entry = gtk_entry_new();
  gtk_entry_set_placeholder_text
    (GTK_ENTRY(database_file_entry), "database...");
  gtk_entry_set_text(GTK_ENTRY(database_file_entry), opts->database_file);

  /* tree lines */
  GtkWidget* tree_lines_button = gtk_check_button_new_with_label
    ("Show tree lines");
  g_signal_connect
    (GTK_WIDGET(tree_lines_button), "toggled", G_CALLBACK(tree_lines_set), NULL);

  if(gtk_tree_view_get_enable_tree_lines(GTK_TREE_VIEW(g_treeview)))
    {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tree_lines_button)
				   ,TRUE);
    }

  /* favicons */
  GtkWidget* favicon_button = gtk_check_button_new_with_label
    ("Download favicon");
  g_signal_connect
    (GTK_WIDGET(favicon_button), "toggled", G_CALLBACK(favicons_set), NULL);

  if(!strcmp(opts->download_favicon, "true"))
    {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(favicon_button), TRUE);
    }
  
  GtkWidget* grid = grid_new();
  gtk_grid_attach(GTK_GRID(grid), database_label, 0,  0, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), database_file_entry, 40, 0, 50, 1);
  gtk_grid_attach(GTK_GRID(grid), tree_lines_button, 0,  1, 25, 1);
  gtk_grid_attach(GTK_GRID(grid), favicon_button, 25,  1, 25, 1);
  
  return grid;
}

void
options_window(GtkWidget* button) 
{
  GtkWidget* window = dialogs("Options", gpmb_window);

  /* settings page */
  GtkWidget* page_settings_l = gtk_label_new("Settings");
  GtkWidget* page_settings_b = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
  GtkWidget* settings_grid = settings_page();
  gtk_container_add(GTK_CONTAINER(page_settings_b), settings_grid);

  /* appearance page */
  GtkWidget* page_appearance_l = gtk_label_new("Appearance");
  GtkWidget* page_appearance_b = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
  GtkWidget* appearance_grid = appearance_page();
  gtk_container_add(GTK_CONTAINER(page_appearance_b), appearance_grid);

  /* buttons */
  GtkWidget* apply_button = gtk_button_new_with_mnemonic("_Apply");
  g_signal_connect
    (GTK_WIDGET(apply_button), "clicked", G_CALLBACK(apply_settings)
     ,NULL);

  GtkWidget* cancel_button = gtk_button_new_with_mnemonic("_Cancel");
  g_signal_connect
    (GTK_WIDGET(cancel_button), "clicked", G_CALLBACK(close_window)
     ,window);

  /* button box */
  GtkWidget* button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
  gtk_box_pack_end(GTK_BOX(button_box), cancel_button, FALSE, FALSE, 1);
  gtk_box_pack_end(GTK_BOX(button_box), apply_button, FALSE, FALSE, 1);

  /* main box */
  GtkWidget* main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);

  /* notebook */
  GtkWidget* notebook = gtk_notebook_new();
  gtk_notebook_append_page
    (GTK_NOTEBOOK(notebook), page_settings_b, page_settings_l);
  
  gtk_notebook_append_page
    (GTK_NOTEBOOK(notebook), page_appearance_b, page_appearance_l);

  gtk_box_pack_start(GTK_BOX(main_box), notebook, TRUE, TRUE, 1);
  gtk_box_pack_end(GTK_BOX(main_box), button_box, FALSE, FALSE, 1);
  gtk_container_add(GTK_CONTAINER(window), main_box);

  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_spinner_start(GTK_SPINNER(g_spinner));
}
