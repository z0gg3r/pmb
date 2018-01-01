#ifndef DIRECTORY_URL_H
#define DIRECTORY_URL_H

#include "interface.h"

typedef struct directory_url directory_url;

directory_url*
directory_url_new();

void
directory_url_destroy(directory_url*);

int
directory_url_size(directory_url*);

void
directory_url_position_first(directory_url* dir_url);

char*
directory_url_next(directory_url*);

/* fill the char** arg with url's inside the current
   selected directory of tree view */
void
collect_directory_url(GtkTreeIter, directory_url*);

#endif
