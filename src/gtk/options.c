#include "options.h"

char* config_file = NULL;
char* name_options_l[NAME_OPTIONS_SIZE][3];

GtkWidget* id_fg_button = NULL;
GtkWidget* name_fg_button = NULL;
GtkWidget* url_fg_button = NULL;
GtkWidget* comment_fg_button = NULL;
GtkWidget* id_reset_button = NULL;
GtkWidget* name_reset_button = NULL;
GtkWidget* url_reset_button = NULL;
GtkWidget* comment_reset_button = NULL;
GtkWidget* id_font_button = NULL;
GtkWidget* name_font_button = NULL;
GtkWidget* url_font_button = NULL;
GtkWidget* comment_font_button = NULL;

GdkRGBA id_rgba;
GdkRGBA name_rgba;
GdkRGBA url_rgba;
GdkRGBA comment_rgba;

void 
set_options() 
{
  const char* home = secure_getenv("HOME");	
  char*	conf_dir = ".config/pmb";
  char*	config_filename = "gpmb.conf";
  
  unsigned int size = strlen(home)
    + strlen(conf_dir)
    + strlen(config_filename)
    + 3;
  
  config_file = calloc(size, sizeof(char));
  check_oom(config_file, "options > set_options - config_file");
  
  snprintf(config_file, size, "%s/%s/%s", home
	   ,conf_dir, config_filename);

  name_options_l[0][0] = "database=%s\n";
  name_options_l[0][1] = g_database_file;
  name_options_l[0][2] = "database";
  
  name_options_l[1][0] = "tree_lines=%s\n";
  name_options_l[1][1] = strdup("true");

  name_options_l[1][2] = "tree_lines";
  
  name_options_l[2][0] = "download_favicon=%s\n";
  name_options_l[2][1] = strdup("false");
  name_options_l[2][2] = "download_favicon";
  
  name_options_l[3][0] = "id_fg=%s\n";
  name_options_l[3][1] = NULL;
  name_options_l[3][2] = "id_fg";
  
  name_options_l[4][0] = "name_fg=%s\n";
  name_options_l[4][1] = NULL;
  name_options_l[4][2] = "name_fg";
  
  name_options_l[5][0] = "url_fg=%s\n";
  name_options_l[5][1] = NULL;
  name_options_l[5][2] = "url_fg";
  
  name_options_l[6][0] = "comment_fg=%s\n";
  name_options_l[6][1] = NULL;
  name_options_l[6][2] = "comment_fg";
  
  name_options_l[7][0] = "id_font=%s\n";
  name_options_l[7][1] = NULL;
  name_options_l[7][2] = "id_font";  
  
  name_options_l[8][0] = "name_font=%s\n";
  name_options_l[8][1] = NULL;
  name_options_l[8][2] = "name_font";
  
  name_options_l[9][0] = "url_font=%s\n";
  name_options_l[9][1] = NULL;
  name_options_l[9][2] = "url_font";
  
  name_options_l[10][0] = "comment_font=%s\n";
  name_options_l[10][1] = NULL;
  name_options_l[10][2] = "comment_font";
  
  name_options_l[11][0] = "scroll_up_key=%s\n";
  name_options_l[11][1] = strdup("k");
  name_options_l[11][2] = "scroll_up_key";
  
  name_options_l[12][0] = "scroll_down_key=%s\n";
  name_options_l[12][1] = strdup("j");
  name_options_l[12][2] = "scroll_down_key";
  
  name_options_l[13][0] = "scroll_left_key=%s\n";
  name_options_l[13][1] = strdup("h");
  name_options_l[13][2] = "scroll_left_key";
  
  name_options_l[14][0] = "scroll_right_key=%s\n";
  name_options_l[14][1] = strdup("l");
  name_options_l[14][2] = "scroll_right_key";
  
  name_options_l[15][0] = "scroll_top_key=%s\n";
  name_options_l[15][1] = strdup("g");;
  name_options_l[15][2] = "scroll_top_key";
  
  name_options_l[16][0] = "scroll_bottom_key=%s\n";
  name_options_l[16][1] = strdup("G");
  name_options_l[16][2] = "scroll_bottom_key";
  
  name_options_l[17][0] = "scroll_begin_row_key=%s\n";
  name_options_l[17][1] = strdup("0");
  name_options_l[17][2] = "scroll_begin_row_key";
  
  name_options_l[18][0] = "scroll_end_row_key=%s\n";
  name_options_l[18][1] = strdup("dollar");
  name_options_l[18][2] = "scroll_end_row_key";
  
  name_options_l[19][0] = "scroll_page_up_key=%s\n";
  name_options_l[19][1] = strdup("b");
  name_options_l[19][2] = "scroll_page_up_key";
  
  name_options_l[20][0] = "scroll_page_down_key=%s\n";
  name_options_l[20][1] = strdup("f");
  name_options_l[20][2] = "scroll_page_down_key";
  
  name_options_l[21][0] = "ec_key=%s\n";
  name_options_l[21][1] = strdup("x");
  name_options_l[21][2] = "ec_key";
  
  name_options_l[22][0] = "ea_key=%s\n";
  name_options_l[22][1] = strdup("X");
  name_options_l[22][2] = "ea_key";
  
  name_options_l[23][0] = "search_bar_key=%s\n";
  name_options_l[23][1] = strdup("s");
  name_options_l[23][2] = "search_bar_key";
  
  name_options_l[24][0] = "copy_key=%s\n";
  name_options_l[24][1] = strdup("c");
  name_options_l[24][2] = "copy_key";
  
  name_options_l[25][0] = "reload_key=%s\n";
  name_options_l[25][1] = strdup("r");
  name_options_l[25][2] = "reload_key";
  
  name_options_l[26][0] = "insert_key=%s\n";
  name_options_l[26][1] = strdup("i");
  name_options_l[26][2] = "insert_key";
  
  name_options_l[27][0] = "edit_key=%s\n";
  name_options_l[27][1] = strdup("e");
  name_options_l[27][2] = "edit_key";
  
  name_options_l[28][0] = "delete_key=%s\n";
  name_options_l[28][1] = strdup("d");
  name_options_l[28][2] = "delete_key";
  
  name_options_l[29][0] = "rename_key=%s\n";
  name_options_l[29][1] = strdup("n");
  name_options_l[29][2] = "rename_key";
  
  name_options_l[30][0] = "options_key=%s\n";
  name_options_l[30][1] = strdup("O");
  name_options_l[30][2] = "options_key";
  
  name_options_l[31][0] = "open_database_key=%s\n";
  name_options_l[31][1] = strdup("o");
  name_options_l[31][2] = "open_database_key";
}

