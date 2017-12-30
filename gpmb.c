#include "gtk/interface.h"

int 
main(int argc, char *argv[]) 
{
	const char* 	home = secure_getenv("HOME");	
	char* 		file = calloc((strlen(home) + strlen(DATABASE)
				 + 2), sizeof(char));

	if(file) 
	{
		snprintf(file, strlen(file) - 1, "%s/%s", home
			,DATABASE);

		db = bookmark_db_open(file);
		free(file);
	}
	else
		exit(EXIT_FAILURE);

	read_options();
	short res = gtk_interface(argc, argv);

	free(opts);
	bookmark_db_close(db);
	return res;
}

