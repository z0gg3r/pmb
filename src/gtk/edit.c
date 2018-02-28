#include "edit.h"

static void
move_directory(char* tag)
{
  GtkTreeIter iter, iter_copy, child, parent;

  bookmark_list* bl = bookmark_list_new(); 
  bookmark* b = NULL;
  bookmark* sb = get_data(NULL);
  unsigned int parents = 0;

  if(gtk_tree_model_get_iter(GTK_TREE_MODEL(g_model), &iter, g_selected_path))
    {
      iter_copy = iter;

      /* count number of parents */
      while(gtk_tree_model_iter_parent
	    (GTK_TREE_MODEL(g_model), &parent, &iter_copy))
	{
	  parents++;
	  iter_copy = parent;
	}

      /* collect children, if any */
      if(gtk_tree_model_iter_nth_child
	 (GTK_TREE_MODEL(g_model), &child, &iter, 0))
	{
	  collect_bookmark(child, bl);
	}
    }

  /* start moving */
  while((b = bookmark_list_return_next_bookmark(bl)))
    {
      unsigned int id = strtol(bookmark_id(b), NULL, 10);
      char* bm_tag = bookmark_tag(b);
      char* bm_tag_bkp = strdup(bm_tag);

      if(parents > 0)
	{
	  strsep(&bm_tag, "//");
	}

      if(!bm_tag)
	{
	  bookmark_db_edit(g_db, id, 3, tag);
	}

      else if(strlen(tag) < 2)
	{
	  char* n_tag = NULL;

	  while(bm_tag)
	    {
	      n_tag = strsep(&bm_tag, "//");

	      if(!(strcmp(n_tag, bookmark_id(sb))))
		{
		  break;
		}
	    }

	  if(bm_tag)
	    {
	      char* new_tag = calloc
		(1, (strlen(n_tag) + strlen(bm_tag) + 2) * sizeof(char));

	      snprintf
		(new_tag, strlen(new_tag) - 1, "%s/%s", n_tag, bm_tag);

	      bookmark_db_edit(g_db, id, 3, new_tag);
	      free(new_tag);
	    }
	  else
	    {
	      bookmark_db_edit(g_db, id, 3, n_tag);
	    }
	}
      else
	{
	  if(parents > 1)
	    {
	      char* res	= NULL;
	      
	      while(strcmp(bm_tag, bookmark_id(sb)))
		{
		  res = strsep(&bm_tag, "//");

		  if(!(strcmp(res, bookmark_id(sb))))
		    {
		      break;
		    }
		}
	    }

	  char* new_tag = calloc
	    (1, (strlen(tag) + strlen(bm_tag)+ 2) * sizeof(char));

	  snprintf
	    (new_tag, strlen(new_tag) - 1, "%s/%s", tag, bm_tag);

	  bookmark_db_edit(g_db, id, 3, new_tag);
	  free(new_tag);
	}

      free(bm_tag_bkp);
      bookmark_destroy(b);
    }
	
  bookmark_destroy(sb);
  bookmark_destroy(b);
  bookmark_list_destroy(bl);
}