void
destroy_options()
{
  for(int i = 0; i < NAME_OPTIONS_SIZE; i++)
    {
      if(name_options_l[i][1])
	{
	  free(name_options_l[i][1]);
	}
    }
}

void
write_config()
{
  FILE* fp = fopen(config_file, "w");

  if(fp)
    {
      for(int i = 0; i < NAME_OPTIONS_SIZE; ++i)
	{
	  if(name_options_l[i][1])
	    {
	      fprintf(fp, name_options_l[i][0], name_options_l[i][1]);
	    }
	}
      
      fclose(fp);
    }
}

void
read_config()
{
  set_options();

  FILE* fp = fopen(config_file, "r");
  
  if(fp)
    {
      char buffer[128];

      while(fgets(buffer, 128, fp))
	{
	  if((buffer[0] == '\0') 	/* blank line */
	     ||(buffer[0] == ' ')
	     ||(buffer[0] == '\t')
	     ||(buffer[0] == '\n')
	     ||(buffer[0] == '#'))	/* comment */
	    {
	      continue;
	    }
	  else
	    {
	      char* option;
	      char* option_bkp;
	      option = strdup(buffer);
	      option_bkp = option;
	      
	      char* str = strsep(&option, "=");
	      char* arg = strsep(&option, "=");		  
	      arg[strlen(arg) - 1] = '\0';

	      for(int i = 0; i < NAME_OPTIONS_SIZE; ++i)
		{
		  if(!(strcmp(str, name_options_l[i][2])))
		    {
		      if(name_options_l[i][1])
			{
			  free(name_options_l[i][1]);
			}

		      name_options_l[i][1] = strdup(arg);
		      break;
		    }
		}
	      
	      free(option_bkp);	      
	    }
	}

      fclose(fp);
    }
}

