#ifndef PARSER

#define _GNU_SOURCE 	/* getsubopt, secure_getenv */
#define VERSION 	"0.0.1"
#define CONFIG_FILE	".pmb.conf"

#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> 	/* isatty */
#include <limits.h>
#include <stdlib.h>
#include "bookmark.h"

extern char* red;
extern char* green;
extern char* yellow;
extern char* blue;
extern char* magenta;
extern char* cyan;
extern char* gray;
extern char* white;
extern char* reset;

extern char* id_color;
extern char* name_color;
extern char* url_color;
extern char* comment_color;
extern char* tag_color;

extern short	color;
extern short	verbose;
extern sqlite3* db;

typedef struct 
cl_option
{
	int	(*func)(char*); 
	char* 	optarg;
} cl_option;

typedef struct 
cl_option_list 
{
	int 		size;
	int 		position;
	cl_option** 	opt;
} cl_option_list;

/* -- create an option structure's */
cl_option*
create_option(int(*)(char*), char*);

/* -- destroy option structure's */
int
destroy_option(cl_option*);

/* -- destroy option list's */
int
destroy_option_list(cl_option_list*);

/* used to exec parsed options */
void
exec_option(cl_option_list*);

/* -- create option list's */
cl_option_list*
option_list_create();

/* -- add option to option list */
int
option_list_add(cl_option_list*, cl_option*);

/* read config file */
char**
read_config(char*); 

void
parse_options(int, char*[], cl_option_list*
	,cl_option_list*);

/* -- add a new bookmark */
int
add_bookmark(char*);

/* -- set colors */
int
color_list(char*);

/* -- delete */
int
delete_bookmark(char*);

/* -- edit */
int
edit_bookmark(char*);

/* -- show help on terminal */
void
help();

/* -- html output */
int
html(char*); 

/* -- import bookmarks from a database */
int
import(char*);

/* -- open a new database file */
int
open_file(char*);

/* read config file */
int
parse_config_file(char*);

/* -- print */
int
print_bookmark(char*);

/* -- search bookmarks on database */
int
search(char*);

/* -- turn on/off terminal color output */
int
set_color(char*);

/* -- turn on/off verbose output */
int
set_verbose(char*);

/* -- show version on terminal */
void 
version();

#define PARSER
#endif

