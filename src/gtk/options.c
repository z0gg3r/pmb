#include "options.h"

char* config_file = NULL;
char* name_options_l[NAME_OPTIONS_SIZE][3];
key_s* keys[KEYS_N];

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

/* used to parse key options and store it in key_s* keys */
static void
set_keys();

/* to feed the options matrix easily */
static void
options_insert(int index, char* option, char* data)
{
  char* option_cut = strdup(option);
  char* option_bkp = strdup(option_cut);
  char* option_name = strsep(&option_cut, "=");

  name_options_l[index][0] = option;
  name_options_l[index][2] = option_name;
  name_options_l[index][1] = NULL;
    
  if(data)
    {
      name_options_l[index][1] = strdup(data);
    }
  
  free(option_bkp);
}

/* set up default options at program start */
static void 
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

  options_insert(0, "database=%s\n", g_database_file);
  options_insert(1, "tree_lines=%s\n", "true");
  options_insert(2, "download_favicons=%s\n", "false");
  options_insert(3, "open_url_browser=%s\n", "firefox -new-tab %s &");
  options_insert(4, "show_toolbar=%s\n", "true");
  options_insert(5, "show_statusbar=%s\n", "true");
  options_insert(6, "show_headers=%s\n", "true");
  options_insert(7, "show_searchbar=%s\n", "true");
  options_insert(8, "id_fg=%s\n", NULL);
  options_insert(9, "name_fg=%s\n", NULL);
  options_insert(10, "url_fg=%s\n", NULL);
  options_insert(11, "comment_fg=%s\n", NULL);
  options_insert(12, "id_font=%s\n", NULL);
  options_insert(13, "name_font=%s\n", NULL);
  options_insert(14, "url_font=%s\n", NULL);
  options_insert(15, "comment_font=%s\n", NULL);
  options_insert(16, "scroll_up_key=%s\n", "k");
  options_insert(17, "scroll_down_key=%s\n", "j");
  options_insert(18, "scroll_left_key=%s\n", "h");
  options_insert(19, "scroll_right_key=%s\n", "l");
  options_insert(20, "scroll_top_key=%s\n", "g");
  options_insert(21, "scroll_bottom_key=%s\n", "Shift-G");
  options_insert(22, "scroll_begin_line_key=%s\n", "0");
  options_insert(23, "scroll_end_line_key=%s\n", "Shift-dollar");
  options_insert(24, "scroll_page_up_key=%s\n", "b");
  options_insert(25, "scroll_page_down_key=%s\n", "f");
  options_insert(26, "ec_key=%s\n", "x");
  options_insert(27, "ea_key=%s\n", "Shift-X");
  options_insert(28, "search_bar_key=%s\n", "s");
  options_insert(29, "copy_key=%s\n", "c");
  options_insert(30, "reload_key=%s\n", "r");
  options_insert(31, "insert_key=%s\n", "i");
  options_insert(32, "edit_key=%s\n", "e");
  options_insert(33, "delete_key=%s\n", "d");
  options_insert(34, "rename_key=%s\n", "n");
  options_insert(35, "options_key=%s\n", "o");
  options_insert(36, "open_database_key_key=%s\n", "Control-o");
  options_insert(37, "open_url_browser_key=%s\n", "p");
  options_insert(38, "toggle_searchbar_key=%s\n", "Shift-S");
  
  set_keys();
}

/* free options at program end */
void
destroy_options()
{
  for(int i = 0; i < NAME_OPTIONS_SIZE; ++i)
    {
      if(name_options_l[i][1])
	{
	  free(name_options_l[i][1]);
	}
    }

  for(int i = 0; i < KEYS_N; ++i)
    {
      if(keys[i])
	{
	  free(keys[i]);
	}
    }
}

/* write confg file */
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
	      fprintf(fp, name_options_l[i][0]
		      ,name_options_l[i][1]);
	    }
	}
      
      fclose(fp);
    }
}

/* read config file */
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
	      option_bkp = strdup(option);
	      
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