static void
select_font(GtkFontButton* button, gpointer name)
{
  for(int i = 0; i < NAME_OPTIONS_SIZE; ++i)
    {
      if(!(strcmp(name, name_options_l[i][2])))
	{
	  if(name_options_l[i][1])
	    {
	      free(name_options_l[i][1]);
	    }

	  GtkCellRenderer* renderer = NULL;

	  if(!(strcmp(name, "id_font")))
	    {
	      renderer = g_cell_renderer_id;
	    }
	  else if(!(strcmp(name, "name_font")))
	    {
	      renderer = g_cell_renderer_name;
	    }
	  else if(!(strcmp(name, "url_font")))
	    {
	      renderer = g_cell_renderer_url;
	    }
	  else if(!(strcmp(name, "comment_font")))
	    {
	      renderer = g_cell_renderer_comment;
	    }
	  
	  name_options_l[i][1] = strdup(gtk_font_button_get_font_name
				 (GTK_FONT_BUTTON(button)));
	  
	  g_object_set(renderer, "font", name_options_l[i][1], NULL);

	  break;
	}
    }
}

static void
select_color(GtkColorButton* button, gpointer name)
{
  GdkRGBA color;

  for(int i = 0; i < NAME_OPTIONS_SIZE; ++i)
    {
      if(!(strcmp(name, name_options_l[i][2])))
	{
	  if(name_options_l[i][1])
	    {
	      free(name_options_l[i][1]);
	    }

	  GtkCellRenderer* renderer = NULL;
	  
	  if(!(strcmp(name, "id_fg")))
	    {
	      renderer = g_cell_renderer_id;
	    }
	  else if(!(strcmp(name, "name_fg")))
	    {
	      renderer = g_cell_renderer_name;
	    }
	  else if(!(strcmp(name, "url_fg")))
	    {
	      renderer = g_cell_renderer_url;
	    }
	  else if(!(strcmp(name, "comment_fg")))
	    {
	      renderer = g_cell_renderer_comment;
	    }
	  
	  gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(button), &color);
	  name_options_l[i][1] = strdup(gdk_rgba_to_string(&color));
	  g_object_set(renderer, "foreground", name_options_l[i][1], NULL);

	  break;
	}
    }

  read_database(NULL, NULL);
}

static void
tree_lines_set(GtkToggleButton* button)
{
  for(int i = 0; i < NAME_OPTIONS_SIZE; ++i)
    {
      if(!(strcmp("tree_lines", name_options_l[i][2])))
	{
	  if(name_options_l[i][1])
	    {
	      free(name_options_l[i][1]);
	    }
	  
	  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button)))
	    {
	      name_options_l[i][1] = strdup("true");
	      gtk_tree_view_set_enable_tree_lines(GTK_TREE_VIEW(g_treeview), TRUE);  
	    }
	  else
	    {
	      name_options_l[i][1] = strdup("false");
	      gtk_tree_view_set_enable_tree_lines(GTK_TREE_VIEW(g_treeview), FALSE);        
	    }

	  break;
	}
    }

  read_database(NULL, NULL);
}