static void
rename_directory(GtkWidget* button, gpointer** args)
{
  bookmark* b = get_data(NULL);
  bookmark* bt = NULL;
  char* path = get_full_path(b);
  
  bookmark_list* bl = bookmark_db_search(g_db, TAG, path);
  char* input_tag = (char*)gtk_entry_get_text(GTK_ENTRY(args[0]));
  char*	full_tag = get_full_path(b);
  char*	root_tag = NULL;
  char*	new_tag	= NULL;

  /* assembly the tag */
  while(full_tag && strcmp(full_tag, bookmark_id(b)))
    {
      char* full_tag_sep = strsep(&full_tag, "//");
		
      if(root_tag)
	{
	  char* r_tag_bkp = strdup(root_tag);
	  unsigned int size =
	    (strlen(r_tag_bkp) + strlen(full_tag_sep) + 2)
	    * sizeof(char);

	  root_tag = realloc(root_tag, size);

	  snprintf(root_tag, size, "%s/%s", r_tag_bkp, full_tag_sep);
	  free(r_tag_bkp);
	}
      else
	{
	  root_tag = calloc(1, strlen(full_tag_sep) + 1 * sizeof(char));

	  strcpy(root_tag, full_tag_sep);
	}
    }

  if(root_tag)
    {
      unsigned int new_tag_size =
	(strlen(root_tag) + strlen(input_tag) + 2) * sizeof(char);

      new_tag = calloc(1, new_tag_size);

      snprintf(new_tag, new_tag_size, "%s/%s", root_tag, input_tag);
      free(root_tag);
    }
  else
    {
      new_tag = calloc(1, (strlen(input_tag) + 1) * sizeof(char));
      strcpy(new_tag, input_tag);
    }

  /* start renaming */
  if(strlen(new_tag) > 1)
    {	
      while((bt = bookmark_list_return_next_bookmark(bl)))
	{
	  char* new_tag_bkp = strdup(new_tag);
	  char* b_tag = bookmark_tag(bt);
	  char* nt_sep = strsep(&new_tag_bkp, "//");
	  char* tag_sep = strsep(&b_tag, "//");
	  char* tag_root = NULL;

	  if(nt_sep)
	    {
	      tag_root = realloc
		(tag_root, (strlen(nt_sep) + 1) * sizeof(char));

	      strcpy(tag_root, nt_sep);
	    }

	  while(b_tag && new_tag_bkp && !(strcmp(nt_sep, tag_sep)))
	    {
	      nt_sep = strsep(&new_tag_bkp, "//");
	      tag_sep = strsep(&b_tag, "//");
	      char* tr_bkp = strdup(tag_root);
	      unsigned int size =
		(strlen(tr_bkp) + strlen(nt_sep) + 2) * sizeof(char);

	      tag_root = realloc(tag_root, size); 

	      snprintf(tag_root, size - 1, tr_bkp, nt_sep);
	      free(tr_bkp);
	    }

	  if(b_tag)
	    {
	      unsigned int size =
		(strlen(new_tag) + strlen(b_tag) + 2) * sizeof(char);

	      char* tag = calloc(1, size);

	      snprintf(tag, size, "%s/%s", new_tag, b_tag);

	      if(tag)
		{
		  bookmark_db_edit
		    (g_db, strtol(bookmark_id(bt), NULL, 10), 3, tag);

		  free(tag);
		}
	    }
	  else
	    {
	      bookmark_db_edit
		(g_db, strtol(bookmark_id(bt), NULL, 10), 3, new_tag);
	    }

	  bookmark_destroy(bt);
	  gtk_label_set_text(GTK_LABEL(g_info_label), "Rename: Done");
	}
    }
  else
    {
      gtk_label_set_text
	(GTK_LABEL(g_info_label), "Rename: Error, name field empty");
    }

  free(new_tag);
  bookmark_destroy(b);
  bookmark_list_destroy(bl);
  close_window(NULL, args[1]);
  g_free(args);
  read_database(NULL, NULL);
}

static void
edit_bookmark(GtkWidget* button, gpointer** args) 
{
  char* name = (char*)gtk_entry_get_text(GTK_ENTRY(args[0]));
  char* url = (char*)gtk_entry_get_text(GTK_ENTRY(args[1]));
  char* comment = (char*)gtk_entry_get_text(GTK_ENTRY(args[2]));
  char* tag = (char*)gtk_combo_box_text_get_active_text
    (GTK_COMBO_BOX_TEXT(args[3]));
  char* message	= NULL;

  if(url) 
    {
      bookmark_list* bl = bookmark_db_search(g_db, URL, url);
		
      if(bl) 
	{
	  bookmark* b = bookmark_list_return_next_bookmark(bl);

	  if(b) 
	    {
	      unsigned int id = strtol(bookmark_id(b), NULL, 10);

	      if(name)
		{
		  bookmark_db_edit(g_db, id, 0, name);
		}

	      if(url)
		{
		  bookmark_db_edit(g_db, id, 1, url);
		}

	      if(comment)
		{
		  bookmark_db_edit(g_db, id, 2, comment);
		}

	      if(tag)
		{
		  bookmark_db_edit(g_db, id, 3, tag);
		}

	      unsigned int size = (strlen(bookmark_id(b))
				   + strlen(bookmark_url(b)) 
				   + strlen("edit: id = , url = ")
				   + 3) * sizeof(char);

	      message = calloc(1, size);

	      snprintf
		(message, size - 1, "edit: id = %s, url = %s"
		 ,bookmark_id(b), bookmark_url(b));

	      gtk_label_set_text(GTK_LABEL(g_info_label), message);

	      bookmark_destroy(b);
	    }

	  bookmark_list_destroy(bl);
	}
    }

  close_window(NULL, args[4]);
  g_free(args);
  read_database(NULL, NULL);
  free(message);
}