/* parse keybnds into key_s structures */
static key_s*
key_parser(char* keybind)
{
  if(!keybind)
    {
      return NULL;
    }
  
  char* key = strdup(keybind);
  char* key_bkp = strdup(key);

  key_s* k = malloc(sizeof(key_s));
  k->state = 0;
  k->key = NULL;
  
  while(1)
    {
      char* tmp = strsep(&key, "-");

      if(tmp)
	{
	  if(!strcmp(tmp, "Control"))
	    {
	      k->state += GDK_CONTROL_MASK;
	    }

	  else if(!strcmp(tmp, "Alt"))
	    {
	      k->state += GDK_MOD1_MASK;
	    }

	  else if(!strcmp(tmp, "Shift"))
	    {
	      k->state += GDK_SHIFT_MASK;
	    }

	  else if(!strcmp(tmp, "Super")
		  ||!strcmp(tmp, "Win"))
	    {
	      k->state += 67108928;
	    }

	  else
	    {
	      k->key = tmp;
	    }
	}
      else
	{
	  break;
	}
    }
  
  free(key_bkp);
  return k;
}

/* made parsed keys available to other parts of the program */
static void
set_keys()
{
  for(int i = 0; i < KEYS_N; ++i)
    {
      if(keys[i])
	{
	  free(keys[i]);
	}
      
      keys[i] = NULL;
    }

  SCROLL_UP = key_parser(SCROLL_UP_KEY);
  SCROLL_DOWN = key_parser(SCROLL_DOWN_KEY);
  SCROLL_LEFT = key_parser(SCROLL_LEFT_KEY);
  SCROLL_RIGHT = key_parser(SCROLL_RIGHT_KEY);
  SCROLL_TOP = key_parser(SCROLL_TOP_KEY);
  SCROLL_BOTTOM = key_parser(SCROLL_BOTTOM_KEY);
  SCROLL_PAGE_UP = key_parser(SCROLL_PAGE_UP_KEY);
  SCROLL_PAGE_DOWN = key_parser(SCROLL_PAGE_DOWN_KEY);
  SCROLL_BEGIN_LINE = key_parser(SCROLL_BEGIN_LINE_KEY);
  SCROLL_END_LINE = key_parser(SCROLL_END_LINE_KEY);
  EC = key_parser(EC_KEY);
  EA = key_parser(EA_KEY);
  SEARCH_BAR = key_parser(SEARCH_BAR_KEY);
  COPY = key_parser(COPY_KEY);
  RELOAD = key_parser(RELOAD_KEY);
  INSERT = key_parser(INSERT_KEY);
  EDIT = key_parser(EDIT_KEY);
  DELETE = key_parser(DELETE_KEY);
  RENAME = key_parser(RENAME_KEY);
  OPTIONS = key_parser(OPTIONS_KEY);
  OPEN_DATABASE = key_parser(OPEN_DATABASE_KEY);
  OPEN_URL_BROWSER = key_parser(OPEN_URL_BROWSER_KEY);
  TOGGLE_SEARCHBAR = key_parser(TOGGLE_SEARCHBAR_KEY);
}

/* change font via appearance_page */
static void
select_font(GtkFontButton* button, gpointer name)
{
  char** option = NULL;
  GtkCellRenderer* renderer = NULL;
  
  if(!(strcmp(name, "id_font")))
    {
      option = &ID_FONT_OPTION;
      renderer = g_cell_renderer_id;
    }
  else if(!(strcmp(name, "name_font")))
    {
      option = &NAME_FONT_OPTION;
      renderer = g_cell_renderer_name;
    }
  else if(!(strcmp(name, "url_font")))
    {
      option = &URL_FONT_OPTION;
      renderer = g_cell_renderer_url;
    }
  else if(!(strcmp(name, "comment_font")))
    {
      option = &COMMENT_FONT_OPTION;
      renderer = g_cell_renderer_comment;
    }
  
  if(*option)
    {
      free(*option);
    }
  
  *option = strdup
    (gtk_font_button_get_font_name(GTK_FONT_BUTTON(button)));
  
  g_object_set(renderer, "font", *option, NULL);
}

