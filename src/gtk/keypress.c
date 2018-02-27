#include "keypress.h"

void
key_press(GtkWidget* window, GdkEventKey* e)
{
  char* key = gdk_keyval_name(e->keyval);
  gboolean r;
  //guint state = e->state;
  //printf("%s\n", key);

  if(!strcmp(key, "Down") 
     ||(!strcmp(key, "Up"))
     ||(!strcmp(key, "Right"))	
     ||(!strcmp(key, "Left"))
     ||(!strcmp(key, "Page_Up"))
     ||(!strcmp(key, "Page_Down"))
     ||(!strcmp(key, "Home"))
     ||(!strcmp(key, "End")))
    {
      gtk_tree_view_set_cursor
	(GTK_TREE_VIEW(g_treeview), g_selected_path, NULL, 0);
    }

  else if(!strcmp(key, "j"))
    {
      g_signal_emit_by_name
	(g_treeview, "move-cursor", GTK_MOVEMENT_DISPLAY_LINES, 1, &r);
    }

  else if(!strcmp(key, "k"))
    {
      g_signal_emit_by_name
	(g_treeview, "move-cursor", GTK_MOVEMENT_DISPLAY_LINES, -1, &r);
    }

  else if(!strcmp(key, "h"))
    {
      g_signal_emit_by_name
	(g_treeview, "move-cursor", GTK_MOVEMENT_LOGICAL_POSITIONS, -1, &r);
    }

  else if(!strcmp(key, "l"))
    {
      g_signal_emit_by_name
	(g_treeview, "move-cursor", GTK_MOVEMENT_LOGICAL_POSITIONS, 1, &r);
    }

  else if(!strcmp(key, "g"))
    {
      g_signal_emit_by_name
	(g_treeview, "move-cursor", GTK_MOVEMENT_BUFFER_ENDS, -1, &r);
    }

  else if(!strcmp(key, "G"))
    {
      g_signal_emit_by_name
	(g_treeview, "move-cursor", GTK_MOVEMENT_BUFFER_ENDS, 1, &r);

      gtk_tree_selection_unselect_all(GTK_TREE_SELECTION(g_selection));
      gtk_tree_selection_select_path
	(GTK_TREE_SELECTION(g_selection), g_selected_path);
    }

  else if(!strcmp(key, "b"))
    {
      g_signal_emit_by_name
	(g_treeview, "move-cursor", GTK_MOVEMENT_PAGES, -1, &r);
    }

  else if(!strcmp(key, "f"))
    {
      g_signal_emit_by_name
	(g_treeview, "move-cursor", GTK_MOVEMENT_PAGES, 1, &r);
    }

  else if(!strcmp(key, "dollar"))
    {
      GtkAdjustment* adj = gtk_scrolled_window_get_hadjustment
	(GTK_SCROLLED_WINDOW(s_window));
      
      gdouble upper = gtk_adjustment_get_upper
	(GTK_ADJUSTMENT(adj));

      gtk_adjustment_set_value(GTK_ADJUSTMENT(adj), upper);
      gtk_tree_view_set_cursor(GTK_TREE_VIEW(g_treeview)
			       ,g_selected_path, NULL, 0);	
    }

  else if((!strcmp(key, "0"))
	  ||(!strcmp(key, "bar")))
    {
      GtkAdjustment* adj = gtk_scrolled_window_get_hadjustment
	(GTK_SCROLLED_WINDOW(s_window));
      
      gdouble lower = gtk_adjustment_get_lower
	(GTK_ADJUSTMENT(adj));

      gtk_adjustment_set_value(GTK_ADJUSTMENT(adj), lower);
      gtk_tree_view_set_cursor
	(GTK_TREE_VIEW(g_treeview), g_selected_path, NULL, 0);
    }

  else if(!strcmp(key, "L")) 
    {
      if(e->state == 5)
	{
	  gtk_tree_view_expand_row
	    (GTK_TREE_VIEW(g_treeview), g_selected_path, TRUE);
	}
      else
	{
	  gtk_tree_view_expand_row
	    (GTK_TREE_VIEW(g_treeview), g_selected_path, FALSE);
	}
    }

  else if(!strcmp(key, "H"))
    {
      gtk_tree_view_collapse_row(GTK_TREE_VIEW(g_treeview), g_selected_path);
    }

  else if(!strcmp(key, "x"))
    {
      row_expander(g_selected_path, 0);
    }

  else if(!strcmp(key, "X"))
    {
      row_expander(g_selected_path, 1);
    }

  else if(!strcmp(key, "c"))
    {
      copy_to_clipboard();
    }

  else if(!strcmp(key, "s"))
    {
      gtk_widget_grab_focus(GTK_WIDGET(search_entry));
    }

  else if(!strcmp(key, "r"))
    {
      read_database(NULL, NULL);
    }

  else if(!strcmp(key, "d"))
    {
      delete(NULL);
    }

  else if(!strcmp(key, "i"))
    {
      add_window(NULL);
    }

  else if(!strcmp(key, "e"))
    {
      edit(NULL);
    }
	
  else if(!strcmp(key, "n"))
    {
      rename_directory_wrapper(NULL);
    }

  else if(!strcmp(key, "o"))
    {
      open_database(NULL);
    }

  else if(!strcmp(key, "O"))
    {
      options_window(NULL);
    }
}