static void
move_directory_wrapper(GtkWidget* button, gpointer** args) 
{
  move_directory(((char*)gtk_combo_box_text_get_active_text
		  (GTK_COMBO_BOX_TEXT(args[0]))));
  close_window(NULL, args[1]);
  g_free(args);
  read_database(NULL, NULL);

  gboolean r;
  g_signal_emit_by_name
    (g_treeview, "move-cursor", GTK_MOVEMENT_DISPLAY_LINES, -1, &r);

  gtk_label_set_text(GTK_LABEL(g_info_label), "Move: Done");
}

static void
move_multiple(GtkWidget* button, gpointer** args)
{
  char* tag = (char*)gtk_combo_box_text_get_active_text
    (GTK_COMBO_BOX_TEXT(args[0]));
  
  GList* rows = gtk_tree_selection_get_selected_rows
    (GTK_TREE_SELECTION(g_selection), &g_model);

  do
    {
      if(rows)
	{
	  bookmark* b = get_data(rows->data);

	  if(strlen(bookmark_url(b)) > 1)
	    {
	      bookmark_db_edit
		(g_db, (strtol(bookmark_id(b), NULL, 10)), 3, tag);
	    }
	  else
	    {
	      g_selected_path = rows->data;	
	      move_directory(tag);
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
  close_window(NULL, args[1]);
  g_free(args);
  read_database(NULL, NULL);

  gboolean r;
  g_signal_emit_by_name
    (g_treeview, "move-cursor", GTK_MOVEMENT_DISPLAY_LINES, -1, &r);
  
  gtk_label_set_text(GTK_LABEL(g_info_label), "Move Multiple: Done");
}

static void
edit_delete_favicon()
{
  bookmark* b = get_data(NULL);

  bookmark_db_edit(g_db, strtol(bookmark_id(b), NULL, 10), 4, "none");
  read_database(NULL, NULL);
  bookmark_destroy(b);
}

static void
edit_download_favicon()
{
  bookmark* b = get_data(NULL);
  char* favicon = download_favicon(bookmark_url(b));

  if(favicon)
    {
      bookmark_db_edit(g_db, strtol(bookmark_id(b), NULL, 10), 4, favicon);
      read_database(NULL, NULL);
      bookmark_destroy(b);
    }
}

static void
edit_bookmark_window(bookmark* b) 
{
  GtkWidget* window = dialogs("Edit bookmark", gpmb_window);
  GtkWidget** e = entries(TRUE);
  GtkWidget* tag_box = tag_box_new();

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
	  gtk_entry_set_text(GTK_ENTRY(e[5]), bookmark_comment(b));
	}

      /* tag */
      GtkWidget* tag_entry = gtk_bin_get_child(GTK_BIN(tag_box));
      gtk_entry_set_text(GTK_ENTRY(tag_entry), get_full_path(b));
      gtk_entry_set_icon_from_icon_name
	(GTK_ENTRY(tag_entry), GTK_ENTRY_ICON_PRIMARY, "folder");

      /* set favicon */
      set_url_favicon(e[3]);
      bookmark_destroy(b);
    }

  GtkWidget** edit_bookmark_args = g_new(GtkWidget*, 5);
  edit_bookmark_args[0] = e[1];
  edit_bookmark_args[1] = e[3];
  edit_bookmark_args[2] = e[5];
  edit_bookmark_args[3] = tag_box;
  edit_bookmark_args[4] = window;

  /* button */
  GtkWidget* edit_button = gtk_button_new_with_mnemonic("_Edit");
  g_signal_connect
    (edit_button, "clicked", G_CALLBACK(edit_bookmark), edit_bookmark_args);

  GtkWidget* cancel_button = gtk_button_new_with_mnemonic("_Cancel");
  g_signal_connect
    (cancel_button, "clicked", G_CALLBACK(close_window), window);

  /* favicon buttons */
  GtkWidget* clean_label = gtk_label_new("Favicon");
  gtk_widget_set_halign(GTK_WIDGET(clean_label), GTK_ALIGN_START);

  GtkWidget* clean_button = gtk_button_new_with_mnemonic("C_lean");
  g_signal_connect
    (GTK_WIDGET(clean_button), "clicked", G_CALLBACK(edit_delete_favicon)
     ,NULL);

  GtkWidget* download_button = gtk_button_new_with_mnemonic("Do_wnload");
  g_signal_connect
    (GTK_WIDGET(download_button), "clicked", G_CALLBACK(edit_download_favicon)
     ,NULL);

  /* grid */
  GtkWidget* grid = grid_new();

  gtk_grid_attach(GTK_GRID(grid), e[0], 0,  0, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), e[1], 20, 0, 50, 1);
  gtk_grid_attach(GTK_GRID(grid), e[2], 0,  1, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), e[3], 20, 1, 50, 1);
  gtk_grid_attach(GTK_GRID(grid), e[4], 0,  2, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), e[5], 20, 2, 50, 1);
  gtk_grid_attach(GTK_GRID(grid), e[6], 0,  3, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), tag_box, 20, 3, 50, 1);
  gtk_grid_attach(GTK_GRID(grid), clean_label, 0,  4, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), download_button, 20, 4, 25, 1);
  gtk_grid_attach(GTK_GRID(grid), clean_button, 45, 4, 25, 1);
  gtk_grid_attach(GTK_GRID(grid), edit_button, 20, 5, 20, 10);
  gtk_grid_attach(GTK_GRID(grid), cancel_button, 40, 5, 20, 10);

  g_free(e);
  gtk_container_add(GTK_CONTAINER(window), grid);
  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_spinner_start(GTK_SPINNER(g_spinner));
}

