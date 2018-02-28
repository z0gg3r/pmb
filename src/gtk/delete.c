#include "delete.h"

static void
delete_bookmark(GtkWidget* button, gpointer** args) 
{
  char* url = (char*)gtk_entry_get_text(GTK_ENTRY(args[0]));

  if(url) 
    {
      bookmark_list* bl = bookmark_db_search(g_db, URL, url);
		
      if(bl) 
	{
	  bookmark* b = bookmark_list_return_next_bookmark(bl);

	  if(b) 
	    {
	      GtkTreeIter iter;
	      gtk_tree_model_get_iter
		(GTK_TREE_MODEL(g_model), &iter, g_selected_path);

	      gtk_tree_store_remove
		(GTK_TREE_STORE(g_bookmarks), &iter);

	      bookmark_db_delete(g_db, strtol(bookmark_id(b), NULL, 10));
	      bookmark_destroy(b);
	    }

	  bookmark_list_destroy(bl);
	}
    }

  close_window(NULL, args[1]);
  g_free(args);

  gboolean r;
  g_signal_emit_by_name
    (g_treeview, "move-cursor", GTK_MOVEMENT_DISPLAY_LINES, -1, &r);
  gtk_label_set_text(GTK_LABEL(g_info_label), "Delete: Done");
}

static void
delete_directory(GtkWidget* button, gpointer window) 
{
  GtkTreeIter iter, child;

  bookmark_list* bl = bookmark_list_new(); 
  bookmark* b = NULL;

  if(gtk_tree_model_get_iter(GTK_TREE_MODEL(g_model), &iter, g_selected_path))
    {
      if(gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(g_model), &child
				       ,&iter, 0))
	{
	  collect_bookmark(child, bl);
	}
    }

  while((b = bookmark_list_return_next_bookmark(bl)))
    {
      bookmark_db_delete(g_db, (strtol(bookmark_id(b), NULL, 10)));
      bookmark_destroy(b);
    }

  bookmark_list_destroy(bl);
  gtk_tree_store_remove(GTK_TREE_STORE(g_bookmarks), &iter);

  close_window(NULL, window);

  gboolean r;
  g_signal_emit_by_name
    (g_treeview, "move-cursor", GTK_MOVEMENT_DISPLAY_LINES, -1, &r);
  gtk_label_set_text(GTK_LABEL(g_info_label), "Delete Directory: Done");
}

static void
delete_multiple(GtkWidget* button, gpointer window)
{
  GList* rows = gtk_tree_selection_get_selected_rows
    (GTK_TREE_SELECTION(g_selection)
     ,&g_model);

  do
    {
      if(rows)
	{
	  bookmark* b = get_bookmark_from_row(rows->data);

	  if(strlen(bookmark_url(b)) > 1)
	    {
	      bookmark_db_delete(g_db, (strtol(bookmark_id(b), NULL, 10)));
	    }
	  else
	    {
	      g_selected_path = rows->data;	
	      delete_directory(button, window);
	    }

	  bookmark_destroy(b);
	}
      else
	{
	  break;
	}
    }
  while((rows = rows->next));

  g_list_free_full(rows, (GDestroyNotify) gtk_tree_path_free);
  close_window(NULL, window);
  read_database(NULL, NULL);
	
  gboolean r;
  g_signal_emit_by_name
    (g_treeview, "move-cursor", GTK_MOVEMENT_DISPLAY_LINES, -1, &r);
  gtk_label_set_text(GTK_LABEL(g_info_label), "Delete Multiple: Done");
}

