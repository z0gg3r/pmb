#define _GNU_SOURCE 		/* getsubopt */

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bookmark.h"

/* used to get data from selected bookmark */
struct bookmark_data 
{
	long int id;
	char* name;
	char* url;
	char* comment;
	char* tag;
}; 
typedef struct bookmark_data bookmark_data;

struct gpmb_options 
{
	/* colors */
	char* id_fg;
	char* name_fg;
	char* url_fg;
	char* comment_fg;
	char* tag_fg;
};
typedef struct gpmb_options gpmb_options;

gpmb_options* 		opts = NULL;

sqlite3* 		db = NULL;

/* tree store */
GtkTreeStore* 		bookmarks;

/* tree view model */
GtkTreeModel* 		model = NULL;

/* selected path and column in tree view */
GtkTreePath* 		selected_path = NULL;
GtkTreeViewColumn* 	selected_column = NULL;

/* quit */
static void 
destroy(); 	

/* cancel */
static void 
close_window(GtkWidget*, gpointer);

/* when click 'add' button inside add bookmark window */
static void 
add_bookmark(GtkWidget*, gpointer**);

/* the add window */
static void 
add_window(GtkWidget*, gpointer);

/* when click 'delete' inside delete bookmark window */
static void 
delete_bookmark(GtkWidget*, gpointer**);

/* the delete window */
static void 
delete_window(GtkWidget*, gpointer);

/* when click 'edit' inside edit bookmark window */
static void 
edit_bookmark(GtkWidget*, gpointer**);

/* the edit window */
static void 
edit_window(GtkWidget*, gpointer);

/* the option window */
static void 
options_window(GtkWidget*, GtkWidget*);

/* add bookmark to the tree store */
static void 
tree_store_feed(GtkTreeIter
	,char* 	/* id */
	,char* 	/* name */
	,char* 	/* url */
	,char* 	/* comment */
	,char*); /* tag */ 

/* read database */
static void 
read_database(GtkWidget*, gpointer**); 

/* update selected_path and selected_column globals */
static void 
update_selected_row(gpointer**);

/* used by get_data() -- create a new bookmark_data struct */
static bookmark_data* 
create_data();

/* get data of current selected bookmark */
static bookmark_data* 
get_data(); 

/* create tree view */
static GtkWidget* 
tree_view(GtkWidget*);

/* options */
static void 
read_options();

/* gtk interface */
static int 
gtk_interface(int, char*[]);

int 
main(int argc, char *argv[]) 
{
	const char* 	home = secure_getenv("HOME");	
	char* 		file = calloc((strlen(home) + strlen(DATABASE)
				 + 2), sizeof(char));

	if(file) 
	{
		snprintf(file, strlen(file) - 1, "%s/%s", home
			, DATABASE);

		db = bookmark_db_open(file);
		free(file);
		file = NULL;
	}
	else /* going out if we can't allocate memory */
		exit(EXIT_FAILURE);

	read_options();
	short res = gtk_interface(argc, argv);

	free(opts);
	opts = NULL;

	bookmark_db_close(db);
	db = NULL;

	return res;
}

static void 
destroy() 
{
	gtk_main_quit();
}

static void  
close_window(GtkWidget* button, gpointer window) 
{
	gtk_widget_destroy(window);
}

static void 
read_options() 
{
	opts 	= malloc(sizeof(gpmb_options));

	if(opts) 
	{
		opts->id_fg 		= "#00ff00";
		opts->name_fg 		= "#ff0000";
		opts->url_fg 		= "#0000ff";
		opts->comment_fg 	= "#ff00ff";
		opts->tag_fg 		= "#f0a243";
	}
}

static void 
add_bookmark(GtkWidget* button, gpointer** args) 
{
	char* name 	= NULL;
	char* url 	= NULL;
	char* comment 	= "None";
	char* tag 	= "None";

	if(gtk_entry_get_text_length(GTK_ENTRY(args[0])))
		name 	= (char*)gtk_entry_get_text(GTK_ENTRY(args[0]));

	if(gtk_entry_get_text_length(GTK_ENTRY(args[1])))
		url 	= (char*)gtk_entry_get_text(GTK_ENTRY(args[1]));

	if(gtk_entry_get_text_length(GTK_ENTRY(args[2])))
		comment	= (char*)gtk_entry_get_text(GTK_ENTRY(args[2]));

	if(gtk_entry_get_text_length(GTK_ENTRY(args[3])))
		tag 	= (char*)gtk_entry_get_text(GTK_ENTRY(args[3]));

	if(name && url) 
	{
		bookmark* b = bookmark_create();
		bookmark_add(b, name, url, comment, tag);
		bookmark_db_write(b, db);
		bookmark_destroy(b);

		read_database(NULL, NULL);
		close_window(NULL, args[4]);
		g_free(args);
	}
}

