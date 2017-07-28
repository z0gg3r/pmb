#define _GNU_SOURCE 		/* strsep */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

file_option*
create_file_option() 
{
	file_option* opt 	= calloc(1, sizeof(file_option));

	if(opt)
	{
		opt->size	= 1;
		opt->position	= 0;
		opt->option	= calloc(opt->size, sizeof(char));

		opt->option[0] 	= 0;
		return opt;
	}

	return NULL;
}

int
file_option_destroy(file_option* opt)
{
	if(opt) 
	{
		free(opt->option);
		free(opt);
		return 0;
	}

	return 1;
}

int
file_option_add_char(file_option* opt, char c)
{
	if(c) 
	{
		opt->option[opt->position] = c;
		++opt->position;

		if(opt->position >= opt->size - 1) 
		{
			opt->size++;
			char* noption 	= realloc(opt->option,  opt->size * sizeof(char));

			if(!noption) 
				return 1;
			else
			{
				opt->option = noption;
				opt->option[opt->size - 1] = '\0';
			}
		}
	}

	return 0;
}

char**
read_config(char* filename) 
{
	int 	st;
	FILE* 	fp = fopen(filename, "r");

	if(fp)
	{
		file_option* 	opt = create_file_option();
		char** 		ret = calloc(2, sizeof(char*));

		ret[0] = NULL;
		ret[1] = NULL;

		while(!feof(fp))
		{
			st = fgetc(fp);

			if(st == '\n')
			{
				char* str = strsep(&opt->option, "=");

				if(!(strcmp(str, "color")))
					ret[0] = strsep(&opt->option, "=");

				else if(!(strcmp(str, "verbose")))
					ret[1] = strsep(&opt->option, "=");

				file_option_destroy(opt);
				opt = create_file_option();
			}
			else
				file_option_add_char(opt, st);
		}

		file_option_destroy(opt);
		fclose(fp);
		return ret;
	}
	else
		return NULL;
}

