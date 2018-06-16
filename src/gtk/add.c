#include "add.h"

GtkWidget* favicon_button;

static void 
add_bookmark(GtkWidget* button, gpointer** args) 
{
  char* name = NULL;
  char* url = NULL;
  char* comment = "none";
  char* tag = "none";
  char* tag_t = NULL;
  char* favicon	= "none";

  if(gtk_entry_get_text_length(GTK_ENTRY(args[0])))
    {
      name = (char*)gtk_entry_get_text(GTK_ENTRY(args[0]));
    }

  if(gtk_entry_get_text_length(GTK_ENTRY(args[1])))
    {
      url = (char*)gtk_entry_get_text(GTK_ENTRY(args[1]));
    }

  if(gtk_entry_get_text_length(GTK_ENTRY(args[2])))
    {
      comment = (char*)gtk_entry_get_text(GTK_ENTRY(args[2]));
    }

  tag_t = (char*)gtk_combo_box_text_get_active_text
    (GTK_COMBO_BOX_TEXT(args[3]));

  if(strlen(tag_t) > 1)
    {
      tag = tag_t;
    }

  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(favicon_button)))
    {
      char* favicon_temp = download_favicon(url);

      if(favicon_temp)
	{
	  favicon = favicon_temp;
	}
    }

  if(name && url) 
    {
      /* write to database */
      bookmark* b = bookmark_new(name, url, comment, tag, favicon);
      bookmark_db_write(b, g_db);
      g_free(favicon);
      bookmark_destroy(b);

      /*
      // get it from database 
      bookmark_list* bl = bookmark_db_search(db, URL, url);
      b = bookmark_list_return_next_bookmark(bl);

      // add to treestore 
      GtkTreeIter iter, p_iter;
      bookmark* sb = get_data(NULL);

      gtk_tree_model_get_iter(GTK_TREE_MODEL(model)
      ,&p_iter, selected_path);

      if(selected_path && (strlen(bookmark_url(sb))) > 1)
      gtk_tree_store_insert_after(GTK_TREE_STORE(bookmarks)
      ,&iter, NULL, &p_iter);
      else
      gtk_tree_store_insert_after(GTK_TREE_STORE(bookmarks)
      ,&iter, &p_iter, NULL);

      gtk_tree_store_set(GTK_TREE_STORE(bookmarks), &iter
      ,0, bookmark_id(b)
      ,1, bookmark_name(b)
      ,2, bookmark_url(b)
      ,3, bookmark_comment(b)
      ,4, bookmark_tag(b)
      ,5, star_icon
      , -1);
	
      bookmark_list_destroy(bl);
      bookmark_destroy(sb);
      bookmark_destroy(b);
      */
      read_database(NULL, NULL);
      close_window(NULL, args[4]);
      g_free(args);
      gtk_label_set_text(GTK_LABEL(g_info_label), "Add: Done");
    }
  else
    {
      gtk_label_set_text
	(GTK_LABEL(g_info_label) ,"Add: Error, Name or Url field empty");
    }
}

void 
add_window(GtkWidget* button) 
{
  GtkWidget* window = dialog_new("Add bookmark", gpmb_window);
  GtkWidget** e = entries(TRUE);
  bookmark* b = get_bookmark_from_row(NULL);

  /* tag box */
  GtkWidget* tag_box = tag_box_new();
  GtkWidget* tag_entry = gtk_bin_get_child(GTK_BIN(tag_box));

  gtk_entry_set_icon_from_icon_name
    (GTK_ENTRY(tag_entry), GTK_ENTRY_ICON_PRIMARY, "folder");

  gtk_entry_set_icon_from_icon_name
    (GTK_ENTRY(e[3]), GTK_ENTRY_ICON_PRIMARY, "non-starred");

  if(b) 
    {
      gtk_entry_set_text(GTK_ENTRY(tag_entry), get_full_path(b));

      bookmark_destroy(b);
      b = NULL;
    }

  GtkWidget** add_bookmark_args = g_new(GtkWidget*, 5);
  
  add_bookmark_args[0] = e[1];
  add_bookmark_args[1] = e[3];
  add_bookmark_args[2] = e[5];
  add_bookmark_args[3] = tag_box;
  add_bookmark_args[4] = window;

  /* button */
  GtkWidget* add_button = gtk_button_new_with_mnemonic("_Add");
  g_signal_connect
    (add_button, "clicked", G_CALLBACK(add_bookmark), add_bookmark_args);

  GtkWidget* cancel_button = gtk_button_new_with_mnemonic("_Cancel");
  g_signal_connect
    (cancel_button, "clicked", G_CALLBACK(close_window), window);

  GtkWidget* favicon_label = gtk_label_new("Download");
  gtk_widget_set_halign(GTK_WIDGET(favicon_label), GTK_ALIGN_START);

  favicon_button = gtk_check_button_new_with_label("Favicon");

  if(!strcmp(DOWNLOAD_FAVICON_OPTION, "true"))
    {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(favicon_button), TRUE);
    }
  
  /* grid */
  GtkWidget *grid = gtk_grid_new();
  
  gtk_grid_set_column_spacing(GTK_GRID(grid), 2);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 2);
  gtk_grid_set_column_homogeneous(GTK_GRID(grid), 1);

  gtk_grid_attach(GTK_GRID(grid), e[0], 0,  0, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), e[1], 20, 0, 50, 1);
  gtk_grid_attach(GTK_GRID(grid), e[2], 0,  1, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), e[3], 20, 1, 50, 1);
  gtk_grid_attach(GTK_GRID(grid), e[4], 0,  2, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), e[5], 20, 2, 50, 1);
  gtk_grid_attach(GTK_GRID(grid), e[6], 0,  3, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), tag_box, 20, 3, 50, 1);
  gtk_grid_attach(GTK_GRID(grid), favicon_label, 0,  4, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), favicon_button, 20, 4, 50, 1);
  gtk_grid_attach(GTK_GRID(grid), add_button, 20, 5, 20, 10);
  gtk_grid_attach(GTK_GRID(grid), cancel_button, 40, 5, 20, 10);

  g_free(e);
  gtk_container_add(GTK_CONTAINER(window), grid);
  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_spinner_start(GTK_SPINNER(g_spinner));
}