static void 
add_window(GtkWidget* button, gpointer main_window) 
{
	/* set up window */
	GtkWidget* add_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(add_window), "Add bookmark");
	gtk_window_set_resizable(GTK_WINDOW(add_window), FALSE);
	gtk_window_set_transient_for(GTK_WINDOW(add_window)
		,GTK_WINDOW(main_window));

	g_signal_connect(add_window, "destroy", G_CALLBACK(close_window)
		,add_window);

	/* create entries and labels for each column of the database */
	GtkWidget* name_label = gtk_label_new("Name");
	gtk_widget_set_halign(GTK_WIDGET(name_label), GTK_ALIGN_START);

	GtkWidget* name_entry = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(name_entry), "name");
	//gtk_entry_set_width_chars(GTK_ENTRY(name_entry), 100);
	
	GtkWidget* url_label = gtk_label_new("Url");
	gtk_widget_set_halign(GTK_WIDGET(url_label), GTK_ALIGN_START);

	GtkWidget* url_entry = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(url_entry), "url");

	GtkWidget* comment_label = gtk_label_new("Comment");
	gtk_widget_set_halign(GTK_WIDGET(comment_label), GTK_ALIGN_START);

	GtkWidget* comment_entry = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(comment_entry), "comment");

	GtkWidget* tag_label = gtk_label_new("Tag");
	gtk_widget_set_halign(GTK_WIDGET(tag_label), GTK_ALIGN_START);

	GtkWidget* tag_entry = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(tag_entry), "tag");

	/* get data about selected bookmark */
	bookmark_data* data = get_data();

	if(data) 
	{
		if(data->tag)
			gtk_entry_set_text(GTK_ENTRY(tag_entry), data->tag);

		free(data);
		data = NULL;
	}

	GtkWidget** add_bookmark_args = g_new(GtkWidget*, 5);
	add_bookmark_args[0] = name_entry;
	add_bookmark_args[1] = url_entry;
	add_bookmark_args[2] = comment_entry;
	add_bookmark_args[3] = tag_entry;
	add_bookmark_args[4] = add_window;

	/* button */
	GtkWidget* add_button = gtk_button_new_with_mnemonic("_Add");
	g_signal_connect(add_button, "clicked", G_CALLBACK(add_bookmark)
		,add_bookmark_args);

	GtkWidget* cancel_button = gtk_button_new_with_mnemonic("_Cancel");
	g_signal_connect(cancel_button, "clicked", G_CALLBACK(close_window)
		,add_window);

	/* grid */
	GtkWidget *grid = gtk_grid_new();
	gtk_grid_set_column_spacing(GTK_GRID(grid), 2);
	gtk_grid_set_row_spacing(GTK_GRID(grid), 2);
	gtk_grid_set_column_homogeneous(GTK_GRID(grid), 1);

	gtk_grid_attach(GTK_GRID(grid), name_label 	,0,  0, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), name_entry 	,20, 0, 50, 1);

	gtk_grid_attach(GTK_GRID(grid), url_label 	,0,  1, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), url_entry 	,20, 1, 50, 1);

	gtk_grid_attach(GTK_GRID(grid), comment_label 	,0,  2, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), comment_entry 	,20, 2, 50, 1);

	gtk_grid_attach(GTK_GRID(grid), tag_label 	,0,  3, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), tag_entry 	,20, 3, 50, 1);

	gtk_grid_attach(GTK_GRID(grid), add_button 	,20, 4, 20, 10);
	gtk_grid_attach(GTK_GRID(grid), cancel_button 	,40, 4, 20, 10);

	/* add to window */
	gtk_container_add(GTK_CONTAINER(add_window), grid);

	/* show */
	gtk_widget_show_all(GTK_WIDGET(add_window));
}

static void
delete_bookmark(GtkWidget* button, gpointer** args) 
{
	char*  url 	= NULL;
	char** result;

	url = (char*)gtk_entry_get_text(GTK_ENTRY(args[0]));

	if(url) 
	{
		bookmark_list* bl = bookmark_db_search(db, URL, url);
		
		if(bl) 
		{
			result = bookmark_list_return_next(bl);

			if(result[0]) 
			{
				int id = strtol(result[0], NULL, 10);
				bookmark_db_delete(db, id);
				read_database(NULL, NULL);
				free(result);
				result = NULL;
			}

			bookmark_list_destroy(bl);
		}
	}

	close_window(NULL, args[1]);
	g_free(args);
}

