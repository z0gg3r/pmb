#include "option.h"

struct 
cl_option
{
  int (*func)(char*); 
  char* optarg;
};

struct 
cl_option_list 
{
  int size;
  int position;
  cl_option** opt;
};

cl_option*
option_new(int(*func)(char*), char* optarg) 
{
  cl_option* opt = calloc(1, sizeof(cl_option));
  check_oom(opt, "option - opt");
  
  opt->func = func;	
  opt->optarg = optarg;
      
  return opt;
}

int
destroy_option(cl_option* l)
{
  if(l) 
    {
      free(l);
      return 0;
    }

  return 1;
}

cl_option_list*
option_list_new() 
{
  cl_option_list* l = calloc(1, sizeof(cl_option_list));
  check_oom(l, "option - l");
  
  l->position = 0;
  l->size = 1;

  l->opt = calloc(l->size, sizeof(cl_option*));
  check_oom(l->opt, "option > option_list_new - l->opt");
  
  return l;
}

int
option_list_add(cl_option_list* l, cl_option* opt)
{
  if(l && opt) 
    {
      l->opt[l->position] = opt;

      ++l->position;
      ++l->size;
      
      l->opt = realloc(l->opt, l->size * sizeof(cl_option*));
      check_oom(l->opt, "option > option_list_add - l->opt");

      l->opt[l->position] = calloc(1, sizeof(cl_option*));
      check_oom(l->opt[l->position]
		,"option > option_list_add - l->opt[l->position]");
    }
  else
    {
      return 1;
    }

  return 0;
}

int
destroy_option_list(cl_option_list* l)
{
  if(l)
    {
      for(int i = 0; i < l->size - 1; ++i)
	{
	  destroy_option(l->opt[i]);
	}

      free(l->opt);
      free(l);
      return 0;		
    }

  return 1;
}

void
exec_option(cl_option_list* option)
{
  for(int i = 0; i < option->size - 1; ++i)
    {
      cl_option* opt = option->opt[i];
      opt->func(opt->optarg);
    }
}

char**
read_config(char* filename) 
{
  int st;
  FILE* fp = fopen(filename, "r");

  if(fp)
    {
      int size = 1;
      char* option_bkp = NULL;
      char* option = calloc(size, sizeof(char));
      check_oom(option, "parser - option");
      
      char** ret = calloc(7, sizeof(char*));
      check_oom(ret, "parser - ret");

      ret[0] = NULL;
      ret[1] = NULL;

      while(!feof(fp))
	{
	  st = fgetc(fp);

	  if(st == '\n')
	    {
	      option_bkp = strdup(option);
	      
	      if(!(strcmp(option, "\0")) 	/* blank line */
		 ||(option[0] == ' ')
		 ||(option[0] == '\t')
		 ||(option[0] == '#'))		/* comment */
		{
		  free(option);
		  goto new_option;
		}

	      char* str = strsep(&option, "=");

	      if(!(strcmp(str, "color")))
		{
		  ret[0] = strsep(&option, "=");
		}

	      else if(!(strcmp(str, "verbose")))
		{
		  ret[1] = strsep(&option, "=");
		}

	      else if(!(strcmp(str, "id_color")))
		{
		  ret[2] = strsep(&option, "=");
		}

	      else if(!(strcmp(str, "name_color")))
		{
		  ret[3] = strsep(&option, "=");
		}

	      else if(!(strcmp(str, "url_color")))
		{
		  ret[4] = strsep(&option, "=");
		}

	      else if(!(strcmp(str, "comment_color")))
		{
		  ret[5] = strsep(&option, "=");
		}

	      else if(!(strcmp(str, "tag_color")))
		{
		  ret[6] = strsep(&option, "=");
		}

	      else
		{
		  printf("unknown option: %s\n", str);
		  exit(EXIT_FAILURE);
		}

	    new_option:
	      free(option_bkp);
	      size = 1;
	      option = calloc(size, sizeof(char));
	      check_oom(option, "option > read_config - option");
	      option_bkp = NULL;
	    }
	  else
	    {
	      option[size - 1] = st;
	      ++size;
	      option = realloc(option, size * sizeof(char));
	      check_oom(option, "option > read_config - option");
	      option[size - 1] = '\0';
	    }
	}

      free(option);
      free(option_bkp);
      fclose(fp);
      return ret;
    }
  else
    {
      return NULL;
    }
}