static void
delete_bookmark_window(bookmark* b) 
{
  GtkWidget* window = dialog_new("Delete bookmark", gpmb_window);
  GtkWidget** e = entries(FALSE);

  if(b) 
    {
      if(bookmark_name(b))
	{
	  gtk_entry_set_text(GTK_ENTRY(e[1]), bookmark_name(b));
	}

      if(bookmark_url(b))
	{
	  gtk_entry_set_text(GTK_ENTRY(e[3]), bookmark_url(b));
	}

      if(bookmark_comment(b))
	{
	  gtk_entry_set_text
	    (GTK_ENTRY(e[5]), bookmark_comment(b));
	}

      if(bookmark_tag(b))
	{
	  gtk_entry_set_text(GTK_ENTRY(e[7]), bookmark_tag(b));
	}

      gtk_entry_set_icon_from_icon_name
	(GTK_ENTRY(e[7]), GTK_ENTRY_ICON_PRIMARY, "folder");

      /* set favicon */
      set_url_favicon(e[3]);

      bookmark_destroy(b);
    }

  GtkWidget** delete_bookmark_args = g_new(GtkWidget*, 2);
  delete_bookmark_args[0] = e[3];
  delete_bookmark_args[1] = window;

  /* button */
  GtkWidget* delete_button = gtk_button_new_with_mnemonic("_Delete");
  g_signal_connect
    (delete_button, "clicked", G_CALLBACK(delete_bookmark)
     ,delete_bookmark_args);

  GtkWidget* cancel_button = gtk_button_new_with_mnemonic("_Cancel");
  g_signal_connect
    (cancel_button, "clicked", G_CALLBACK(close_window)
     ,window);

  /* grid */
  GtkWidget* grid = grid_new();

  gtk_grid_attach(GTK_GRID(grid), e[0], 0,  0, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), e[1], 20, 0, 50, 1);
  gtk_grid_attach(GTK_GRID(grid), e[2], 0,  1, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), e[3], 20, 1, 50, 1);
  gtk_grid_attach(GTK_GRID(grid), e[4], 0,  2, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), e[5], 20, 2, 50, 1);
  gtk_grid_attach(GTK_GRID(grid), e[6], 0,  3, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), e[7], 20, 3, 50, 1);
  gtk_grid_attach(GTK_GRID(grid), delete_button, 20, 4, 20, 10);
  gtk_grid_attach(GTK_GRID(grid), cancel_button, 40, 4, 20, 10);

  g_free(e);
  gtk_container_add(GTK_CONTAINER(window), grid);
  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_widget_grab_focus(GTK_WIDGET(cancel_button));
  gtk_spinner_start(GTK_SPINNER(g_spinner));
}

static void
delete_directory_window(bookmark* b)
{
  GtkWidget* window = dialog_new("Delete directory", gpmb_window);

  GtkWidget* name_entry_label = gtk_label_new("Name");
  gtk_widget_set_halign(GTK_WIDGET(name_entry_label), GTK_ALIGN_START);

  GtkWidget* name_entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(name_entry), "name");
  gtk_editable_set_editable(GTK_EDITABLE(name_entry), FALSE);

  if(b)
    {
      gtk_entry_set_text(GTK_ENTRY(name_entry), get_full_path(b));
      bookmark_destroy(b);
    }

  /* button */
  GtkWidget* delete_button = gtk_button_new_with_mnemonic("_Delete");
  g_signal_connect
    (delete_button, "clicked", G_CALLBACK(delete_directory), window);

  GtkWidget* cancel_button = gtk_button_new_with_mnemonic("_Cancel");
  g_signal_connect
    (cancel_button, "clicked", G_CALLBACK(close_window), window);

  /* grid */
  GtkWidget* grid = grid_new();

  gtk_grid_attach(GTK_GRID(grid), name_entry_label, 0,  0, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), name_entry, 20, 0, 50, 1);
  gtk_grid_attach(GTK_GRID(grid), delete_button, 20, 1, 20, 10);
  gtk_grid_attach(GTK_GRID(grid), cancel_button, 40, 1, 20, 10);

  gtk_container_add(GTK_CONTAINER(window), grid);
  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_widget_grab_focus(GTK_WIDGET(cancel_button));
  gtk_spinner_start(GTK_SPINNER(g_spinner));
}

static void
delete_multiple_window()
{
  GtkWidget* window = dialog_new("Delete multiple bookmarks", gpmb_window);
  GtkWidget* advice = gtk_label_new
    ("Delete all selected bookmarks/directories?");

  /* button */
  GtkWidget* delete_button = gtk_button_new_with_mnemonic("_Delete");
  g_signal_connect
    (delete_button, "clicked", G_CALLBACK(delete_multiple)
     ,window);

  GtkWidget* cancel_button = gtk_button_new_with_mnemonic("_Cancel");
  g_signal_connect
    (cancel_button, "clicked", G_CALLBACK(close_window)
     ,window);

  /* grid */
  GtkWidget* grid = grid_new();

  gtk_grid_attach(GTK_GRID(grid), advice, 0,  0, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), delete_button, 0,  1, 20, 10);
  gtk_grid_attach(GTK_GRID(grid), cancel_button, 20, 1, 20, 10);

  gtk_container_add(GTK_CONTAINER(window), grid);
  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_widget_grab_focus(GTK_WIDGET(cancel_button));
  gtk_spinner_start(GTK_SPINNER(g_spinner));
}

void
delete(GtkWidget* button)
{
  if(gtk_tree_selection_count_selected_rows
     (GTK_TREE_SELECTION(g_selection)) > 1)
    {
      delete_multiple_window();
    }
  else
    {
      bookmark* b = get_bookmark_from_row(NULL);

      if(b)
	{
	  if(strlen(bookmark_url(b)) > 1)
	    {
	      delete_bookmark_window(b);
	    }
	  else
	    {
	      delete_directory_window(b);
	    }
	}
    }
}
