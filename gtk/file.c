#include "file.h"

void
open_database(GtkWidget* button, gpointer window)
{
	gtk_spinner_start(GTK_SPINNER(spinner));

	GtkWidget* file_chooser = gtk_file_chooser_dialog_new("Open database"
			,window
			,GTK_FILE_CHOOSER_ACTION_SAVE
			,"_Open"
			,GTK_RESPONSE_ACCEPT
			,"_Cancel"
			,GTK_RESPONSE_REJECT
			,NULL);

	gint result = gtk_dialog_run(GTK_DIALOG(file_chooser));

	switch(result)
	{
		case GTK_RESPONSE_ACCEPT:
		{
			char* filename 	= gtk_file_chooser_get_filename
						(GTK_FILE_CHOOSER(file_chooser));

			db 		= bookmark_db_open(filename);

			free(database_file);
			database_file 	= strdup(filename);

			g_free(filename);
			read_database(NULL, NULL);
			break;
		}
		case GTK_RESPONSE_REJECT:
			break;
		default:
			break;
	}
	
	gtk_widget_destroy(file_chooser);
	gtk_spinner_stop(GTK_SPINNER(spinner));
}

void
import_database(GtkWidget* button, gpointer window)
{
	gtk_spinner_start(GTK_SPINNER(spinner));

	GtkWidget* file_chooser = gtk_file_chooser_dialog_new("Import database"
			,window
			,GTK_FILE_CHOOSER_ACTION_OPEN
			,"_Import"
			,GTK_RESPONSE_ACCEPT
			,"_Cancel"
			,GTK_RESPONSE_REJECT
			,NULL);

	gint result = gtk_dialog_run(GTK_DIALOG(file_chooser));

	switch(result)
	{
		case GTK_RESPONSE_ACCEPT:
		{
			char* 	filename = gtk_file_chooser_get_filename
						(GTK_FILE_CHOOSER(file_chooser));

			if(filename)
			{
				sqlite3* i_db = bookmark_db_open(filename);

				if(i_db)
				{
					bookmark_db_import(db, i_db);
					bookmark_db_close(i_db);	
				}
			}

			g_free(filename);
			break;
		}
		case GTK_RESPONSE_REJECT:
			break;
		default:
			break;
	}
	
	gtk_widget_destroy(file_chooser);
	gtk_spinner_stop(GTK_SPINNER(spinner));
	read_database(NULL, NULL);
}

void
export_html_page(GtkWidget* button, gpointer window)
{
	gtk_spinner_start(GTK_SPINNER(spinner));

	GtkWidget* file_chooser = gtk_file_chooser_dialog_new("Export html page"
			,window
			,GTK_FILE_CHOOSER_ACTION_SAVE
			,"_Save"
			,GTK_RESPONSE_ACCEPT
			,"_Cancel"
			,GTK_RESPONSE_REJECT
			,NULL);

	gint result = gtk_dialog_run(GTK_DIALOG(file_chooser));

	switch(result)
	{
		case GTK_RESPONSE_ACCEPT:
		{
			char* 		filename 	= gtk_file_chooser_get_filename
								(GTK_FILE_CHOOSER
									(file_chooser));

			FILE* 		fp 		= fopen(filename, "w");
			bookmark_list* 	bl 		= bookmark_db_query(db, 0, NULL);

			bookmark_html_tree(bl, fp);
			fclose(fp);
			bookmark_list_destroy(bl);
			g_free(filename);
			break;
		}
		case GTK_RESPONSE_REJECT:
			break;
		default:
			break;
	}
	
	gtk_widget_destroy(file_chooser);
	gtk_spinner_stop(GTK_SPINNER(spinner));
}

/* for selective import/export functions */
GtkWidget*	db_entry;
GtkWidget*	pattern_entry;
GtkWidget*	field_box;

static void
selective_get_database(GtkWidget* button, gpointer window)
{
	GtkWidget* file_chooser = gtk_file_chooser_dialog_new("Select database"
			,window
			,GTK_FILE_CHOOSER_ACTION_SAVE
			,"_Open"
			,GTK_RESPONSE_ACCEPT
			,"_Cancel"
			,GTK_RESPONSE_REJECT
			,NULL);

	gint result = gtk_dialog_run(GTK_DIALOG(file_chooser));

	switch(result)
	{
		case GTK_RESPONSE_ACCEPT:
		{
			char* filename 	= gtk_file_chooser_get_filename
						(GTK_FILE_CHOOSER(file_chooser));
			
			gtk_entry_set_text(GTK_ENTRY(db_entry), filename);
			g_free(filename);
			break;
		}
		case GTK_RESPONSE_REJECT:
			break;
		default:
			break;
	}
	
	gtk_widget_destroy(file_chooser);
}

