#ifndef TREE_H

#define _GNU_SOURCE 	/* strsep */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bookmark.h"

typedef struct directory directory;
typedef struct directory_name_list directory_name_list;

directory* 
directory_create(char* name);

char*
directory_name(directory*);

void
directory_children_rewind(directory*);

void
directory_bookmark_rewind(directory*);

void
directory_rewind(directory*);

directory*
directory_return_next_children(directory*);

bookmark_i*
directory_return_next_bookmark(directory*);

void
directory_destroy(directory*);

void
directory_add_children(directory*, directory*);

void
directory_add_bookmark(directory*, bookmark_i*);

int
directory_delete_children(directory*, int);

int
directory_delete_bookmark(directory*, int);

directory*
directory_edit_name(directory*, char*);

directory*
directory_contain_children(directory*, char*);

bookmark_i*
directory_contain_bookmark(directory*, int);

directory_name_list*
directory_name_list_create();

void
directory_name_list_destroy(directory_name_list*);

void
directory_name_list_add_dir(directory_name_list*, char*);

char*
directory_name_list_return_next(directory_name_list*);

void
directory_name_list_rewind(directory_name_list*);

directory_name_list*
dismember(bookmark_i*);

char*
directory_name_list_pop(directory_name_list*);

directory**
create_directory_tree_from_list(directory_name_list*, int);

void
directory_add_children_from_list(directory*, directory_name_list*, bookmark_i*);

void
directory_delete_last_children_from_list(directory*, directory_name_list*);

void
directory_delete_path(directory*, directory_name_list*);

void
directory_edit_last_children_name_from_list(directory*, directory_name_list*, char*);

void
directory_edit_name_from_list(directory* d, char*, char*, directory_name_list*);

void
directory_move_children(directory*, directory*, char*);

void
directory_move_children_all(directory*, directory*);

#define TREE_H
#endif
