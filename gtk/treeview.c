#include "treeview.h"

void
copy_to_clipboard()
{
	GtkClipboard* 	clip	= gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
	GtkClipboard* 	primary	= gtk_clipboard_get(GDK_SELECTION_PRIMARY);
	bookmark* 	b 	= get_data(NULL);

	if(b)
	{
		char*	url	= bookmark_url(b);

		if(url)
		{
			gtk_clipboard_set_text(clip, url, -1);
			gtk_clipboard_set_text(primary, url, -1);
		}

		bookmark_destroy(b);
	}

	g_object_ref_sink(G_OBJECT(clip));
	g_object_unref(G_OBJECT(clip));

	g_object_ref_sink(G_OBJECT(primary));
	g_object_unref(G_OBJECT(primary));
}

static GtkCellRenderer*
cell_renderer_new(char* color) 
{
	GdkRGBA	gcolor;
	GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
	gdk_rgba_parse(&gcolor, color);
	g_object_set(G_OBJECT(renderer)
		,"foreground-rgba", &gcolor
		,NULL);

	return renderer;
}

/* -- update selected_path and selected_column globals -- */
static void
update_selected_row(GtkWidget* tree_view) 
{
	GtkTreePath* 	path 	= gtk_tree_path_new();

	gtk_tree_view_get_cursor(GTK_TREE_VIEW(tree_view), &path, NULL);

	gtk_tree_path_free(selected_path);

	selected_path 		= path;
}

static void
tree_store_add_child(GtkTreeIter* iter, directory* child)
{
	short		icon_size 	= GTK_ICON_SIZE_MENU;
	GtkIconTheme*	theme		= gtk_icon_theme_get_default();

	GdkPixbuf*	folder		= gtk_icon_theme_load_icon(
						GTK_ICON_THEME(theme)
						,"folder"
						,icon_size
						,0
						,NULL); 

	GdkPixbuf*	star		= gtk_icon_theme_load_icon(
						GTK_ICON_THEME(theme)
						,"starred"
						,icon_size
						,0
						,NULL); 

	if(!iter && child)
	{
		GtkTreeIter iter;

		directory_rewind(child);

		gtk_tree_store_append(bookmarks, &iter, NULL);

		gtk_tree_store_set(bookmarks, &iter, 0, directory_name(child)
			,-1);

		gtk_tree_store_set(bookmarks, &iter, 5, folder, -1);

		tree_store_add_child(&iter, child);
	}

	else if(iter && child)
	{
		directory* 	d 	= NULL;
		bookmark*  	b 	= NULL;

		while((d = directory_next_children(child)))
		{
			GtkTreeIter d_iter;

			directory_rewind(d);

			gtk_tree_store_append(bookmarks, &d_iter, iter);

			gtk_tree_store_set(bookmarks, &d_iter, 0	
				,directory_name(d), -1);

			gtk_tree_store_set(bookmarks, &d_iter, 5, folder, -1);

			tree_store_add_child(&d_iter, d);
		}

		while((b = directory_next_bookmark(child)))
		{
			GtkTreeIter b_iter;

			gtk_tree_store_append(bookmarks, &b_iter, iter);

			gtk_tree_store_set(bookmarks, &b_iter, 0
				,bookmark_id(b), -1);

			gtk_tree_store_set(bookmarks, &b_iter, 1
				,bookmark_name(b), -1);

			gtk_tree_store_set(bookmarks, &b_iter, 2
				,bookmark_url(b), -1);

			gtk_tree_store_set(bookmarks, &b_iter, 3
				,bookmark_comment(b), -1);

			gtk_tree_store_set(bookmarks, &b_iter, 4
				,bookmark_tag(b), -1);

			gtk_tree_store_set(bookmarks, &b_iter, 5
				,star, -1);

			//bookmark_destroy(b);
		}

		if(d)
			directory_destroy(d);

		if(b)
			bookmark_destroy(b);
	}
	
	g_object_unref(G_OBJECT(folder));
	g_object_unref(G_OBJECT(star));
}

void 
read_database(GtkWidget* button, GtkWidget* entry) 
{
	bookmark_list* 	bl = NULL;

	gtk_tree_store_clear(bookmarks);

	if(entry) 
	{
		if(entry) 
		{
			char* tag = (char*)gtk_entry_get_text(GTK_ENTRY
				(entry));

			if(strlen(tag) > 1)
				bl = bookmark_db_search(db, NULL ,tag);
			else
				goto get_all;
		}
		else
			goto get_all;
	}
	else 
	{
		get_all:
		bl = bookmark_db_query(db, 0, NULL);
	}

	if(bl) 
	{
		directory*	child	= NULL;
		directory* 	root 	= create_tree_from_bookmark_list(bl
						,"root");

		if(root)
		{
			directory_rewind(root);

			while((child = directory_next_children(root)))
				tree_store_add_child(NULL, child);

			if(child)
				directory_destroy(child);

			directory_destroy(root);
		}

		bookmark_list_destroy(bl);
	}

	/* focus n the first tree view item */
	GtkTreeIter iter;
	gtk_tree_model_get_iter_first(GTK_TREE_MODEL(model), &iter);

	GtkTreePath* path = gtk_tree_model_get_path(GTK_TREE_MODEL(model)
		,&iter);

	gtk_tree_view_set_cursor(GTK_TREE_VIEW(treeview), path, NULL, 0);
	gtk_tree_path_free(path);
}

