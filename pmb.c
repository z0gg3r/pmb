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
  unsigned int size = 0;
  
  /* dir path */
  const char* home = secure_getenv("HOME");	
  char*	conf_dir = ".config/pmb";

  size = strlen(home)
    + strlen(conf_dir)
    + 3;
  
  char*	path = calloc(size, sizeof(char));

  check_oom(path, "path");
  
  snprintf(path, strlen(path) - 1, "%s/%s", home, conf_dir);

  /* create directory, if not exist */
  {
    struct stat st = {0};

    if(stat(path, &st) == -1)
      {
	mkdir(path,  0700);
      }
  }

  /* database */
  size = strlen(path)
    + strlen(DATABASE)
    + 3;

  char*	database_file = calloc(size, sizeof(char));

  check_oom(database_file, "database_file");
  
  snprintf(database_file, strlen(database_file) - 1, "%s/%s" 
	   ,path, DATABASE);

  /* config file */
  size = strlen(home)
    + strlen(conf_dir)
    + strlen(CONFIG_FILE)
    + 3;

  char* config_file = calloc(size, sizeof(char));

  check_oom(config_file, "config_file");
  
  snprintf(config_file, strlen(config_file) - 1, "%s/%s"
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
  free(path);
  bookmark_db_close(g_db);
  exit(EXIT_SUCCESS);
}