static void
delete_window(GtkWidget* button, gpointer main_window) 
{
	/* set up window */
	GtkWidget* delete_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(delete_window), "Delete bookmark");
	gtk_window_set_resizable(GTK_WINDOW(delete_window), FALSE);
	gtk_window_set_transient_for(GTK_WINDOW(delete_window)
		,GTK_WINDOW(main_window));

	g_signal_connect(delete_window, "destroy", G_CALLBACK(close_window)
		,delete_window);

	/* label, entry */
	GtkWidget* delete_entry_label = gtk_label_new("Delete");
	gtk_widget_set_halign(GTK_WIDGET(delete_entry_label), GTK_ALIGN_START);
	GtkWidget* delete_entry = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(delete_entry), "delete");

	/* name */
	GtkWidget* name_label = gtk_label_new("Name");
	GtkWidget* name_entry = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(name_entry), "name");
	gtk_widget_set_halign(GTK_WIDGET(name_label), GTK_ALIGN_START);

	/* url */
	GtkWidget* url_label = gtk_label_new("Url");
	GtkWidget* url_entry = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(url_entry), "url");
	gtk_widget_set_halign(GTK_WIDGET(url_label), GTK_ALIGN_START);
	
	/* comment */
	GtkWidget* comment_label = gtk_label_new("Comment");
	GtkWidget* comment_entry = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(comment_entry), "comment");
	gtk_widget_set_halign(GTK_WIDGET(comment_label), GTK_ALIGN_START);

	/* tag */
	GtkWidget* tag_label = gtk_label_new("Delete");
	GtkWidget* tag_entry = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(tag_entry), "tag");
	gtk_widget_set_halign(GTK_WIDGET(tag_label), GTK_ALIGN_START);

	/* get data about selected bookmark */
	bookmark_data* data = get_data();

	if(data) 
	{
		if(data->name)
			gtk_entry_set_text(GTK_ENTRY(name_entry), data->name);

		if(data->url)
			gtk_entry_set_text(GTK_ENTRY(url_entry), data->url);

		if(data->comment)
			gtk_entry_set_text(GTK_ENTRY(comment_entry)
				,data->comment);

		if(data->tag)
			gtk_entry_set_text(GTK_ENTRY(tag_entry), data->tag);

		free(data);
		data = NULL;
	}

	GtkWidget** delete_bookmark_args = g_new(GtkWidget*, 2);
	delete_bookmark_args[0] = url_entry;
	delete_bookmark_args[1] = delete_window;

	/* button */
	GtkWidget* delete_button = gtk_button_new_with_mnemonic("_Delete");
	g_signal_connect(delete_button, "clicked", G_CALLBACK(delete_bookmark)
		,delete_bookmark_args);

	GtkWidget* cancel_button = gtk_button_new_with_mnemonic("_Cancel");
	g_signal_connect(cancel_button, "clicked", G_CALLBACK(close_window)
		,delete_window);

	/* grid */
	GtkWidget* grid = gtk_grid_new();
	gtk_grid_set_column_spacing(GTK_GRID(grid), 2);
	gtk_grid_set_row_spacing(GTK_GRID(grid), 2);
	gtk_grid_set_column_homogeneous(GTK_GRID(grid), 1);

	gtk_grid_attach(GTK_GRID(grid), name_label 	,0,  0, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), name_entry	,20, 0, 50, 1);

	gtk_grid_attach(GTK_GRID(grid), url_label 	,0,  1, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), url_entry	,20, 1, 50, 1);

	gtk_grid_attach(GTK_GRID(grid), comment_label 	,0,  2, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), comment_entry	,20, 2, 50, 1);

	gtk_grid_attach(GTK_GRID(grid), tag_label 	,0,  3, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), tag_entry	,20, 3, 50, 1);

	gtk_grid_attach(GTK_GRID(grid), delete_button	,20, 4, 20, 10);
	gtk_grid_attach(GTK_GRID(grid), cancel_button 	,40, 4, 20, 10);

	/* add to window */
	gtk_container_add(GTK_CONTAINER(delete_window), grid);

	/* show */
	gtk_widget_show_all(GTK_WIDGET(delete_window));
}

static void
edit_bookmark(GtkWidget* button, gpointer** args) 
{
	char*  name 	= NULL;
	char*  url 	= NULL;
	char*  comment 	= NULL;
	char*  tag 	= NULL;
	char** result;

	name 	= (char*)gtk_entry_get_text(GTK_ENTRY(args[0]));
	url 	= (char*)gtk_entry_get_text(GTK_ENTRY(args[1]));
	comment = (char*)gtk_entry_get_text(GTK_ENTRY(args[2]));
	tag 	= (char*)gtk_entry_get_text(GTK_ENTRY(args[3]));

	if(url) 
	{
		bookmark_list* bl = bookmark_db_search(db, URL, url);
		
		if(bl) 
		{
			result = bookmark_list_return_next(bl);

			if(result[0]) 
			{
				int id = strtol(result[0], NULL, 10);

				if(name)
					bookmark_db_edit(db, id, 0, name);

				if(url)
					bookmark_db_edit(db, id, 1, url);

				if(comment)
					bookmark_db_edit(db, id, 2, comment);

				if(tag)
					bookmark_db_edit(db, id, 3, tag);

				read_database(NULL, NULL);
				free(result);
				result = NULL;
			}

			bookmark_list_destroy(bl);
		}
	}

	close_window(NULL, args[4]);
	g_free(args);
}

