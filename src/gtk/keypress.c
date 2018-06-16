#include "keypress.h"

void
move_cursor(int direction, int position, gboolean* r)
{
  g_signal_emit_by_name
    (g_treeview, "move-cursor", position, direction, &r);

  gtk_tree_view_set_cursor_on_cell
    (GTK_TREE_VIEW(g_treeview), g_selected_path, NULL, NULL, FALSE);  
}

void
scroll_line(int direction)
{
  GtkAdjustment* adj = gtk_scrolled_window_get_hadjustment
    (GTK_SCROLLED_WINDOW(g_scrolled_window));

  gdouble value;

  if(direction)
    {
      value = gtk_adjustment_get_upper
	(GTK_ADJUSTMENT(adj));
    }
  else
    {
      value = gtk_adjustment_get_lower
	(GTK_ADJUSTMENT(adj));
    }

  gtk_adjustment_set_value(GTK_ADJUSTMENT(adj), value);
  gtk_tree_view_set_cursor
    (GTK_TREE_VIEW(g_treeview), g_selected_path, NULL, 0);
}

void
key_press(GtkWidget* window, GdkEventKey* e)
{
  char* key = gdk_keyval_name(e->keyval);
  gboolean r;

  //printf("%d %s\n", e->state, key);
  
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

      return;
    }

  else if(!strcmp(key, SCROLL_DOWN->key)
	  && e->state == SCROLL_DOWN->state)
    {
      move_cursor(1, GTK_MOVEMENT_DISPLAY_LINES, &r);
      return;
    }

  else if(!strcmp(key, SCROLL_UP->key)
	  && e->state == SCROLL_UP->state)
    {
      move_cursor(-1, GTK_MOVEMENT_DISPLAY_LINES, &r);
      return;
    }

  else if(!strcmp(key, SCROLL_LEFT->key)
	  && e->state == SCROLL_LEFT->state)
    {
      move_cursor(-1, GTK_MOVEMENT_LOGICAL_POSITIONS, &r);
      return;
    }
  
  else if(!strcmp(key, SCROLL_RIGHT->key)
	  && e->state == SCROLL_RIGHT->state)
    {
      move_cursor(1, GTK_MOVEMENT_LOGICAL_POSITIONS, &r);
      return;
    }

  else if(!strcmp(key, SCROLL_TOP->key)
	  && e->state == SCROLL_TOP->state)
    {
      move_cursor(-1, GTK_MOVEMENT_BUFFER_ENDS, &r);
      return;
    }

  else if(!strcmp(key, SCROLL_BOTTOM->key)
	  && e->state == SCROLL_BOTTOM->state)
    {
      move_cursor(1, GTK_MOVEMENT_BUFFER_ENDS, &r);

      gtk_tree_selection_unselect_all(GTK_TREE_SELECTION(g_selection));
      gtk_tree_view_set_cursor_on_cell
	(GTK_TREE_VIEW(g_treeview), g_selected_path, NULL, NULL, FALSE);  

      return;     
    }

  else if(!strcmp(key, SCROLL_PAGE_UP->key)
	  && e->state == SCROLL_PAGE_UP->state)  
    {
      move_cursor(-1, GTK_MOVEMENT_PAGES, &r);      
      return;
    }

  else if(!strcmp(key, SCROLL_PAGE_DOWN->key)
	  && e->state == SCROLL_PAGE_DOWN->state)  
    {
      move_cursor(1, GTK_MOVEMENT_PAGES, &r);
      return;
    }

  else if(!strcmp(key, SCROLL_END_LINE->key)
	  && e->state == SCROLL_END_LINE->state)  
    {
      scroll_line(1);
      return;
    }

  else if(!strcmp(key, SCROLL_BEGIN_LINE->key)
	  && e->state == SCROLL_BEGIN_LINE->state)  
    {
      scroll_line(0);
      return;
    }

  else if(!strcmp(key, EC->key)
	  && e->state == EC->state)
    {
      row_expander(g_selected_path, 0);
      return;
    }

  else if(!strcmp(key, EA->key)
	  && e->state == EA->state)
    {
      row_expander(g_selected_path, 1);
      return;
    }

  else if(!strcmp(key, COPY->key)
	  && e->state == COPY->state)
    {
      copy_to_clipboard();
      return;
    }

  else if(!strcmp(key, SEARCH_BAR->key)
	  && e->state == SEARCH_BAR->state)
    {
      gtk_widget_grab_focus(GTK_WIDGET(g_search_entry));
      return;
    }

  else if(!strcmp(key, RELOAD->key)
	  && e->state == RELOAD->state)
    {
      read_database(NULL, NULL);
      return;
    }

  else if(!strcmp(key, DELETE->key)
	  && e->state == DELETE->state)
    {
      delete(NULL);
      return;
    }

  else if(!strcmp(key, INSERT->key)
	  && e->state == INSERT->state)
    {
      add_window(NULL);
      return;
    }

  else if(!strcmp(key, EDIT->key)
	  && e->state == EDIT->state)
    {
      edit(NULL);
      return;
    }

  else if(!strcmp(key, RENAME->key)
	  && e->state == RENAME->state)
    {
      rename_directory_wrapper(NULL);
      return;
    }

  else if(!strcmp(key, OPEN_DATABASE->key)
	  && e->state == OPEN_DATABASE->state)
    {
      open_database(NULL);
      return;
    }

  else if(!strcmp(key, OPTIONS->key)
	  && e->state == OPTIONS->state)
    {
      options_window(NULL);
      return;
    }
}
