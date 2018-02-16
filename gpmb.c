#include "gtk/interface.h"

char* database_file = NULL;

int 
main(int argc, char *argv[]) 
{
	const char* 	home 	= secure_getenv("HOME");	
	database_file 		= calloc(1, (strlen(home) + strlen(DATABASE)
					 + 2) * sizeof(char));

	if(database_file) 
	{
		snprintf(database_file, strlen(database_file) - 1, "%s/%s", home
			,DATABASE);

		db = bookmark_db_open(database_file);
	}
	else
		exit(EXIT_FAILURE);

	read_options();
	short res = gtk_interface(argc, argv);

	free(opts);
	free(database_file);
	bookmark_db_close(db);
	return res;
}