/* change color via appearance_page */
static void
select_color(GtkColorButton* button, gpointer name)
{
  char** option = NULL;
  GdkRGBA color;

  GtkCellRenderer* renderer = NULL;
  
  if(!(strcmp(name, "id_fg")))
    {
      option = &ID_FG_OPTION;
      renderer = g_cell_renderer_id;
    }
  else if(!(strcmp(name, "name_fg")))
    {
      option = &NAME_FG_OPTION;
      renderer = g_cell_renderer_name;
    }
  else if(!(strcmp(name, "url_fg")))
    {
      option = &URL_FG_OPTION;
      renderer = g_cell_renderer_url;
    }
  else if(!(strcmp(name, "comment_fg")))
    {
      option = &COMMENT_FG_OPTION;
      renderer = g_cell_renderer_comment;
    }
  
  if(*option)
    {
      free(*option);
    }
  
  gtk_color_chooser_get_rgba
    (GTK_COLOR_CHOOSER(button), &color);
  
  *option = strdup(gdk_rgba_to_string(&color));
  g_object_set(renderer, "foreground", *option, NULL);

  read_database(NULL, NULL);
}

/* reset appearance, signal to reset buttons in appearance_page */
static void
appearance_reset(GtkWidget* button, gpointer name)
{
  GdkRGBA new_rgba;
  GdkRGBA old_rgba;
  
  char** font_option = NULL;
  char** color_option = NULL;
  
  GtkCellRenderer* renderer = NULL;
  GtkWidget* color_button = NULL;
  GtkWidget* font_button = NULL;

  if(!(strcmp(name, "id")))
    {
      font_option = &ID_FONT_OPTION;
      color_option = &ID_FG_OPTION;
      color_button = id_fg_button;
      font_button = id_font_button;
      renderer = g_cell_renderer_id;
      old_rgba = id_rgba;
    }

  if(!(strcmp(name, "name")))
    {
      font_option = &NAME_FONT_OPTION;
      color_option = &NAME_FG_OPTION;
      color_button = name_fg_button;
      font_button = name_font_button;
      renderer = g_cell_renderer_name;
      old_rgba = name_rgba;
    }

  if(!(strcmp(name, "url")))
    {
      font_option = &URL_FONT_OPTION;
      color_option = &URL_FG_OPTION;
      color_button = url_fg_button;
      font_button = url_font_button;
      renderer = g_cell_renderer_url;
      old_rgba = url_rgba;
    }

  if(!(strcmp(name, "comment")))
    {
      font_option = &COMMENT_FONT_OPTION;
      color_option = &COMMENT_FG_OPTION;
      color_button = comment_fg_button;
      font_button = comment_font_button;
      renderer = g_cell_renderer_comment;
      old_rgba = comment_rgba;
    }
  
  if(*color_option)
    {
      free(*color_option);
    }
  
  old_rgba = new_rgba;
  
  gtk_color_chooser_set_rgba
    (GTK_COLOR_CHOOSER(color_button), &old_rgba);

  *color_option = NULL;
  
  g_object_set(renderer, "foreground", *color_option, NULL);
  
  if(*font_option)
    {
      free(*font_option);
    }
	  
  gtk_font_button_set_font_name
    (GTK_FONT_BUTTON(font_button), "");

  *font_option = NULL;

  g_object_set(renderer, "font", *font_option, NULL);
      
  read_database(NULL, NULL);
}

/* set font buttons in appearance_page */
static void
set_font_button_name(GtkWidget* button, char* font_option)
{
  char* font_name = "none";
  
  if(font_option)
    {
      font_name = font_option;
    }
  
  gtk_font_button_set_font_name
    (GTK_FONT_BUTTON(button), font_name);
}