bookmark*
get_data(GtkTreePath* path) 
{
	if(!path)
		path = selected_path;

	if(path) 
	{
		GtkTreeIter iter;
		gtk_tree_model_get_iter(GTK_TREE_MODEL(model), &iter
			,path);

		bookmark* b = bookmark_new();

		if(b) 
		{
			char* temp[4];

			gtk_tree_model_get(GTK_TREE_MODEL(model), &iter
				,0, &temp[0]
				,1, &temp[1]
				,2, &temp[2]
				,3, &temp[3]
				,4, &temp[4]
				,-1);

			if(temp[0])
				bookmark_set_id(b, temp[0]);
			else
				bookmark_set_id(b, "");

			if(temp[1])
				bookmark_set_name(b, temp[1]);
			else
				bookmark_set_name(b, "");

			if(temp[2])
				bookmark_set_url(b, temp[2]);
			else
				bookmark_set_url(b, "");

			if(temp[3])
				bookmark_set_comment(b, temp[3]);
			else
				bookmark_set_comment(b, "");

			if(temp[4])
				bookmark_set_tag(b, temp[4]);
			else
				bookmark_set_tag(b, "");

			return b;
		}
		else
			return NULL;
	}
	else
		return NULL;
}

GtkWidget*
tree_view(GtkWidget* search_entry) 
{
	GtkCellRenderer* cell_renderer_text;

	/* id/tag */
	cell_renderer_text = cell_renderer_new(opts->id_fg);

	/* directory icon */
	GtkCellRenderer* dir_icon = gtk_cell_renderer_pixbuf_new();;

	/* dir column */
	GtkTreeViewColumn* dir_column = gtk_tree_view_column_new();

	gtk_tree_view_column_pack_start(GTK_TREE_VIEW_COLUMN(dir_column)
		,dir_icon, FALSE);

	gtk_tree_view_column_pack_start(GTK_TREE_VIEW_COLUMN(dir_column)
		,cell_renderer_text, FALSE);

	gtk_tree_view_column_add_attribute(GTK_TREE_VIEW_COLUMN(dir_column)
		,cell_renderer_text, "text", 0);

	gtk_tree_view_column_add_attribute(GTK_TREE_VIEW_COLUMN(dir_column)
		,dir_icon, "pixbuf", 5);

	g_object_set(G_OBJECT(dir_column), "title", "Tag/Id", NULL);

	/* name column */
	cell_renderer_text = cell_renderer_new(opts->name_fg);
	GtkTreeViewColumn* name_view_column = 
		gtk_tree_view_column_new_with_attributes(
		"Name"
		,cell_renderer_text
		,"text", 1
		,NULL);

	/* url column */
	cell_renderer_text = cell_renderer_new(opts->url_fg);
	GtkTreeViewColumn* url_view_column = 
		gtk_tree_view_column_new_with_attributes(
		"Url"
		,cell_renderer_text
		,"text", 2
		,NULL);

	/* comment column */
	cell_renderer_text = cell_renderer_new(opts->comment_fg);
	GtkTreeViewColumn* comment_view_column = 
		gtk_tree_view_column_new_with_attributes(
		"Comment"
		,cell_renderer_text
		,"text", 3
		,NULL);

	/* tag column */
	cell_renderer_text = cell_renderer_new(opts->tag_fg);
	GtkTreeViewColumn* tag_view_column = 
		gtk_tree_view_column_new_with_attributes(
		"Tag"
		,cell_renderer_text
		,"text", 4
		,NULL);

	/* set expand */
	gtk_tree_view_column_set_expand
		(GTK_TREE_VIEW_COLUMN(name_view_column), TRUE);

	gtk_tree_view_column_set_expand
		(GTK_TREE_VIEW_COLUMN(url_view_column), TRUE);

	gtk_tree_view_column_set_expand
		(GTK_TREE_VIEW_COLUMN(comment_view_column), TRUE);

	gtk_tree_view_column_set_expand
		(GTK_TREE_VIEW_COLUMN(tag_view_column), TRUE);

	/* tree store */
	bookmarks = gtk_tree_store_new(
		6
		,G_TYPE_STRING		//bookmark id
		,G_TYPE_STRING		//bookmark name
		,G_TYPE_STRING		//bookmark url
		,G_TYPE_STRING		//bookmark comment
		,G_TYPE_STRING		//tag
		,GDK_TYPE_PIXBUF	//directory icon
		,G_TYPE_STRING		//directory tag
		,-1);	

	/* tree view */
	GtkWidget* tree_view = gtk_tree_view_new_with_model
		(GTK_TREE_MODEL(bookmarks));

	/* unref bookmarks tree store*/
	g_object_unref(bookmarks);

	/* append columns */
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view)
		,dir_column);

	gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view)
		,name_view_column);

	gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view)
		,url_view_column);

	gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view)
		,comment_view_column);

	/*
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view)
		,tag_view_column);
	*/

	/* search entry */
	gtk_tree_view_set_search_entry(GTK_TREE_VIEW(tree_view)
		,GTK_ENTRY(search_entry));	

	/* enable searching in the tree */
	gtk_tree_view_set_enable_search(GTK_TREE_VIEW(tree_view), FALSE);

	/* clickable headers */
	gtk_tree_view_set_headers_clickable(GTK_TREE_VIEW(tree_view), FALSE);

	/* visible headers */
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tree_view), TRUE);

	/* lines */
	gtk_tree_view_set_enable_tree_lines(GTK_TREE_VIEW(tree_view), TRUE);

	model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree_view));

	g_signal_connect(tree_view, "cursor-changed"
		,G_CALLBACK(update_selected_row), tree_view);

	return tree_view;	
}

