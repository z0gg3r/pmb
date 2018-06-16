#define _GNU_SOURCE /* secure_getenv */

#include <sys/stat.h>
#include "src/gtk/interface.h"

char* g_database_file = NULL;

int 
main(int argc, char *argv[]) 
{
  unsigned int size = 0;
  
  /* dir path */
  const char* home = secure_getenv("HOME");	
  char*	conf_dir = ".config/pmb";
  
  size = strlen(home)
    + strlen(conf_dir)
    + 2;
  
  char*	path = calloc(size, sizeof(char));
  check_oom(path, "path");
  
  snprintf(path, size, "%s/%s", home, conf_dir);
  
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
    + 2;

  g_database_file = calloc(size, sizeof(char));
  check_oom(g_database_file, "database_file");
  
  snprintf(g_database_file, size, "%s/%s" 
	   ,path, DATABASE);

  if(g_database_file) 
    {
      read_config();
      free(path);
      g_db = bookmark_db_open(DATABASE_OPTION);
    }
  else
    {
      exit(EXIT_FAILURE);
    }

  short res = gtk_interface(argc, argv);

  destroy_options();

  if(g_database_file)
    {
      free(g_database_file);
    }
  
  bookmark_db_close(g_db);
  return res;
}