static GtkWidget*
appearance_page()
{
  if(ID_FG_OPTION)
    {
      gdk_rgba_parse(&id_rgba, ID_FG_OPTION);
    }
  
  if(NAME_FG_OPTION)
    {
      gdk_rgba_parse(&name_rgba, NAME_FG_OPTION);
    }

  if(URL_FG_OPTION)
    {
      gdk_rgba_parse(&url_rgba, URL_FG_OPTION);
    }
      
  if(COMMENT_FG_OPTION)
    {
      gdk_rgba_parse(&comment_rgba, COMMENT_FG_OPTION);
    }

    /* font buttons */
  id_font_button = gtk_font_button_new();
  name_font_button = gtk_font_button_new();
  url_font_button = gtk_font_button_new();
  comment_font_button = gtk_font_button_new();
  
  set_font_button_name(id_font_button, ID_FONT_OPTION);
  set_font_button_name(name_font_button, NAME_FONT_OPTION);
  set_font_button_name(url_font_button, URL_FONT_OPTION);
  set_font_button_name(comment_font_button, COMMENT_FONT_OPTION);

  char* reset_label = "Reset";
  
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

/* turn toggle buttons in settings_page on/off */
static gboolean
toggle_buttons_set(GtkToggleButton* button, char* option)
{
  char* opt = "false";
  gboolean b = FALSE;
  
  if(option)
    {
      free(option);
    }
  
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button)))
    {
      opt = "true";
      b = TRUE;
    }
  
  option = strdup(opt);
  return b;
}

/* set treelines option */
static void
tree_lines_set(GtkToggleButton* button)
{
  if(TREE_LINES_OPTION)
    {
      free(TREE_LINES_OPTION);
    }
  
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button)))
    {
      TREE_LINES_OPTION = strdup("true");
      gtk_tree_view_set_enable_tree_lines
	(GTK_TREE_VIEW(g_treeview), TRUE);  
    }
  else
    {
      TREE_LINES_OPTION = strdup("false");
      gtk_tree_view_set_enable_tree_lines
	(GTK_TREE_VIEW(g_treeview), FALSE);        
    }

  read_database(NULL, NULL);
}

/* set favicon option */
static void
favicons_set(GtkToggleButton* button)
{
  toggle_buttons_set
    (GTK_TOGGLE_BUTTON(button), DOWNLOAD_FAVICON_OPTION);  
}

static void
show_toolbar_set(GtkToggleButton* button)
{
  toggle_buttons_set
    (GTK_TOGGLE_BUTTON(button), SHOW_TOOLBAR_OPTION);

  widget_hide(NULL, "toolbar");
}

/* show/hide statusbar option */
static void
show_statusbar_set(GtkToggleButton* button)
{
  toggle_buttons_set
    (GTK_TOGGLE_BUTTON(button), SHOW_STATUSBAR_OPTION);
  
  widget_hide(NULL, "statusbar");
}

/* show/hide headers option */
static void
show_headers_set(GtkToggleButton* button)
{
  toggle_buttons_set
    (GTK_TOGGLE_BUTTON(button), SHOW_HEADERS_OPTION);
  
  widget_hide(NULL, "headers"); 
}

/* show/hide searchbar option */
static void
show_searchbar_set(GtkToggleButton* button)
{
  toggle_buttons_set
    (GTK_TOGGLE_BUTTON(button), SHOW_SEARCHBAR_OPTION);
  
  widget_hide(NULL, "searchbar"); 
}

