#ifndef TREE_H
#define TREE_H
#define _GNU_SOURCE 	/* strsep */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

typedef struct directory directory;
typedef struct directory_name_list directory_name_list;

/* -- create a new directory object -- */
/* char* name */
directory* 
directory_new(char*);

/* -- return directory name -- */
char*
directory_name(directory*);

/* -- return how many childrens directory have -- */
int
directory_childrens(directory*);

/* -- return children_position on children** of directory -- */
int
directory_children_position(directory*);

/* -- return how many bookmarks directory have -- */
int
directory_bookmarks(directory*);

/* -- return bookmark_position on bookmark** of directory -- */
int
directory_bookmark_position(directory*);

/* -- set children_position to 0 -- */
void
directory_children_rewind(directory*);

/* -- set bookmark_position to 0 -- */
void
directory_bookmark_rewind(directory*);

/* -- set bookmark_position and children_position to 0 -- */
void
directory_rewind(directory*);

/* -- return next directory children -- */
directory*
directory_next_children(directory*);

/* -- return next bookmark of directory -- */
/* directory* d */
bookmark*
directory_next_bookmark(directory*);

/* -- destroy a directory -- */
void
directory_destroy(directory*);

/* -- add a children to directory -- */
/* directory* d, directory* children */
void
directory_add_children(directory*, directory*);

/* -- add a bookmark to directory */
/* directory* d, bookmark* b */
void
directory_add_bookmark(directory*, bookmark*);

/* -- delete children of directory on position -- */
/* directory* d, int position */
int
directory_delete_children(directory*, int);

/* -- delete bookmark of directory on position -- */
/* directory* d, int position */
int
directory_delete_bookmark(directory*, int);

/* -- edit the name of directory -- */
/* directory* d, char* name */
directory*
directory_edit_name(directory*, char*);

/* -- verify if directory contain children -- */
/* directory* d, char* children_name */
directory*
directory_contain_children(directory*, char*);

/* -- verify if directory contain bookmark -- */
/* directory* d, int bookmark_id */
bookmark*
directory_contain_bookmark(directory*, int);

/* -- create a array of directory* from a directory_name_list* -- */
directory**
create_directory_tree_from_list(directory_name_list*, int);

/* -- add childrens to directory from directory_name_list and add bookmark to last child -- */
/* directory* d, directory_name_list* l, bookmark* b */
void
directory_add_children_from_list(directory*, directory_name_list*, bookmark*);

/* -- delete the last children of directory from directory_name_list -- */
void
directory_delete_last_children_from_list(directory*, directory_name_list*);

/* -- delete, from root to leaf based on directory_name_list path -- */
void
directory_delete_path(directory*, directory_name_list*);

/* -- edit leaf based on directory_name_list path -- */
void
directory_edit_last_children_name_from_list(directory*, directory_name_list*, char*);

/* -- rename directory who have current_name to new_name -- */
/* directory *d, char* current_name, char* new_name, directory_name_list* l */
void
directory_edit_name_from_list(directory*, char*, char*, directory_name_list*);

/* -- move leaf from directory r to directory d -- */
/* directory* d, directory* r */
void
directory_move_children(directory*, directory*, char*);

/* -- move all childrens from directory r to directory d -- */
/* directory* d, directory* r */
void
directory_move_children_all(directory*, directory*);

/* -- make a tree from bookmark_list content -- */
/* bookmark_list* bl, char* name */
directory*
create_tree_from_bookmark_list(bookmark_list*, char*);

/* -- create a new directory_name_list object -- */
directory_name_list*
directory_name_list_new();

/* -- destroy a directory_name_list object -- */
void
directory_name_list_destroy(directory_name_list*);

/* -- return size of directory_name_list -- */
int
directory_name_list_size(directory_name_list*);

/* -- return position on directory_name_list -- */
int
directory_name_list_position(directory_name_list*);

/* -- add a name to directory_name_list -- */
/* directory_name_list* l, char* name */
void
directory_name_list_add_dir(directory_name_list*, char*);

/* -- return the next name of directory_name_list -- */
char*
directory_name_list_return_next(directory_name_list*);

/* -- return the last name of directory_name_list -- */
char*
directory_name_list_return_last(directory_name_list*);

/* -- set position of directory_name_list to 0 -- */
void
directory_name_list_rewind(directory_name_list*);

/* -- cut the bookmark* b->tag string and return it in directory_name_list* -- */
directory_name_list*
dismember(bookmark*);

/* -- pop out last name of directory_name_list -- */
char*
directory_name_list_pop(directory_name_list*);

#endif

