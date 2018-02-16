#include "file.h"

void
new_database(GtkWidget* button, gpointer window)
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
open_database(GtkWidget* button, gpointer window)
{
	gtk_spinner_start(GTK_SPINNER(spinner));

	GtkWidget* file_chooser = gtk_file_chooser_dialog_new("Open database"
			,window
			,GTK_FILE_CHOOSER_ACTION_OPEN
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