static GtkWidget*
settings_page()
{
  /* database entry */
  GtkWidget* database_label = gtk_label_new("Database");

  GtkWidget* database_file_entry = gtk_entry_new();
  gtk_entry_set_placeholder_text
    (GTK_ENTRY(database_file_entry), "database");

  gtk_entry_set_text
    (GTK_ENTRY(database_file_entry), DATABASE_OPTION);

  /* browser entry */
  GtkWidget* browser_label = gtk_label_new("Browser");

  GtkWidget* browser_entry = gtk_entry_new();
  gtk_entry_set_placeholder_text
    (GTK_ENTRY(browser_entry), "browser");

  gtk_entry_set_text
    (GTK_ENTRY(browser_entry), OPEN_URL_BROWSER_OPTION);

  /* tree lines */
  GtkWidget* tree_lines_button = gtk_check_button_new_with_label
    ("Show tree lines");

  if(!(strcmp(TREE_LINES_OPTION, "true")))
    {
      gtk_toggle_button_set_active
	(GTK_TOGGLE_BUTTON(tree_lines_button), TRUE);
    }

  g_signal_connect
    (GTK_WIDGET(tree_lines_button), "toggled"
     ,G_CALLBACK(tree_lines_set), NULL);
  
  /* favicons */
  GtkWidget* favicon_button = gtk_check_button_new_with_label
    ("Download favicon");

  if(!(strcmp(DOWNLOAD_FAVICON_OPTION, "true")))
    {
      gtk_toggle_button_set_active
	(GTK_TOGGLE_BUTTON(favicon_button), TRUE);
    }

  g_signal_connect
    (GTK_WIDGET(favicon_button), "toggled"
     ,G_CALLBACK(favicons_set), NULL);
  
  GtkWidget* toolbar_button = gtk_check_button_new_with_label
    ("Show toolbar");

  if(!(strcmp(SHOW_TOOLBAR_OPTION, "true")))
    {
      gtk_toggle_button_set_active
	(GTK_TOGGLE_BUTTON(toolbar_button), TRUE);
    }

  g_signal_connect
    (GTK_WIDGET(toolbar_button), "toggled"
     ,G_CALLBACK(show_toolbar_set), NULL);
    
  GtkWidget* statusbar_button = gtk_check_button_new_with_label
    ("Show statusbar");
  
  if(!(strcmp(SHOW_STATUSBAR_OPTION, "true")))
    {
      gtk_toggle_button_set_active
	(GTK_TOGGLE_BUTTON(statusbar_button), TRUE);
    }

  g_signal_connect
    (GTK_WIDGET(statusbar_button), "toggled"
     ,G_CALLBACK(show_statusbar_set), NULL);
  
  GtkWidget* headers_button = gtk_check_button_new_with_label
    ("Show Headers");

  if(!(strcmp(SHOW_HEADERS_OPTION, "true")))
    {
      gtk_toggle_button_set_active
	(GTK_TOGGLE_BUTTON(headers_button), TRUE);
    }

  g_signal_connect
    (GTK_WIDGET(headers_button), "toggled"
     ,G_CALLBACK(show_headers_set), NULL);
  
  GtkWidget* searchbar_button = gtk_check_button_new_with_label
    ("Show searchbar");

  if(!(strcmp(SHOW_SEARCHBAR_OPTION, "true")))
    {
      gtk_toggle_button_set_active
	(GTK_TOGGLE_BUTTON(searchbar_button), TRUE);
    }

  g_signal_connect
    (GTK_WIDGET(searchbar_button), "toggled"
     ,G_CALLBACK(show_searchbar_set), NULL);
  
  GtkWidget* grid = grid_new();
  gtk_grid_attach(GTK_GRID(grid), database_label, 0, 0, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), database_file_entry, 40, 0, 50, 1);
  gtk_grid_attach(GTK_GRID(grid), browser_label, 0, 1, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), browser_entry, 40, 1, 50, 1);
  gtk_grid_attach(GTK_GRID(grid), tree_lines_button, 0,  2, 25, 1);
  gtk_grid_attach(GTK_GRID(grid), favicon_button, 25,  2, 25, 1);
  gtk_grid_attach(GTK_GRID(grid), toolbar_button, 0, 3, 25, 1);
  gtk_grid_attach(GTK_GRID(grid), statusbar_button, 25, 3, 25, 1);
  gtk_grid_attach(GTK_GRID(grid), headers_button, 0, 4, 25, 1);
  gtk_grid_attach(GTK_GRID(grid), searchbar_button, 25, 4, 25, 1);

  return grid;
}

/* set keys configured in keybind_page */
static void
key_set(GtkWidget* entry, gpointer str)
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

  set_keys();
}

