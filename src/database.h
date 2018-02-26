#ifndef DATABASE_H
#define DATABASE_H
#define _GNU_SOURCE /* asprintf, secure_getenv */

#include <limits.h>
#include <stdio.h>
#include <sqlite3.h>
#include <time.h>
#include "bookmark.h"
#include "bookmark_list.h"

char* date(char*, int);

/* open database - full file path */
sqlite3* 
bookmark_db_open(char*);

/* sqlite3 database */
void 
bookmark_db_close(sqlite3*);

/* bookmark* bookmark, sqlite3* database */
int 
bookmark_db_write(bookmark*, sqlite3*);

/* -- import one database into another (second into the first) */
/* sqlite3* database, sqlite3* database */
int
bookmark_db_import(sqlite3*, sqlite3*);

/* sqlite3* database, int item id */
int 
bookmark_db_delete(sqlite3*, int);

/* -- delete all bookmarks tagged with char* tag */
/* -- if greedy = 1, it will delete any tag containing 
   the words in tag */
/* sqlite3* database, char* tag, int greedy */
int
bookmark_db_delete_tag(sqlite3*, char*, int);

/* sqlite3* database, int item id, int database field/column, char* new value */
int 
bookmark_db_edit(sqlite3*, int, int, char*);

/* sqlite3* database, int database field/column, char* current value
   ,char* new value */
int 
bookmark_db_edit_bulk(sqlite3*, int, char*, char*);

/* sqlite3* database, char* database field/column, char* string to search */
bookmark_list* 
bookmark_db_search(sqlite3*, char*, char*);

/* sqlite3* database, int database item id, char* database field/column */
bookmark_list* 
bookmark_db_query(sqlite3*, int, char*);

#endif