static void
edit_window(GtkWidget* button, gpointer main_window) 
{
	/* set up window */
	GtkWidget* edit_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(edit_window), "Edit bookmark");
	gtk_window_set_resizable(GTK_WINDOW(edit_window), FALSE);
	gtk_window_set_transient_for(GTK_WINDOW(edit_window)
		,GTK_WINDOW(main_window));

	g_signal_connect(edit_window, "destroy", G_CALLBACK(close_window)
		,edit_window);

	GtkWidget* name_entry_label = gtk_label_new("Name");
	gtk_widget_set_halign(GTK_WIDGET(name_entry_label), GTK_ALIGN_START);

	GtkWidget* name_entry = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(name_entry), "name");
	//gtk_entry_set_width_chars(GTK_ENTRY(name_entry), 100);
	
	GtkWidget* url_entry_label = gtk_label_new("Url");
	gtk_widget_set_halign(GTK_WIDGET(url_entry_label), GTK_ALIGN_START);

	GtkWidget* url_entry = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(url_entry), "url");

	GtkWidget* comment_entry_label = gtk_label_new("Comment");
	gtk_widget_set_halign(GTK_WIDGET(comment_entry_label)
		,GTK_ALIGN_START);

	GtkWidget* comment_entry = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(comment_entry), "comment");

	GtkWidget* tag_entry_label = gtk_label_new("Tag");
	gtk_widget_set_halign(GTK_WIDGET(tag_entry_label), GTK_ALIGN_START);

	GtkWidget* tag_entry = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(tag_entry), "tag");

	/* get data about selected bookmark */
	bookmark_data* data = get_data();

	if(data) 
	{
		if(data->name)
			gtk_entry_set_text(GTK_ENTRY(name_entry), data->name);

		if(data->url)
			gtk_entry_set_text(GTK_ENTRY(url_entry), data->url);

		if(data->comment)
			gtk_entry_set_text(GTK_ENTRY(comment_entry)
				,data->comment);

		if(data->tag)
			gtk_entry_set_text(GTK_ENTRY(tag_entry), data->tag);

		free(data);
		data = NULL;
	}

	GtkWidget** edit_bookmark_args = g_new(GtkWidget*, 5);
	edit_bookmark_args[0] = name_entry;
	edit_bookmark_args[1] = url_entry;
	edit_bookmark_args[2] = comment_entry;
	edit_bookmark_args[3] = tag_entry;
	edit_bookmark_args[4] = edit_window;

	/* button */
	GtkWidget* edit_button = gtk_button_new_with_mnemonic("_edit");
	g_signal_connect(edit_button, "clicked", G_CALLBACK(edit_bookmark)
		,edit_bookmark_args);

	GtkWidget* cancel_button = gtk_button_new_with_mnemonic("_Cancel");
	g_signal_connect(cancel_button, "clicked", G_CALLBACK(close_window)
		,edit_window);

	/* grid */
	GtkWidget* grid = gtk_grid_new();
	gtk_grid_set_column_spacing(GTK_GRID(grid), 2);
	gtk_grid_set_row_spacing(GTK_GRID(grid), 2);
	gtk_grid_set_column_homogeneous(GTK_GRID(grid), 1);

	gtk_grid_attach(GTK_GRID(grid), name_entry_label 	,0,  0, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), name_entry 		,20, 0, 50, 1);

	gtk_grid_attach(GTK_GRID(grid), url_entry_label 	,0,  1, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), url_entry 		,20, 1, 50, 1);

	gtk_grid_attach(GTK_GRID(grid), comment_entry_label 	,0,  2, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), comment_entry 		,20, 2, 50, 1);

	gtk_grid_attach(GTK_GRID(grid), tag_entry_label 	,0,  3, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), tag_entry 		,20, 3, 50, 1);

	gtk_grid_attach(GTK_GRID(grid), edit_button 		,20, 4, 20, 10);
	gtk_grid_attach(GTK_GRID(grid), cancel_button 		,40, 4, 20, 10);

	/* add to window */
	gtk_container_add(GTK_CONTAINER(edit_window), grid);

	/* show */
	gtk_widget_show_all(GTK_WIDGET(edit_window));
}

static void
options_window(GtkWidget* button, GtkWidget* main_window) 
{
	/* set up window */
	GtkWidget* options_dialog = gtk_dialog_new();
	GtkWidget* dialog_box = gtk_dialog_get_content_area
		(GTK_DIALOG(options_dialog));
	gtk_window_set_transient_for(GTK_WINDOW(options_dialog)
		,GTK_WINDOW(main_window));
	//gtk_window_set_type_hint(GTK_WINDOW(options_dialog), GDK_WINDOW_TYPE_HINT_DIALOG);

	g_signal_connect(options_dialog, "destroy", G_CALLBACK(close_window)
		,options_dialog);

	/* page 1 */
	GtkWidget* page_1_label = gtk_label_new("page 1");
	GtkWidget* test_1_button = gtk_button_new_with_label("test 1");

	GtkWidget* page_1_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
	gtk_container_add(GTK_CONTAINER(page_1_box), test_1_button);

	/* page 2 */
	GtkWidget* page_2_label = gtk_label_new("page 2");
	GtkWidget* test_2_button = gtk_button_new_with_label("test 2");

	GtkWidget* page_2_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
	gtk_container_add(GTK_CONTAINER(page_2_box), test_2_button);

	/* notebook */
	GtkWidget* notebook = gtk_notebook_new();
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page_1_box, page_1_label);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page_2_box, page_2_label);

	/* show */
	gtk_container_add(GTK_CONTAINER(dialog_box), notebook);
	gtk_widget_show_all(GTK_WIDGET(options_dialog));
}

static void
update_selected_row(gpointer** args) 
{
	GtkTreePath* path 		= gtk_tree_path_new();
	GtkTreeViewColumn* column 	= gtk_tree_view_column_new();

	gtk_tree_view_get_cursor(GTK_TREE_VIEW(&args[0]), &path, &column);
	selected_path 		= path;
	selected_column 	= column;
}

static bookmark_data*
create_data() 
{
	bookmark_data* data = malloc(sizeof(bookmark_data));

	if(data) 
	{
		data->id 	= 0;
		data->name 	= NULL;
		data->url 	= NULL;
		data->comment 	= NULL;
		data->tag 	= NULL;

		return data;
	}
	else
		return NULL;
}

static bookmark_data*
get_data() 
{
	GtkTreeIter iter;

	if(selected_path) 
	{
		gtk_tree_model_get_iter(GTK_TREE_MODEL(model), &iter
			,selected_path);

		bookmark_data* data = create_data();

		if(data) 
		{
			gtk_tree_model_get(GTK_TREE_MODEL(model), &iter
				,0, &data->id
				,1, &data->name
				,2, &data->url
				,3, &data->comment
				,4, &data->tag
				,-1);

			return data;
		}
		else
			return NULL;
	}
	else
		return NULL;
}

static GtkCellRenderer*
cell_renderer_create(char* color) 
{
	GdkRGBA	gcolor;
	GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
	gdk_rgba_parse(&gcolor, color);
	g_object_set(G_OBJECT(renderer)
		,"foreground-rgba", &gcolor
		,NULL);

	return renderer;
}