/* generate key entres to keybind_page */
static GtkWidget*
key_entry_generator(char* label, char* action, char* key)
{
  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
  GtkWidget* key_label = gtk_label_new(label);
  GtkWidget* key_entry = gtk_entry_new();

  gtk_entry_set_text(GTK_ENTRY(key_entry), key);

  g_signal_connect(GTK_WIDGET(key_entry), "activate"
		   ,G_CALLBACK(key_set), action);

  gtk_box_pack_end(GTK_BOX(box), key_entry, FALSE, FALSE, 1);  
  gtk_box_pack_end(GTK_BOX(box), key_label, FALSE, FALSE, 1);

  return box; 
}

static GtkWidget*
keybind_page()
{
  char* keybind_keys[KEYS_N][3] =
    {
      {"Scroll Up", "scroll_up_key", SCROLL_UP_KEY}
      ,{"Scroll Down", "scroll_down_key", SCROLL_DOWN_KEY}
      ,{"Scroll Left", "scroll_left_key", SCROLL_LEFT_KEY}
      ,{"Scroll Right", "scroll_right_key", SCROLL_RIGHT_KEY}
      ,{"Scroll to bottom", "scroll_bottom_key", SCROLL_BOTTOM_KEY}
      ,{"Scroll to top", "scroll_top_key", SCROLL_TOP_KEY}
      ,{"Scroll to begin of line", "scroll_begin_line_key", SCROLL_BEGIN_LINE_KEY}
      ,{"Scroll to end of line", "scroll_end_line_key", SCROLL_END_LINE_KEY}
      ,{"Scroll page up", "scroll_page_up_key", SCROLL_PAGE_UP_KEY}
      ,{"Scroll page down", "scroll_page_down_key", SCROLL_PAGE_DOWN_KEY}
      ,{"Expand/Contract row", "ec_key", EC_KEY}
      ,{"Expand/Contract all rows", "ea_key", EA_KEY}
      ,{"Focus on search bar", "search_key", SEARCH_BAR_KEY}
      ,{"Copy url", "copy_key", COPY_KEY}
      ,{"Reload", "reload_key", RELOAD_KEY}
      ,{"Insert", "insert_key", INSERT_KEY}
      ,{"Edit", "edit_key", EDIT_KEY}
      ,{"Rename", "rename_key", RENAME_KEY}
      ,{"Delete", "delete_key", DELETE_KEY}
      ,{"Options", "options_key", OPTIONS_KEY}
      ,{"Open database", "open_database_key", OPEN_DATABASE_KEY}
      ,{"Open url in browser", "open_url_browser_key", OPEN_URL_BROWSER_KEY}
      ,{"Toggle searchbar", "toggle_searchbar_key", TOGGLE_SEARCHBAR_KEY}
    };

  GtkWidget* grid = grid_new();
  
  for(int i = 0; i < KEYS_N; ++i)
    {
      GtkWidget* key_box = key_entry_generator
	(keybind_keys[i][0], keybind_keys[i][1], keybind_keys[i][2]);

      gtk_grid_attach(GTK_GRID(grid), key_box, 0, i, 10, 1);      
    }

  GtkWidget* scrolled_window = gtk_scrolled_window_new(NULL, NULL);

  gtk_scrolled_window_set_min_content_width
    (GTK_SCROLLED_WINDOW(scrolled_window), 500);

  gtk_scrolled_window_set_min_content_height
    (GTK_SCROLLED_WINDOW(scrolled_window), 300);
  
  gtk_container_add(GTK_CONTAINER(scrolled_window), grid);
  
  return scrolled_window;
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
  
  gtk_box_pack_start(GTK_BOX(main_box), notebook, TRUE, TRUE, 1);
  gtk_box_pack_end(GTK_BOX(main_box), button_box, FALSE, FALSE, 1);
  gtk_container_add(GTK_CONTAINER(window), main_box);

  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_spinner_start(GTK_SPINNER(g_spinner));
}
