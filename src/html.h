#ifndef HTML_H
#define HTML_H

#include "tree.h"

extern sqlite3 *g_db;

int bookmark_html_tree(bookmark_list *, FILE *);

#endif