static void
selective_copy(GtkWidget* button, unsigned int* action)
{
	char* filename	= (char*)gtk_entry_get_text(GTK_ENTRY(db_entry));
	char* pattern 	= (char*)gtk_entry_get_text(GTK_ENTRY(pattern_entry));
	char* field	= gtk_combo_box_text_get_active_text
				(GTK_COMBO_BOX_TEXT(field_box));

	if(filename)
	{
		sqlite3* i_db = bookmark_db_open(filename);

		if(i_db && pattern)
		{
			sqlite3* 	db_1 	= db;
			sqlite3* 	db_2 	= i_db;
			bookmark_list* 	bl 	= NULL;

			if(*action)
			{
				db_1 = i_db;
				db_2 = db;
			}

			if(!(strcmp(field, "any")))
				bl = bookmark_db_search(db_2, 0, pattern);

			else if(!(strcmp(field, "id")))
				bl = bookmark_db_query(db_2
					,(strtol(pattern, NULL, 10)), NULL);

			else if(!(strcmp(field, "name")))
				bl = bookmark_db_search(db_2, NAME, pattern);

			else if(!(strcmp(field, "url")))
				bl = bookmark_db_search(db_2, URL, pattern);

			else if(!(strcmp(field, "comment")))
				bl = bookmark_db_search(db_2, COMMENT, pattern);

			else if(!(strcmp(field, "tag")))
				bl = bookmark_db_search(db_2, TAG, pattern);

			if(bl)
			{
				bookmark* b = NULL;

				while((b = bookmark_list_return_next_bookmark(bl)))
				{
					bookmark_db_write(b, db_1);
					bookmark_destroy(b);
				}

				bookmark_list_destroy(bl);
				read_database(NULL, NULL);
			}

			bookmark_db_close(i_db);
		}
	}
}

static void
selective_dialog_content(GtkWidget* window, unsigned int* action)
{
	/* selected database */
	GtkWidget*	db_entry_label	= gtk_label_new("Database");
	db_entry			= gtk_entry_new();

	/* select database button */
	GtkWidget* 	select_button 	= gtk_button_new_with_mnemonic("_Select database");
	g_signal_connect(GTK_WIDGET(select_button), "clicked"
		,G_CALLBACK(selective_get_database), window);

	/* pattern */
	GtkWidget*	pattern_label	= gtk_label_new("Pattern | Id");
	pattern_entry			= gtk_entry_new();

	field_box 			= gtk_combo_box_text_new();
	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(field_box), NULL, "any");
	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(field_box), NULL, "id");
	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(field_box), NULL, "name");
	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(field_box), NULL, "url");
	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(field_box), NULL, "comment");
	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(field_box), NULL, "tag");

	GtkTreeIter	iter;
	gtk_tree_model_get_iter_first(GTK_TREE_MODEL
		((gtk_combo_box_get_model(GTK_COMBO_BOX(field_box))))
		,&iter);
	gtk_combo_box_set_active_iter(GTK_COMBO_BOX(field_box), &iter);

	/* buttons */
	GtkWidget* 	apply_button 	= gtk_button_new_with_mnemonic("_Apply");
	g_signal_connect(GTK_WIDGET(apply_button), "clicked", G_CALLBACK(selective_copy)
		,action);

	GtkWidget* 	cancel_button	= gtk_button_new_with_mnemonic("_Cancel");
	g_signal_connect(GTK_WIDGET(cancel_button), "clicked", G_CALLBACK(close_window)
		,window);

	/* button box */
	GtkWidget* 	button_box  	= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	gtk_box_pack_end(GTK_BOX(button_box), cancel_button, FALSE, FALSE, 1);
	gtk_box_pack_end(GTK_BOX(button_box), apply_button, FALSE, FALSE, 1);

	/* grid */
	GtkWidget* 	grid 		= gtk_grid_new();
	gtk_grid_set_column_spacing(GTK_GRID(grid), 2);
	gtk_grid_set_row_spacing(GTK_GRID(grid), 2);
	gtk_grid_set_column_homogeneous(GTK_GRID(grid), 1);

	gtk_grid_attach(GTK_GRID(grid), db_entry_label 	,0,   0, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), db_entry 	,30,  0, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), select_button 	,60,  0, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), pattern_label 	,0,   1, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), pattern_entry 	,30,  1, 30, 1);
	gtk_grid_attach(GTK_GRID(grid), field_box 	,60,  1, 30, 1);

	/* main box */
	GtkWidget* 	main_box  	= gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
	gtk_box_pack_start(GTK_BOX(main_box), grid, TRUE, TRUE, 1);
	gtk_box_pack_end(GTK_BOX(main_box), button_box, FALSE, FALSE, 1);
	gtk_container_add(GTK_CONTAINER(window), main_box);

	gtk_widget_show_all(GTK_WIDGET(window));
	gtk_spinner_start(GTK_SPINNER(spinner));
}

void
selective_import_window(GtkWidget* button, gpointer main_window)
{
	unsigned int 	action = 0;
	GtkWidget* 	window = dialogs("Selective import", main_window);
	selective_dialog_content(window, &action);
}

void
selective_export_window(GtkWidget* button, gpointer main_window)
{
	unsigned int 	action = 1;
	GtkWidget* 	window = dialogs("Selective export", main_window);
	selective_dialog_content(window, &action);
}

