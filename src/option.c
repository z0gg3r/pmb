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

  if(opt)
    {
      opt->func	= func;	
      opt->optarg = optarg;

      return opt;
    }

  return NULL;
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

  if(l) 
    {
      l->position = 0;
      l->size = 1;

      l->opt = calloc(l->size, sizeof(cl_option*));
      check_oom(l->opt, "option > option_list_new - l->opt");
      return l;
    }

  return NULL;
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