static GtkWidget*
tree_view(GtkWidget* search_entry) 
{
	GtkCellRenderer* 	cell_renderer_text;

	cell_renderer_text = cell_renderer_create(opts->id_fg);
	GtkTreeViewColumn* bookmark_id_column = 
		gtk_tree_view_column_new_with_attributes(
		"Id"
		,cell_renderer_text
		,"text", 0
		,NULL);

	/* name */
	cell_renderer_text = cell_renderer_create(opts->name_fg);
	GtkTreeViewColumn* name_view_column = 
		gtk_tree_view_column_new_with_attributes(
		"Name"
		,cell_renderer_text
		,"text", 1
		,NULL);

	/* url */
	cell_renderer_text = cell_renderer_create(opts->url_fg);
	GtkTreeViewColumn* url_view_column = 
		gtk_tree_view_column_new_with_attributes(
		"Url"
		,cell_renderer_text
		,"text", 2
		,NULL);

	/* comment */
	cell_renderer_text = cell_renderer_create(opts->comment_fg);
	GtkTreeViewColumn* comment_view_column = 
		gtk_tree_view_column_new_with_attributes(
		"Comment"
		,cell_renderer_text
		,"text", 3
		,NULL);

	/* tag */
	cell_renderer_text = cell_renderer_create(opts->tag_fg);
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

	GtkWidget* tree_view = gtk_tree_view_new_with_model
		(GTK_TREE_MODEL(bookmarks));

	gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view)
		,bookmark_id_column);

	gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view)
		,name_view_column);

	gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view)
		,url_view_column);

	gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view)
		,comment_view_column);

	gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view)
		,tag_view_column);

	/* search entry */
	gtk_tree_view_set_search_entry(GTK_TREE_VIEW(tree_view)
		,GTK_ENTRY(search_entry));	

	/* enable searching in the tree */
	gtk_tree_view_set_enable_search(GTK_TREE_VIEW(tree_view), TRUE);

	/* clickable headers */
	gtk_tree_view_set_headers_clickable(GTK_TREE_VIEW(tree_view), TRUE);

	model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree_view));

	GtkWidget** update_selected_row_args = g_new(GtkWidget*, 1);
	update_selected_row_args[0] = tree_view;

	g_signal_connect(tree_view, "cursor-changed"
		,G_CALLBACK(update_selected_row), update_selected_row_args);

	return tree_view;	
}

static void 
tree_store_feed(GtkTreeIter i
	,char* id
	,char* name
	,char* url
	,char* comment
	,char* tag) 
{
	if(bookmarks) 
	{
		gtk_tree_store_append(bookmarks, &i, NULL);
		gtk_tree_store_set(bookmarks, &i, 0, id, -1);
		gtk_tree_store_set(bookmarks, &i, 1, name, -1);
		gtk_tree_store_set(bookmarks, &i, 2, url, -1);
		gtk_tree_store_set(bookmarks, &i, 3, comment, -1);
		gtk_tree_store_set(bookmarks, &i, 4, tag, -1);
	}
}

static void 
read_database(GtkWidget* button, gpointer** args) 
{
	bookmark_list* 	bl = NULL;
	GtkTreeIter 	tree_iter;
	char** 		result;

	gtk_tree_store_clear(bookmarks);

	if(&args[0]) 
	{
		GtkWidget* entry = GTK_WIDGET(args[0]);

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
		bl = bookmark_db_query(db, 0 ,NULL);
	}

	if(bl) 
	{
		for(int i = 0; i < bookmark_list_get_size(bl) - 1; ++i) 
		{
			result = bookmark_list_return_next(bl);

			/*
			char* str 	= calloc(1, strlen(result[4]));
			char* r 	= calloc(1, strlen(result[4]));
			strncpy(r, result[4], strlen(result[4]));

			while((str = strsep(&r, "/")))
				printf("%s\n", str);

			free(r);
			free(str);
			*/

			if(result)
				tree_store_feed(tree_iter
					,result[0]
					,result[1]
					,result[2]
					,result[3]
					,result[4]);
		}

		if(result) 
		{
			free(result);
			result = NULL;
		}

		bookmark_list_destroy(bl);
	}
}

