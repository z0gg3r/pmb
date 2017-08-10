#ifndef BOOKMARK

#define ID 		"id"
#define NAME 		"name"
#define URL 		"url"
#define COMMENT 	"comment"
#define TAG 		"tag"
#define DATABASE 	".bookmarks.db"
#include <sqlite3.h>

typedef struct bookmark bookmark;
typedef struct bookmark_list bookmark_list;

char* date(char*, int);

/* -- create a new bookmark object */
bookmark* 
bookmark_create(); 

/* -- destroy a bookmark object */
void 
bookmark_destroy(bookmark*);

/* bookmark *b, char *name, char *url, char *comment, char *tag */
int 
bookmark_add(bookmark*, char*, char*, char*, char*);

/* open database - full file path */
sqlite3* 
bookmark_db_open(char*);

/* sqlite3 database */
void 
bookmark_db_close(sqlite3*);

/* bookmark* bookmark, sqlite3* database */
int 
bookmark_db_write(bookmark*, sqlite3*);

/* -- import one database into another (secound into the first) */
/* sqlite3* database, sqlite3* database */
int
bookmark_db_import(sqlite3*, sqlite3*);

/* sqlite3* database, int item id */
int 
bookmark_db_delete(sqlite3*, int);

/* -- delete all bookmarks tagged with char* tag */
/* -- if int greedy = 1, it will delete any tag containing 
	the words in char* tag */
/* sqlite3 database, char* tag, int greedy */
int
bookmark_db_delete_tag(sqlite3*, char*, int);

/* sqlite3* database, int item id, int database field/column, char* new value */
int 
bookmark_db_edit(sqlite3*, int, int, char*);

/* sqlite3 database, int database field/column, char *current value
 	,char *new value */
int 
bookmark_db_edit_bulk(sqlite3*, int, char*, char*);

/* sqlite3* database, char* database field/column, char* string to search */
bookmark_list* 
bookmark_db_search(sqlite3*, char*, char*);

/* sqlite3* database, int database item id, char* database field/column */
bookmark_list* 
bookmark_db_query(sqlite3*, int, char*);

/* -- destroy the list */
void 
bookmark_list_destroy(bookmark_list*);

/* -- get items of list destroying it [ dequeue ] 
 * -- need to realign the list at every call, may be slow */
char** 
bookmark_list_dequeue(bookmark_list*);

/* -- get elements of the list without destroy it [ no dequeue ] */
char** 
bookmark_list_return_next(bookmark_list*);

/* -- get next position */
int 
bookmark_list_get_next_position(bookmark_list*);

/* -- get the size of a bookmark_list */
int 
bookmark_list_get_size(bookmark_list*);

/* -- get the current position in a bookmark_list */
int 
bookmark_list_get_position(bookmark_list*);

/* -- rewind a bookark list to position 0 */
void
bookmark_list_rewind(bookmark_list*);

#define BOOKMARK
#endif