static void
favicons_set(GtkToggleButton* button)
{
  for(int i = 0; i < NAME_OPTIONS_SIZE; ++i)
    {
      if(!(strcmp("download_favicon", name_options_l[i][2])))
	{
	  if(name_options_l[i][1])
	    {
	      free(name_options_l[i][1]);
	    }
	  
	  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button)))
	    {
	      name_options_l[i][1] = strdup("true");
	    }
	  else
	    {
	      name_options_l[i][1] = strdup("false");
	    }

	  break;
	}
    }
}

static void
set_key(GtkWidget* entry, gpointer str)
{
  char* arg = (char*)gtk_entry_get_text(GTK_ENTRY(entry));

  for(int i = 0; i < NAME_OPTIONS_SIZE; ++i)
    {
      if(!(strcmp(str, name_options_l[i][2])))
	{
	  if(name_options_l[i][1])
	    {
	      free(name_options_l[i][1]);
	    }

	  name_options_l[i][1] = strdup(arg);

	  break;
	}
    }
}

static void
appearance_reset(GtkWidget* button, gpointer name)
{
  GdkRGBA new_rgba;
  GdkRGBA old_rgba;
  
  int color_flag = 0;
  int font_flag = 0;

  char* color_id = NULL;
  char* font_id = NULL;

  GtkCellRenderer* renderer = NULL;
  GtkWidget* color_button = NULL;
  GtkWidget* font_button = NULL;

  if(!(strcmp(name, "id")))
    {
      color_id = "id_fg";
      font_id = "id_font";
      color_button = id_fg_button;
      font_button = id_font_button;
      renderer = g_cell_renderer_id;
      old_rgba = id_rgba;
    }

  if(!(strcmp(name, "name")))
    {
      color_id = "name_fg";
      font_id = "name_font";
      color_button = name_fg_button;
      font_button = name_font_button;
      renderer = g_cell_renderer_name;
      old_rgba = name_rgba;
    }

  if(!(strcmp(name, "url")))
    {
      color_id = "url_fg";
      font_id = "url_font";
      color_button = url_fg_button;
      font_button = url_font_button;
      renderer = g_cell_renderer_url;
      old_rgba = url_rgba;
    }

  if(!(strcmp(name, "comment")))
    {
      color_id = "comment_fg";
      font_id = "comment_font";
      color_button = comment_fg_button;
      font_button = comment_font_button;
      renderer = g_cell_renderer_comment;
      old_rgba = comment_rgba;
    }
  
  for(int i = 0; i < NAME_OPTIONS_SIZE; ++i)
    {
      if(!(strcmp(name_options_l[i][2], color_id)))
	{
	  if(name_options_l[i][1])
	    {
	      free(name_options_l[i][1]);
	    }
	  
	  old_rgba = new_rgba;
	  gtk_color_chooser_set_rgba(GTK_COLOR_CHOOSER(color_button), &old_rgba);
	  name_options_l[i][1] = NULL;
	  g_object_set(renderer, "foreground", name_options_l[i][1], NULL);
	  color_flag = 1;
	}
      
      if(!(strcmp(name_options_l[i][2], font_id)))
	{
	  if(name_options_l[i][1])
	    {
	      free(name_options_l[i][1]);
	    }
	  
	  gtk_font_button_set_font_name(GTK_FONT_BUTTON(font_button), "");
	  name_options_l[i][1] = NULL;
	  g_object_set(renderer, "font", name_options_l[i][1], NULL);
	  font_flag = 1;
	}
      
      if(font_flag && color_flag)
	{
	  break;
	}
    }
  
  read_database(NULL, NULL);
}