static void
move_directory_window(bookmark* b)
{
  GtkWidget* window = dialogs("Move directory", gpmb_window);
  GtkWidget* tag_box = tag_box_new();

  GtkWidget* name_entry_label = gtk_label_new("Move to");
  gtk_widget_set_halign(GTK_WIDGET(name_entry_label), GTK_ALIGN_START);

  if(b)
    {
      GtkWidget* tag_entry = gtk_bin_get_child(GTK_BIN(tag_box));
      gtk_entry_set_text(GTK_ENTRY(tag_entry), get_full_path(b));
      bookmark_destroy(b);
    }

  GtkWidget** move_directory_args = g_new(GtkWidget*, 2);
  move_directory_args[0] = tag_box;
  move_directory_args[1] = window;

  /* button */
  GtkWidget* edit_button = gtk_button_new_with_mnemonic("_Move");
  g_signal_connect
    (edit_button, "clicked", G_CALLBACK(move_directory_wrapper)
     ,move_directory_args);

  GtkWidget* cancel_button = gtk_button_new_with_mnemonic("_Cancel");
  g_signal_connect
    (cancel_button, "clicked", G_CALLBACK(close_window), window);

  /* grid */
  GtkWidget* grid = grid_new();

  gtk_grid_attach(GTK_GRID(grid), name_entry_label, 0,  0, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), tag_box, 20, 0, 50, 1);
  gtk_grid_attach(GTK_GRID(grid), edit_button, 20, 1, 20, 10);
  gtk_grid_attach(GTK_GRID(grid), cancel_button, 40, 1, 20, 10);

  gtk_container_add(GTK_CONTAINER(window), grid);
  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_spinner_start(GTK_SPINNER(g_spinner));
}

