#ifndef PARSER_H
#define PARSER_H

#define _GNU_SOURCE /* getsubopt, secure_getenv */
#define VERSION "0.0.2"
#define CONFIG_FILE "pmb.conf"

#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> /* isatty */
#include <limits.h>
#include <stdlib.h>
#include "option.h"
#include "tree.h"
#include "html.h"
#include "favicon.h"

extern sqlite3 *g_db;

/* int argc, char* argv[], cl_option_list*, cl_option_list* */
void parse_options(int, char *[], cl_option_list *, cl_option_list *);

/* -- add a new bookmark */
int add_bookmark(char *);

/* -- set colors */
int color_list(char *);

/* -- delete */
int delete_bookmark(char *);

/* -- edit */
int edit_bookmark(char *);

/* -- show help on terminal */
void help();

/* -- html output */
int html(char *);

/* -- import bookmarks from a database */
int import(char *);

/* -- open a new database file */
int open_file(char *);

/* read config file */
int parse_config_file(char *);

/* -- print */
int print_bookmark(char *);

/* -- search bookmarks on database */
int search(char *);

/* -- turn on/off terminal color output */
int set_color(char *);

/* -- turn on/off verbose output */
int set_verbose(char *);

/* -- show version on terminal */
void version();

#endif