static GtkWidget*
appearance_page()
{
  char* reset_label = "Reset";

  /* font buttons */
  id_font_button = gtk_font_button_new();
  name_font_button = gtk_font_button_new();
  url_font_button = gtk_font_button_new();
  comment_font_button = gtk_font_button_new();
  
  for(int i = 0; i < NAME_OPTIONS_SIZE; ++i)
    {
      if(!(strcmp(name_options_l[i][2], "id_fg"))
	 && name_options_l[i][1])
	{
	  gdk_rgba_parse(&id_rgba, name_options_l[i][1]);
	}

      if(!(strcmp(name_options_l[i][2], "name_fg"))
	 && name_options_l[i][1])
	{
	  gdk_rgba_parse(&name_rgba, name_options_l[i][1]);
	}

      if(!(strcmp(name_options_l[i][2], "url_fg"))
	 && name_options_l[i][1])
	{
	  gdk_rgba_parse(&url_rgba, name_options_l[i][1]);
	}
      
      if(!(strcmp(name_options_l[i][2], "comment_fg"))
	 && name_options_l[i][1])
	{
	  gdk_rgba_parse(&comment_rgba, name_options_l[i][1]);
	}

      if(!(strcmp(name_options_l[i][2], "id_font")))
	{
	  if(name_options_l[i][1])
	    {
	      gtk_font_button_set_font_name
		(GTK_FONT_BUTTON(id_font_button), name_options_l[i][1]);
	    }
	  else
	    {
	      gtk_font_button_set_font_name
		(GTK_FONT_BUTTON(id_font_button), "none");
	    }
	}

      if(!(strcmp(name_options_l[i][2], "name_font")))
	{
	  if(name_options_l[i][1])
	    {
	      gtk_font_button_set_font_name
		(GTK_FONT_BUTTON(name_font_button), name_options_l[i][1]);
	    }
	  else
	    {
	      gtk_font_button_set_font_name
		(GTK_FONT_BUTTON(name_font_button), "none");
	    }
	}      

      if(!(strcmp(name_options_l[i][2], "url_font")))
	{
	  if(name_options_l[i][1])
	    {
	      gtk_font_button_set_font_name
		(GTK_FONT_BUTTON(url_font_button), name_options_l[i][1]);
	    }
	  else
	    {
	      gtk_font_button_set_font_name
		(GTK_FONT_BUTTON(url_font_button), "none");
	    }
	}

      if(!(strcmp(name_options_l[i][2], "comment_font")))
	{
	  if(name_options_l[i][1])
	    {
	      gtk_font_button_set_font_name
		(GTK_FONT_BUTTON(comment_font_button), name_options_l[i][1]);
	    }
	  else
	    {
	      gtk_font_button_set_font_name
		(GTK_FONT_BUTTON(comment_font_button), "none");
	    }
	}      
    }
  
  id_fg_button = gtk_color_button_new_with_rgba(&id_rgba);
  name_fg_button = gtk_color_button_new_with_rgba(&name_rgba);
  url_fg_button = gtk_color_button_new_with_rgba(&url_rgba);
  comment_fg_button = gtk_color_button_new_with_rgba(&comment_rgba);
  id_reset_button = gtk_button_new_with_label(reset_label);
  name_reset_button = gtk_button_new_with_label(reset_label);
  url_reset_button = gtk_button_new_with_label(reset_label);
  comment_reset_button = gtk_button_new_with_label(reset_label);
  
  g_signal_connect
    (GTK_WIDGET(id_fg_button), "color-set", G_CALLBACK(select_color)
     ,"id_fg");

  g_signal_connect
    (GTK_WIDGET(name_fg_button), "color-set", G_CALLBACK(select_color)
     ,"name_fg");

  g_signal_connect
    (GTK_WIDGET(url_fg_button), "color-set", G_CALLBACK(select_color)
     ,"url_fg");

  g_signal_connect
    (GTK_WIDGET(comment_fg_button), "color-set", G_CALLBACK(select_color)
     ,"comment_fg");

  g_signal_connect
    (GTK_WIDGET(id_reset_button), "clicked", G_CALLBACK(appearance_reset)
     ,"id");

  g_signal_connect
    (GTK_WIDGET(name_reset_button), "clicked", G_CALLBACK(appearance_reset)
     ,"name");

  g_signal_connect
    (GTK_WIDGET(url_reset_button), "clicked", G_CALLBACK(appearance_reset)
     ,"url");

  g_signal_connect
    (GTK_WIDGET(comment_reset_button), "clicked", G_CALLBACK(appearance_reset)
     ,"comment");
  
  g_signal_connect
    (GTK_WIDGET(id_font_button), "font-set", G_CALLBACK(select_font)
     ,"id_font");

  g_signal_connect
    (GTK_WIDGET(name_font_button), "font-set", G_CALLBACK(select_font)
     ,"name_font");

  g_signal_connect
    (GTK_WIDGET(url_font_button), "font-set", G_CALLBACK(select_font)
     ,"url_font");

  g_signal_connect
    (GTK_WIDGET(comment_font_button), "font-set", G_CALLBACK(select_font)
     ,"comment_font");

  GtkWidget* id_label = gtk_label_new("Dir / Id");
  GtkWidget* name_label= gtk_label_new("Name");
  GtkWidget* url_label = gtk_label_new("Url");
  GtkWidget* comment_label= gtk_label_new("Comment");

  GtkWidget* grid = grid_new();
  gtk_grid_attach(GTK_GRID(grid), id_label, 0,  0, 20, 1);
  gtk_grid_attach(GTK_GRID(grid), id_fg_button, 20, 0, 10, 1);
  gtk_grid_attach(GTK_GRID(grid), id_font_button, 30, 0, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), id_reset_button, 60, 0, 10, 1);
  gtk_grid_attach(GTK_GRID(grid), name_label, 0,  1, 20, 1);
  gtk_grid_attach(GTK_GRID(grid), name_fg_button, 20, 1, 10, 1);
  gtk_grid_attach(GTK_GRID(grid), name_font_button, 30, 1, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), name_reset_button, 60, 1, 10, 1);
  gtk_grid_attach(GTK_GRID(grid), url_label, 0,  2, 20, 1);
  gtk_grid_attach(GTK_GRID(grid), url_fg_button, 20, 2, 10, 1);
  gtk_grid_attach(GTK_GRID(grid), url_font_button, 30, 2, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), url_reset_button, 60, 2, 10, 1);  
  gtk_grid_attach(GTK_GRID(grid), comment_label, 0,  3, 20, 1);
  gtk_grid_attach(GTK_GRID(grid), comment_fg_button, 20, 3, 10, 1);
  gtk_grid_attach(GTK_GRID(grid), comment_font_button, 30, 3, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), comment_reset_button, 60, 3, 10, 1);
  
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

  gtk_entry_set_text(GTK_ENTRY(database_file_entry), name_options_l[0][1]);

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

  if(!(strcmp(name_options_l[2][1], "true")))
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

