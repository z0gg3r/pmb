#include "file.h"

void
open_database(gpointer window)
{
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
}