static GtkWidget*
make_menu_bar(GtkWidget* main_window) 
{
	/*
	GMenu* file_menu = g_menu_new();
	g_menu_insert(file_menu, 0, "File", NULL);
	g_menu_insert(file_menu, 1, "Exit", NULL);

	GMenu* edit_menu = g_menu_new();
	g_menu_insert(edit_menu, 0, "Insert", NULL);
	g_menu_insert(edit_menu, 1, "Edit", NULL);
	g_menu_insert(edit_menu, 2, "Delete", NULL);

	GMenu* help_menu = g_menu_new();
	g_menu_insert(help_menu, 0, "About", NULL);

	GMenu* menu = g_menu_new();
	g_menu_append_submenu(menu, "File", file_menu);
	g_menu_append_submenu(menu, "Edit", edit_menu);
	g_menu_append_submenu(menu, "Help", help_menu);

	*/

	/* file menu */
	GtkWidget* file_menu 	= gtk_menu_new();
	GtkWidget* file_top	= gtk_menu_item_new_with_mnemonic("_File");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_top), file_menu);

	GtkWidget* open 	= gtk_menu_item_new_with_mnemonic("_Open");
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), open);

	GtkWidget* exit 	= gtk_menu_item_new_with_mnemonic("E_xit");
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), exit);
	g_signal_connect(GTK_WIDGET(exit), "activate"
		,G_CALLBACK(destroy), NULL);

	/* edit menu */
	GtkWidget* edit_menu 	= gtk_menu_new();
	GtkWidget* edit_top 	= gtk_menu_item_new_with_mnemonic("_Edit");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit_top), edit_menu);

	GtkWidget* reload	= gtk_menu_item_new_with_mnemonic("_Reload");
	gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), reload);
	g_signal_connect(GTK_WIDGET(reload), "activate"
		,G_CALLBACK(read_database), NULL);

	GtkWidget* insert 	= gtk_menu_item_new_with_mnemonic("_Insert");
	gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), insert);
	g_signal_connect(GTK_WIDGET(insert), "activate"
		,G_CALLBACK(add_window), main_window);

	GtkWidget* edit 	= gtk_menu_item_new_with_mnemonic("_Edit");
	gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), edit);
	g_signal_connect(GTK_WIDGET(edit), "activate"
		,G_CALLBACK(edit_window), main_window);

	GtkWidget* delete	= gtk_menu_item_new_with_mnemonic("_Delete");
	gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), delete);
	g_signal_connect(GTK_WIDGET(delete), "activate"
		,G_CALLBACK(delete_window), main_window);

	GtkWidget* preferences	= gtk_menu_item_new_with_mnemonic
		("_Preferences");
	gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), preferences);
	g_signal_connect(GTK_WIDGET(preferences), "activate"
		,G_CALLBACK(options_window), main_window);

	/* help menu */
	GtkWidget* help_menu	= gtk_menu_new();
	GtkWidget* help_top	= gtk_menu_item_new_with_mnemonic("_Help");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(help_top), help_menu);

	GtkWidget* about	= gtk_menu_item_new_with_mnemonic("_About");
	gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), about);

	/* menu bar */
	GtkWidget *menu_bar 	= gtk_menu_bar_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_top);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), edit_top);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help_top);

	return menu_bar;
}

static GtkWidget*
make_tool_box(GtkWidget* main_window) 
{
	short icon_size 	= 16;

	/* add */
	GtkWidget* add_icon 	= gtk_image_new_from_icon_name
		("bookmark_add", icon_size);
	GtkToolItem* add 	= gtk_tool_button_new(add_icon, "_Add");
	gtk_tool_button_set_use_underline(GTK_TOOL_BUTTON(add), TRUE);
	gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(add), "add");

	g_signal_connect(add, "clicked", G_CALLBACK(add_window)
		,main_window);

	/* edit */
	GtkWidget* edit_icon 	= gtk_image_new_from_icon_name
		("document-page-setup", icon_size);
	GtkToolItem* edit 	= gtk_tool_button_new(edit_icon, "_Edit");
	gtk_tool_button_set_use_underline(GTK_TOOL_BUTTON(edit), TRUE);
	gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(edit), "edit");

	g_signal_connect(edit, "clicked", G_CALLBACK(edit_window)
		,main_window);

	/* delete */
	GtkWidget* delete_icon	= gtk_image_new_from_icon_name
		("edit-delete", icon_size);
	GtkToolItem* delete 	= gtk_tool_button_new(delete_icon, "_Delete");
	gtk_tool_button_set_use_underline(GTK_TOOL_BUTTON(delete), TRUE);
	gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(delete), "delete");

	g_signal_connect(delete, "clicked", G_CALLBACK(delete_window)
		,main_window);

	/* options */
	GtkWidget* options_icon = gtk_image_new_from_icon_name
		("system-run", icon_size);
	GtkToolItem* options 	= gtk_tool_button_new(options_icon
					,"Option");
	//gtk_tool_button_set_use_underline(GTK_TOOL_BUTTON(options), TRUE);
	gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(options), "options");

	g_signal_connect(options, "clicked", G_CALLBACK(options_window)
		,main_window);

	/* reload */
	GtkWidget* reload_icon 	= gtk_image_new_from_icon_name
		("view-refresh", icon_size);
	GtkToolItem* reload 	= gtk_tool_button_new(reload_icon, "_Reload");
	gtk_tool_button_set_use_underline(GTK_TOOL_BUTTON(reload), TRUE);
	gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(reload)
		,"reload");

	g_signal_connect(reload, "clicked", G_CALLBACK(read_database)
		,NULL);

	/* tool bar */
	GtkWidget* tool_bar 	= gtk_toolbar_new();
	gtk_toolbar_set_style(GTK_TOOLBAR(tool_bar), GTK_TOOLBAR_BOTH);
	gtk_toolbar_insert(GTK_TOOLBAR(tool_bar), add, 0);
	gtk_toolbar_insert(GTK_TOOLBAR(tool_bar), edit, 1);
	gtk_toolbar_insert(GTK_TOOLBAR(tool_bar), delete, 2);
	gtk_toolbar_insert(GTK_TOOLBAR(tool_bar), reload, 3);
	gtk_toolbar_insert(GTK_TOOLBAR(tool_bar), options, 4);

	return tool_bar;
}