static GtkWidget*
key_entry_generator(char* label, char* action, char* key)
{
  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
  GtkWidget* key_label = gtk_label_new(label);
  GtkWidget* key_entry = gtk_entry_new();

  gtk_entry_set_text(GTK_ENTRY(key_entry), key);

  g_signal_connect(GTK_WIDGET(key_entry), "activate"
		   ,G_CALLBACK(set_key), action);

  gtk_box_pack_end(GTK_BOX(box), key_entry, FALSE, FALSE, 1);  
  gtk_box_pack_end(GTK_BOX(box), key_label, FALSE, FALSE, 1);

  return box; 
}

static GtkWidget*
keybind_page()
{
  char* keys[21][3] =
    {
      {"Scroll Up", "scroll_up_key", name_options_l[11][1]}
      ,{"Scroll Down", "scroll_down_key", name_options_l[12][1]}
      ,{"Scroll Left", "scroll_left_key", name_options_l[13][1]}
      ,{"Scroll Right", "scroll_right_key", name_options_l[14][1]}
      ,{"Scroll to bottom", "scroll end_key", name_options_l[15][1]}
      ,{"Scroll to top", "scroll_beg_key", name_options_l[16][1]}
      ,{"Scroll to begin of row", "scroll_beg_row_key", name_options_l[17][1]}
      ,{"Scroll to end of row", "scroll_end_row_key", name_options_l[18][1]}
      ,{"Scroll page up", "scroll_page_up_key", name_options_l[19][1]}
      ,{"Scroll page down", "scroll_page_down_key", name_options_l[20][1]}
      ,{"Expand/Contract row", "ec_key", name_options_l[21][1]}
      ,{"Expand/Contract all rows", "ea_key", name_options_l[22][1]}
      ,{"Focus on search bar", "search_key", name_options_l[23][1]}
      ,{"Copy url", "copy_key", name_options_l[24][1]}
      ,{"Reload", "reload_key", name_options_l[25][1]}
      ,{"Insert", "insert_key", name_options_l[26][1]}
      ,{"Edit", "edit_key", name_options_l[27][1]}
      ,{"Rename", "rename_key", name_options_l[28][1]}
      ,{"Delete", "delete_key", name_options_l[29][1]}
      ,{"Options", "options_key", name_options_l[30][1]}
      ,{"Open database", "open_database_key", name_options_l[31][1]}
    };

  GtkWidget* grid = grid_new();
  
  for(int i = 0; i < 21; ++i)
    {
      GtkWidget* key_box = key_entry_generator
	(keys[i][0], keys[i][1], keys[i][2]);

      gtk_grid_attach(GTK_GRID(grid), key_box, 0, i, 10, 1);      
    }
  
  return grid;
}

