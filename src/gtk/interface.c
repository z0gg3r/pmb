#include "interface.h"

/* globals */
sqlite3* g_db = NULL;
GtkWidget* g_info_label = NULL;
GtkWidget* g_search_entry = NULL;
GtkWidget* g_spinner = NULL;
GtkWidget* g_scrolled_window = NULL;
GtkWidget* gpmb_window 	= NULL;

GtkWidget* g_statusbar = NULL;
GtkWidget* g_searchbar = NULL;
GtkWidget* g_toolbar = NULL;

void 
destroy() 
{
  gtk_main_quit();
}

void
widget_hide(GtkWidget* button, char* name)
{
  if(!strcmp(name, "statusbar"))
    {
      void (*function)(GtkWidget*) = gtk_widget_show;
      
      if(gtk_widget_get_visible(GTK_WIDGET(g_statusbar)))
	{
	  function = gtk_widget_hide;
	}

      function(GTK_WIDGET(g_statusbar));
    }

  if(!(strcmp(name, "toolbar")))
    {
      void (*function)(GtkWidget*) = gtk_widget_show;
      
      if(gtk_widget_get_visible(GTK_WIDGET(g_toolbar)))
	{
	  function = gtk_widget_hide;
	}

      function(GTK_WIDGET(g_toolbar));
    }

  if(!(strcmp(name, "searchbar")))
    {
      void (*function)(GtkWidget*) = gtk_widget_show;
      
      if(gtk_widget_get_visible(GTK_WIDGET(g_searchbar)))
	{
	  function = gtk_widget_hide;
	}
      
      function(GTK_WIDGET(g_searchbar));
    }
  
  if(!(strcmp(name, "headers")))
    {
      gboolean b = TRUE;

      if(gtk_tree_view_get_headers_visible
	 (GTK_TREE_VIEW(g_treeview)))
	{
	  b = FALSE;
	}

      gtk_tree_view_set_headers_visible
	(GTK_TREE_VIEW(g_treeview), b);
    }
}

int 
gtk_interface(int argc, char* argv[]) 
{
  gtk_init(&argc, &argv);

  /* set up the main window */
  gpmb_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(gpmb_window), "Bookmarks");
  gtk_window_set_role(GTK_WINDOW(gpmb_window), "Bookmarks");
  gtk_window_set_default_size(GTK_WINDOW(gpmb_window)
			      ,800, 600);

  g_signal_connect
    (gpmb_window, "destroy", G_CALLBACK(destroy), NULL);

  /* tree view */
  g_search_entry = gtk_entry_new();
  g_treeview = tree_view(g_search_entry);

  /* scrolled window for bookmark_view */
  g_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(g_scrolled_window), g_treeview);

  g_signal_connect
    (g_scrolled_window, "key-press-event", G_CALLBACK(key_press), NULL);

  g_signal_connect
    (g_search_entry, "key-release-event", G_CALLBACK(search_entry_key_press)
     ,NULL);

  /* menu bar */
  GtkWidget* menu_bar = menu_bar_new(gpmb_window);

  /* toolbar */
  g_toolbar = tool_box_new(gpmb_window);

  /* searchbar */
  g_searchbar = search_box_new(g_search_entry);

  /* info label */
  g_info_label = gtk_label_new(" ");
  gtk_label_set_xalign(GTK_LABEL(g_info_label), 1);

  /* spinner */
  g_spinner = gtk_spinner_new();

  /* info box */
  g_statusbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
  gtk_box_pack_start(GTK_BOX(g_statusbar), g_info_label, TRUE, TRUE, 1);
  gtk_container_add(GTK_CONTAINER(g_statusbar), g_spinner);

  /* add to main */
  GtkWidget* main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
  gtk_container_add(GTK_CONTAINER(main_box), menu_bar);
  gtk_container_add(GTK_CONTAINER(main_box), g_toolbar);
  gtk_container_add(GTK_CONTAINER(main_box), g_searchbar);
  gtk_box_pack_start(GTK_BOX(main_box), g_scrolled_window, TRUE, TRUE, 1);
  gtk_container_add(GTK_CONTAINER(main_box), g_statusbar);

  /* add main_box into window */
  gtk_container_add(GTK_CONTAINER(gpmb_window), main_box);
  gtk_widget_show_all(gpmb_window);

  /* read database and append to tree */
  read_database(NULL, NULL);

  /* hide bars? */
  if(!(strcmp(SHOW_TOOLBAR_OPTION, "false")))
    {
      widget_hide(g_toolbar, "toolbar");
    }

  if(!(strcmp(SHOW_STATUSBAR_OPTION, "false")))
    {
      widget_hide(g_statusbar, "statusbar");     
    }

  if(!(strcmp(SHOW_HEADERS_OPTION, "false")))
    {
      widget_hide(NULL, "headers");     
    }

  if(!(strcmp(SHOW_SEARCHBAR_OPTION, "false")))
    {
      widget_hide(NULL, "searchbar");     
    }
  
  /* focus tree view */
  gtk_widget_grab_focus(GTK_WIDGET(g_treeview));

  gtk_main();
  return 0;
}
