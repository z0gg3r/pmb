#define _GNU_SOURCE 	/* secure_getenv */

#include <sys/stat.h>
#include "src/gtk/interface.h"

char* database_file = NULL;

int 
main(int argc, char *argv[]) 
{
	/* dir path */
	const char* 	home 	= secure_getenv("HOME");	
	char*		conf_dir 	= ".config/pmb";
	char*		path		= calloc(1, 
								(strlen(home)
								+ strlen(conf_dir)
								+ 3) * sizeof(char));

	snprintf(path, strlen(path) - 1, "%s/%s", home, conf_dir);

	struct stat st = {0};

	if(stat(path, &st) == -1)
		mkdir(path,  0700);

	/* database */
	int		size	= (strlen(home)
						+ (strlen(conf_dir))
						+ strlen(DATABASE)
						+ 5) * sizeof(char);

	database_file 	= calloc(1, size);

	snprintf(database_file, size, "%s/%s" 
		,path, DATABASE);

	if(database_file) 
	{
		read_config();
		free(path);
		db = bookmark_db_open(opts->database_file);
	}
	else
		exit(EXIT_FAILURE);

	short res = gtk_interface(argc, argv);

	free(opts);
	free(database_file);
	bookmark_db_close(db);
	return res;
}

