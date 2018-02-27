#include "searchbox.h"

GtkWidget*
search_box_new(GtkWidget* search_entry)
{
  /* search entry */
  GtkWidget* search_entry_label = gtk_label_new("Search:");
  gtk_widget_set_hexpand(GTK_WIDGET(search_entry), 1);

  /* search button */
  GtkWidget* search_button = gtk_button_new_with_mnemonic("_Go");

  g_signal_connect
    (GTK_WIDGET(search_button), "clicked", G_CALLBACK(read_database)
     ,search_entry);

  g_signal_connect
    (GTK_WIDGET(search_entry), "activate", G_CALLBACK(read_database)
     ,search_entry);

  /* search box */
  GtkWidget* search_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
  gtk_container_add(GTK_CONTAINER(search_box), search_entry_label);
  gtk_container_add(GTK_CONTAINER(search_box), search_entry);
  gtk_container_add(GTK_CONTAINER(search_box), search_button);

  return search_box;
}

void
search_entry_key_press(GtkWidget* window, GdkEventKey* e)
{
  if(!strcmp((gdk_keyval_name(e->keyval)), "Escape")) 
    gtk_widget_grab_focus(GTK_WIDGET(g_treeview));
}
