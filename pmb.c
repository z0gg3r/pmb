/***************************************
 * poor man's bookmark, by neoncortex  *
 * version:	0.1, 17/07/2017        * 
 ***************************************/

#include "parser.h"

int 
main(int argc, char *argv[]) 
{
	/* set default colors */
	id_color 	= blue;
	name_color 	= white;
	url_color 	= cyan;
	comment_color 	= white;
	tag_color 	= green;

	const char* 	home 	= secure_getenv("HOME");	

	/* database file */
	char* 		file 	= calloc((strlen(home) 
				+ strlen(DATABASE) + 2), sizeof(char));

	/* config file */
	char* 	config_file 	= calloc((strlen(home) 
				+ strlen(CONFIG_FILE) + 2), sizeof(char));

	snprintf(config_file, strlen(config_file) - 1, "%s/%s"
				,home, CONFIG_FILE);

	parse_config_file(config_file);
	free(config_file);

	/* parse options */
	cl_option_list* option 	= option_list_create();
	cl_option_list* command = option_list_create();
	parse_options(argc, argv, option, command);
	exec_option(option);

	if(!db)
	{
		if(file) 
		{
			snprintf(file, strlen(file) - 1, "%s/%s"
				,home, DATABASE);

			db = bookmark_db_open(file);
			free(file);
		}
		else
			exit(EXIT_FAILURE);
	}

	/* testing the tree */
	/*
	bookmark_i* 	b 	= NULL;
	bookmark_list* 	bl 	= bookmark_db_query(db, 0, NULL);
	directory* 	d 	= directory_create("root");	
	directory*	ret	= NULL;

	for(int i = 0; i < bookmark_list_get_size(bl) - 1; ++i) 
	{
		b = bookmark_return_next(bl);
		directory_add_children_from_list(d, (dismember(b)), b);
	}

	directory_rewind(d);

	while((ret = directory_return_next_children(d)))
	{
		printf("%s\n", directory_name(ret));
	}

	bookmark_i_destroy(b);
	bookmark_list_destroy(bl);
	directory_destroy(ret);
	directory_destroy(d);
	*/

	exec_option(command);
	destroy_option_list(option);
	destroy_option_list(command);
	bookmark_db_close(db);
	exit(EXIT_SUCCESS);
}

