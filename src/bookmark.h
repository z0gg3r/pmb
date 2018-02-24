#ifndef BOOKMARK_H
#define BOOKMARK_H
#define _GNU_SOURCE

#define ID 		"id"
#define NAME 		"name"
#define URL 		"url"
#define COMMENT 	"comment"
#define TAG 		"tag"
#define DATABASE 	"bookmarks.db"

#include <stdlib.h>

typedef struct bookmark bookmark;

/* -- create a new bookmark object -- */
bookmark* 
bookmark_new(); 

/* -- destroy a bookmark object -- */
void 
bookmark_destroy(bookmark*);

/* -- return bookmark id -- */
char*
bookmark_id(bookmark*);

/* -- return bookmark name -- */
char*
bookmark_name(bookmark*);

/* -- return bookmark url -- */
char*
bookmark_url(bookmark*);

/* -- return bookmark comment -- */
char*
bookmark_comment(bookmark*);

/* -- return bookmark tag -- */
char*
bookmark_tag(bookmark*);

/* -- return bookmark favicon -- */
char*
bookmark_favicon(bookmark*);

int
bookmark_set_id(bookmark*, char*);

int
bookmark_set_name(bookmark*, char*);

int
bookmark_set_url(bookmark*, char*);

int
bookmark_set_comment(bookmark*, char*);

int
bookmark_set_tag(bookmark*, char*);

int
bookmark_set_favicon(bookmark*, char*);

/* bookmark *b, char *name, char *url, char *comment, char *tag, char* favicon */
int 
bookmark_set(bookmark*, char*, char*, char*, char*, char*);

#endif
