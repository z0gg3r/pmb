#define _GNU_SOURCE /* secure_getenv */

/***************************************
 * poor man's bookmark, by neoncortex  *
 * version:	0.1, 17/07/2017        * 
 ***************************************/

#include <sys/stat.h>
#include "src/parser.h"

int 
main(int argc, char *argv[]) 
{
  /* set default colors */
  id_color = blue;
  name_color = white;
  url_color = cyan;
  comment_color	= white;
  tag_color = green;

  /* dir path */
  const char* home = secure_getenv("HOME");	
  char*	conf_dir = ".config/pmb";
  char*	path = calloc
    (1, (strlen(home) + strlen(conf_dir) + 3) * sizeof(char));

  snprintf(path, strlen(path) - 1, "%s/%s", home, conf_dir);

  struct stat st = {0};

  if(stat(path, &st) == -1)
    {
      mkdir(path,  0700);
    }

  /* database */
  int d_size =
    (strlen(home) + (strlen(conf_dir)) + strlen(DATABASE) + 5) * sizeof(char);

  char*	database_file = calloc(1, d_size);

  snprintf(database_file, d_size - 1, "%s/%s" 
	   ,path, DATABASE);

  /* config file */
  int c_size =
    (strlen(home) + (strlen(conf_dir)) + strlen(CONFIG_FILE) + 5)
    * sizeof(char);

  char* config_file = calloc(1, c_size);

  snprintf(config_file, c_size - 1, "%s/%s"
	   ,path, CONFIG_FILE);

  parse_config_file(config_file);
  free(config_file);

  /* parse options */
  cl_option_list* option = option_list_new();
  cl_option_list* command = option_list_new();

  parse_options(argc, argv, option, command);
  exec_option(option);

  if(!g_db)
    {
      if(database_file) 
	{
	  g_db = bookmark_db_open(database_file);
	  free(database_file);
	}
      else
	{
	  exit(EXIT_FAILURE);
	}
    }

  exec_option(command);
  destroy_option_list(option);
  destroy_option_list(command);
  bookmark_db_close(g_db);
  exit(EXIT_SUCCESS);
}