static void
move_multiple_window()
{
  GtkWidget* window = dialogs("Move multiple", gpmb_window);
  GtkWidget* tag_box = tag_box_new();

  GtkWidget* name_entry_label = gtk_label_new("Move to");
  gtk_widget_set_halign(GTK_WIDGET(name_entry_label), GTK_ALIGN_START);

  GtkWidget** move_multiple_args = g_new(GtkWidget*, 2);
  move_multiple_args[0] = tag_box;
  move_multiple_args[1] = window;

  /* button */
  GtkWidget* edit_button = gtk_button_new_with_mnemonic("_Edit");
  g_signal_connect
    (edit_button, "clicked", G_CALLBACK(move_multiple)
     ,move_multiple_args);

  GtkWidget* cancel_button = gtk_button_new_with_mnemonic("_Cancel");
  g_signal_connect
    (cancel_button, "clicked", G_CALLBACK(close_window), window);

  /* grid */
  GtkWidget* grid = grid_new();

  gtk_grid_attach(GTK_GRID(grid), name_entry_label, 0,  0, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), tag_box, 20, 0, 50, 1);
  gtk_grid_attach(GTK_GRID(grid), edit_button, 20, 1, 20, 10);
  gtk_grid_attach(GTK_GRID(grid), cancel_button, 40, 1, 20, 10);

  gtk_container_add(GTK_CONTAINER(window), grid);
  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_spinner_start(GTK_SPINNER(g_spinner));
}

static void
rename_directory_window(bookmark* b)
{
  GtkWidget* window = dialogs("Rename directory", gpmb_window);

  /* name entry */
  GtkWidget* name_entry_label = gtk_label_new("New name");
  gtk_widget_set_halign(GTK_WIDGET(name_entry_label), GTK_ALIGN_START);

  GtkWidget* name_entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(name_entry), "new name");

  if(b)
    {
      gtk_entry_set_text(GTK_ENTRY(name_entry), bookmark_id(b));
      bookmark_destroy(b);
    }

  GtkWidget** rename_multiple_args = g_new(GtkWidget*, 2);
  rename_multiple_args[0] = name_entry;
  rename_multiple_args[1] = window;

  /* button */
  GtkWidget* edit_button = gtk_button_new_with_mnemonic("_Rename");
  g_signal_connect
    (edit_button, "clicked", G_CALLBACK(rename_directory)
     ,rename_multiple_args);

  GtkWidget* cancel_button = gtk_button_new_with_mnemonic("_Cancel");
  g_signal_connect
    (cancel_button, "clicked", G_CALLBACK(close_window), window);

  /* grid */
  GtkWidget* grid = grid_new();

  gtk_grid_attach(GTK_GRID(grid), name_entry_label, 0,  0, 30, 1);
  gtk_grid_attach(GTK_GRID(grid), name_entry, 20, 0, 50, 1);
  gtk_grid_attach(GTK_GRID(grid), edit_button, 20, 1, 20, 10);
  gtk_grid_attach(GTK_GRID(grid), cancel_button, 40, 1, 20, 10);

  gtk_container_add(GTK_CONTAINER(window), grid);
  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_spinner_start(GTK_SPINNER(g_spinner));
}

void
rename_directory_wrapper(GtkWidget* button)
{
  bookmark* b = get_data(NULL);

  if((strlen(bookmark_url(b)) < 2))
    {
      rename_directory_window(b);
    }
  else
    {
      edit_bookmark_window(b);
    }
}

void
edit(GtkWidget* button)
{
  if(gtk_tree_selection_count_selected_rows
     (GTK_TREE_SELECTION(g_selection)) > 1)
    {
      move_multiple_window();
    }
  else
    {
      bookmark* b = get_data(NULL);

      if(b)
	{
	  if(strlen(bookmark_url(b)) > 1)
	    {
	      edit_bookmark_window(b);
	    }
	  else
	    {
	      move_directory_window(b);
	    }
	}
    }
}