static GtkWidget*
make_search_box(GtkWidget* search_entry)
{
	/* search entry */
	GtkWidget* search_entry_label = gtk_label_new("Search:");
	gtk_widget_set_hexpand(GTK_WIDGET(search_entry), 1);

	/* search button */
	GtkWidget* search_button = gtk_button_new_with_mnemonic("_Go");

	GtkWidget** read_database_args = g_new(GtkWidget*, 1);
	read_database_args[0] = search_entry;

	g_signal_connect(GTK_WIDGET(search_button), "clicked"
		,G_CALLBACK(read_database), read_database_args);

	g_signal_connect(GTK_WIDGET(search_entry), "activate"
		,G_CALLBACK(read_database), read_database_args);

	/* search box */
	GtkWidget* search_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
	gtk_container_add(GTK_CONTAINER(search_box), search_entry_label);
	gtk_container_add(GTK_CONTAINER(search_box), search_entry);
	gtk_container_add(GTK_CONTAINER(search_box), search_button);

	return search_box;
}

static void
key_press(GtkWidget* window, GdkEventKey* e, gpointer** args)
{
	char* key = gdk_keyval_name(e->keyval);
	//printf("%s\n", key);

	if(!strcmp(key, "Down") 
	||(!strcmp(key, "Up"))
	||(!strcmp(key, "Right"))	
	||(!strcmp(key, "Left"))
	||(!strcmp(key, "Page_Up"))
	||(!strcmp(key, "Page_Down"))
	||(!strcmp(key, "Home"))
	||(!strcmp(key, "End")))
		gtk_tree_view_set_cursor(GTK_TREE_VIEW(args[0])
			,selected_path, NULL, 0);

	else if(!strcmp(key, "j"))
	{
		GtkTreeIter iter;
		gtk_tree_model_get_iter(GTK_TREE_MODEL(model), &iter, selected_path);

		if(gtk_tree_model_iter_next(GTK_TREE_MODEL(model), &iter))
		{
			gtk_tree_path_next(selected_path);
			gtk_tree_view_set_cursor(GTK_TREE_VIEW(args[0])
				,selected_path, NULL, 0);
		}
	}

	else if(!strcmp(key, "k"))
	{
		if(gtk_tree_path_prev(selected_path))
			gtk_tree_view_set_cursor(GTK_TREE_VIEW(args[0])
				,selected_path, NULL, 0);
	}

	else if(!strcmp(key, "h")) 
	{
		GtkAdjustment* adj	= gtk_scrolled_window_get_hadjustment
						(GTK_SCROLLED_WINDOW(args[1]));
		gdouble value 		= gtk_adjustment_get_value
						(GTK_ADJUSTMENT(adj));
		gdouble step		= gtk_adjustment_get_step_increment
						(GTK_ADJUSTMENT(adj));

		gtk_adjustment_set_value(GTK_ADJUSTMENT(adj), (value - step));
	}

	else if(!strcmp(key, "l")) 
	{
		GtkAdjustment* adj	= gtk_scrolled_window_get_hadjustment
						(GTK_SCROLLED_WINDOW(args[1]));
		gdouble value 		= gtk_adjustment_get_value
						(GTK_ADJUSTMENT(adj));
		gdouble step		= gtk_adjustment_get_step_increment
						(GTK_ADJUSTMENT(adj));

		gtk_adjustment_set_value(GTK_ADJUSTMENT(adj), (value + step));
	}

	else if(!strcmp(key, "g"))
	{
		GtkTreeIter iter;
		gtk_tree_model_get_iter_first(GTK_TREE_MODEL(model), &iter);
		GtkTreePath* path = gtk_tree_model_get_path
			(GTK_TREE_MODEL(model), &iter);

		if(path) 
		{
			gtk_tree_view_set_cursor(GTK_TREE_VIEW(args[0])
				,path, NULL, 0);

			gtk_tree_path_free(path);
			path = NULL;
		}
	}

	else if(!strcmp(key, "G"))
	{
		GtkAdjustment* adj	= gtk_scrolled_window_get_vadjustment
						(GTK_SCROLLED_WINDOW(args[1]));
		gdouble lower		= gtk_adjustment_get_lower
						(GTK_ADJUSTMENT(adj));

		gtk_adjustment_set_value(GTK_ADJUSTMENT(adj), lower);
		gtk_tree_view_set_cursor(GTK_TREE_VIEW(args[0])
			,selected_path, NULL, 0);
	}

	else if(!strcmp(key, "dollar"))
	{
		GtkAdjustment* adj	= gtk_scrolled_window_get_hadjustment
						(GTK_SCROLLED_WINDOW(args[1]));
		gdouble upper		= gtk_adjustment_get_upper
						(GTK_ADJUSTMENT(adj));

		gtk_adjustment_set_value(GTK_ADJUSTMENT(adj), upper);
		gtk_tree_view_set_cursor(GTK_TREE_VIEW(args[0])
			,selected_path, NULL, 0);	
	}

	else if((!strcmp(key, "0"))
	||(!strcmp(key, "bar")))
	{
		GtkAdjustment* adj	= gtk_scrolled_window_get_hadjustment
						(GTK_SCROLLED_WINDOW(args[1]));
		gdouble lower		= gtk_adjustment_get_lower
						(GTK_ADJUSTMENT(adj));

		gtk_adjustment_set_value(GTK_ADJUSTMENT(adj), lower);
		gtk_tree_view_set_cursor(GTK_TREE_VIEW(args[0])
			,selected_path, NULL, 0);
	}

	else if(!strcmp(key, "b"))
	{
		GtkAdjustment* adj	= gtk_scrolled_window_get_vadjustment
						(GTK_SCROLLED_WINDOW(args[1]));
		gdouble value 		= gtk_adjustment_get_value
						(GTK_ADJUSTMENT(adj));
		gdouble page		= gtk_adjustment_get_page_increment
						(GTK_ADJUSTMENT(adj));

		GtkAllocation allocation;
		GtkTreePath* path = gtk_tree_path_new();

		gtk_widget_get_allocation(GTK_WIDGET(args[0]), &allocation);
		gtk_adjustment_set_value(GTK_ADJUSTMENT(adj), (value - page));

		gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(args[0]), 0
			,(allocation.height - page), &path, NULL, NULL, NULL);

		if(path)
			gtk_tree_view_set_cursor(GTK_TREE_VIEW(args[0])
				,path, NULL, 0);

		gtk_tree_path_free(path);
		path = NULL;
	}

	else if(!strcmp(key, "f"))
	{
		GtkAdjustment* adj	= gtk_scrolled_window_get_vadjustment
						(GTK_SCROLLED_WINDOW(args[1]));
		gdouble value 		= gtk_adjustment_get_value
						(GTK_ADJUSTMENT(adj));
		gdouble page		= gtk_adjustment_get_page_increment
						(GTK_ADJUSTMENT(adj));

		GtkAllocation allocation;
		GtkTreePath* path = gtk_tree_path_new();

		gtk_widget_get_allocation(GTK_WIDGET(args[0]), &allocation);
		gtk_adjustment_set_value(GTK_ADJUSTMENT(adj), (value + page));

		gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(args[0]), 0
			,(allocation.height + page), &path, NULL, NULL, NULL);

		if(path)
			gtk_tree_view_set_cursor(GTK_TREE_VIEW(args[0])
				,path, NULL, 0);

		gtk_tree_path_free(path);
		path = NULL;
	}

	else if(!strcmp(key, "s"))
		gtk_widget_grab_focus(GTK_WIDGET(args[2]));

	else if(!strcmp(key, "r"))
		read_database(NULL, NULL);

	else if(!strcmp(key, "d"))
		delete_window(NULL, window);

	else if(!strcmp(key, "i"))
		add_window(NULL, window);

	else if(!strcmp(key, "e"))
		edit_window(NULL, window);
	
	else if(!strcmp(key, "O"))
		options_window(NULL, window);
}

