#include "dialog.h"

void
collect_bookmark(GtkTreeIter iter, bookmark_list* bl)
{
	GtkTreeIter 	child;
	GtkTreePath*	path;

	do
	{
		path 			= gtk_tree_model_get_path(GTK_TREE_MODEL(model), &iter);
		bookmark* 	b 	= get_data(path);

		if(b)
		{
			if(strlen(bookmark_url(b)) > 1)
				bookmark_list_enqueue_bookmark(bl, b);

			bookmark_destroy(b);
		}

		if(gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(model), &child, &iter, 0))
			collect_bookmark(child, bl);
	}
	while(gtk_tree_model_iter_next(GTK_TREE_MODEL(model), &iter));

	gtk_tree_path_free(path);
}

void
tag_entry_set_text(GtkWidget* tag_box, GtkWidget* tag_entry)
{
	if(tag_entry && tag_box)
	{
		char* tag = gtk_combo_box_text_get_active_text	
			(GTK_COMBO_BOX_TEXT(tag_box));
		gtk_entry_set_text(GTK_ENTRY(tag_entry), tag);
	}
}

GtkWidget**
entries(gboolean editable)
{
	/* labels */
	GtkWidget* name_label = gtk_label_new("Name");
	gtk_widget_set_halign(GTK_WIDGET(name_label), GTK_ALIGN_START);

	GtkWidget* url_label = gtk_label_new("Url");
	gtk_widget_set_halign(GTK_WIDGET(url_label), GTK_ALIGN_START);

	GtkWidget* comment_label = gtk_label_new("Comment");
	gtk_widget_set_halign(GTK_WIDGET(comment_label), GTK_ALIGN_START);

	GtkWidget* tag_label = gtk_label_new("Tag");
	gtk_widget_set_halign(GTK_WIDGET(tag_label), GTK_ALIGN_START);

	/* entries */
	GtkWidget* name_entry = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(name_entry), "name");

	GtkWidget* url_entry = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(url_entry), "url");

	GtkWidget* comment_entry = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(comment_entry), "comment");

	GtkWidget* tag_entry = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(tag_entry), "tag");

	if(!editable)
	{
		gtk_editable_set_editable(GTK_EDITABLE(name_entry), FALSE);
		gtk_editable_set_editable(GTK_EDITABLE(url_entry), FALSE);
		gtk_editable_set_editable(GTK_EDITABLE(comment_entry), FALSE);
		gtk_editable_set_editable(GTK_EDITABLE(tag_entry), FALSE);
	}

	GtkWidget** r = g_new(GtkWidget*, 8);
	r[0] = name_label;
	r[1] = name_entry;
	r[2] = url_label;
	r[3] = url_entry;
	r[4] = comment_label;
	r[5] = comment_entry;
	r[6] = tag_label;
	r[7] = tag_entry;

	return r;
}

void  
close_window(GtkWidget* button, gpointer window) 
{
	gtk_widget_destroy(window);
}

void
dialog_key_press(GtkWidget* window, GdkEventKey* e)
{
	char* key = gdk_keyval_name(e->keyval);

	if(!strcmp(key, "Escape"))
		close_window(NULL, window);
}

GtkWidget*
dialogs(char* title, gpointer main_window)
{
	if(title && main_window)
	{
		GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title(GTK_WINDOW(window), title);
		gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
		gtk_window_set_transient_for(GTK_WINDOW(window)
			,GTK_WINDOW(main_window));

		g_signal_connect(window, "key-release-event"
			,G_CALLBACK(dialog_key_press), NULL);

		g_signal_connect(window, "destroy", G_CALLBACK(close_window)
			,window);

		return window;
	}
	
	return NULL;
}