void
options_window(GtkWidget* button) 
{
  GtkWidget* window = dialog_new("Options", gpmb_window);

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

  /* keybind page */
  GtkWidget* page_keybind_l = gtk_label_new("Keybinds");
  GtkWidget* page_keybind_b = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
  GtkWidget* keybind_grid = keybind_page();
  gtk_container_add(GTK_CONTAINER(page_keybind_b), keybind_grid);
  
  /* buttons */
  GtkWidget* write_button = gtk_button_new_with_mnemonic("_Write config");
  g_signal_connect
    (GTK_WIDGET(write_button), "clicked", G_CALLBACK(write_config)
     ,NULL);

  GtkWidget* cancel_button = gtk_button_new_with_mnemonic("_Close");
  g_signal_connect
    (GTK_WIDGET(cancel_button), "clicked", G_CALLBACK(close_window)
     ,window);

  /* button box */
  GtkWidget* button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
  gtk_box_pack_end(GTK_BOX(button_box), cancel_button, FALSE, FALSE, 1);
  gtk_box_pack_end(GTK_BOX(button_box), write_button, FALSE, FALSE, 1);

  /* main box */
  GtkWidget* main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);

  /* notebook */
  GtkWidget* notebook = gtk_notebook_new();
  gtk_notebook_append_page
    (GTK_NOTEBOOK(notebook), page_settings_b, page_settings_l);
  
  gtk_notebook_append_page
    (GTK_NOTEBOOK(notebook), page_appearance_b, page_appearance_l);

  gtk_notebook_append_page
    (GTK_NOTEBOOK(notebook), page_keybind_b, page_keybind_l);

  /* scrolled window */
  GtkWidget* opt_scrolled_window = gtk_scrolled_window_new(NULL, NULL);

  gtk_scrolled_window_set_min_content_width
    (GTK_SCROLLED_WINDOW(opt_scrolled_window), 500);

  gtk_scrolled_window_set_min_content_height
    (GTK_SCROLLED_WINDOW(opt_scrolled_window), 300);
  
  gtk_container_add(GTK_CONTAINER(opt_scrolled_window), notebook);
  
  gtk_box_pack_start(GTK_BOX(main_box), opt_scrolled_window, TRUE, TRUE, 1);
  gtk_box_pack_end(GTK_BOX(main_box), button_box, FALSE, FALSE, 1);
  gtk_container_add(GTK_CONTAINER(window), main_box);

  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_spinner_start(GTK_SPINNER(g_spinner));
}
