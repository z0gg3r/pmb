#ifndef BOOKMARK_LIST_H
#define BOOKMARK_LIST_H

#include <stdlib.h>
#include <string.h>
#include "bookmark.h"

typedef struct bookmark_list bookmark_list;

/* -- new bookmark_list -- */
bookmark_list*
bookmark_list_new(); 

/* -- destroy the list -- */
void 
bookmark_list_destroy(bookmark_list*);

/* -- add a bookmark to bookmark_list -- */
/* bookmark_list* l, char* id, char* name, char* url, char* comment
	,char* tag */
int
bookmark_list_enqueue(bookmark_list* 
	,char*, char*, char*, char*, char*, char*);

/* -- add a bookmark to bookmark_list -- */
int
bookmark_list_enqueue_bookmark(bookmark_list*, bookmark*); 

/* -- get items of list destroying it [ dequeue ] 
 * -- need to realign the list at every call, may be slow */
char** 
bookmark_list_dequeue(bookmark_list*);

/* -- get elements of the list without destroy it [ no dequeue ] -- */
char** 
bookmark_list_return_next(bookmark_list*);

bookmark*
bookmark_list_return_next_bookmark(bookmark_list*);

/* -- get next position -- */
int 
bookmark_list_get_next_position(bookmark_list*);

/* -- get the size of a bookmark_list -- */
int 
bookmark_list_get_size(bookmark_list*);

/* -- get the current position in a bookmark_list -- */
int 
bookmark_list_get_position(bookmark_list*);

/* -- rewind a bookark list to position 0 -- */
void
bookmark_list_rewind(bookmark_list*);

#endif

