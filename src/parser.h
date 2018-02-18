#ifndef PARSER_H
#define PARSER_H

#define _GNU_SOURCE 	/* getsubopt, secure_getenv */
#define VERSION 	"0.0.1"
#define CONFIG_FILE	"pmb.conf"

#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> 	/* isatty */
#include <limits.h>
#include <stdlib.h>
#include "option.h"
#include "tree.h"
#include "html.h"

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

extern int 	color;
extern int 	verbose;
extern sqlite3* db;

/* read config file */
char**
read_config(char*); 

/* int argc, char* argv[], cl_option_list*, cl_option_list* */
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

#endif