static void
search_entry_key_press(GtkWidget* window, GdkEventKey* e, GtkWidget* s_window)
{
	if(!strcmp((gdk_keyval_name(e->keyval)), "Escape")) 
		gtk_widget_grab_focus(GTK_WIDGET(s_window));
}

static int 
gtk_interface(int argc, char* argv[]) 
{
	gtk_init(&argc, &argv);

	/* set up the main window */
	GtkWidget* bookmark_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(bookmark_window), "Bookmarks");
	gtk_window_set_role(GTK_WINDOW(bookmark_window), "Bookmarks");
	gtk_window_set_default_size(GTK_WINDOW(bookmark_window)
		,800, 600);

	g_signal_connect(bookmark_window, "destroy", G_CALLBACK(destroy)
		,NULL);

	/*
	gtk_window_set_wmclass(GTK_WINDOW(bookmark_window)
		,"Bookmarks", "gtkTraining");
	*/
	
	/* tree store */
	bookmarks = gtk_tree_store_new(
		5
		,G_TYPE_STRING		//id
		,G_TYPE_STRING		//name
		,G_TYPE_STRING		//url
		,G_TYPE_STRING		//comment
		,G_TYPE_STRING);	//tag

	/* tree view */
	GtkWidget* search_entry = gtk_entry_new();
	GtkWidget* t_view = tree_view(search_entry);
	gtk_tree_view_set_enable_search(GTK_TREE_VIEW(t_view), FALSE);

	/* scrolled window for bookmark_view */
	GtkWidget* s_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add(GTK_CONTAINER(s_window), t_view);

	GtkWidget** key_press_args = g_new(GtkWidget*, 3);
	key_press_args[0] = t_view;
	key_press_args[1] = s_window;
	key_press_args[2] = search_entry;

	g_signal_connect(s_window, "key-press-event"
		,G_CALLBACK(key_press), key_press_args);

	g_signal_connect(search_entry, "key-release-event"
		,G_CALLBACK(search_entry_key_press), t_view);

	/* menu bar */
	GtkWidget* menu_bar = make_menu_bar(bookmark_window);

	/* tool box */
	GtkWidget* tool_box = make_tool_box(bookmark_window);

	/* search box */
	GtkWidget* search_box = make_search_box(search_entry);

	/* add to v_box */
	GtkWidget* v_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
	gtk_container_add(GTK_CONTAINER(v_box), menu_bar);
	gtk_container_add(GTK_CONTAINER(v_box), tool_box);
	gtk_container_add(GTK_CONTAINER(v_box), search_box);
	gtk_box_pack_start(GTK_BOX(v_box), s_window, TRUE, TRUE, 1);

	/* read database and append to tree */
	read_database(NULL, NULL);

	/* add v_box into window */
	gtk_container_add(GTK_CONTAINER(bookmark_window), v_box);
	gtk_widget_show_all(bookmark_window);

	/* focus tree view */
	gtk_widget_grab_focus(GTK_WIDGET(t_view));

	GtkTreeIter iter;
	gtk_tree_model_get_iter_first(GTK_TREE_MODEL(model), &iter);
	GtkTreePath* path = gtk_tree_model_get_path(GTK_TREE_MODEL(model)
		,&iter);
	gtk_tree_view_set_cursor(GTK_TREE_VIEW(t_view), path, NULL, 0);

	/* free memory */
	g_object_unref(bookmarks);

	gtk_main();
	return 0;
}

