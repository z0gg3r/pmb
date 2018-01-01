#include "directory_url.h"

struct directory_url
{
	int 	size;
	int 	index;
	char** 	urls;	
};

directory_url*
directory_url_new()
{
	directory_url* dir_url = calloc(1, sizeof(directory_url));

	dir_url->size 	= 1;
	dir_url->index 	= 0;
	dir_url->urls	= calloc(1, sizeof(char*));

	return dir_url;
}

void
directory_url_destroy(directory_url* dir_url)
{
	for(int i = 0; i < dir_url->size - 1; ++i)
	{
		if(dir_url->urls[i])
			free(dir_url->urls[i]);
	}

	if(dir_url)
		free(dir_url);
}

int
directory_url_size(directory_url* dir_url)
{
	return dir_url->size;
}

void
directory_url_position_first(directory_url* dir_url)
{
	dir_url->index = 0;
}

char*
directory_url_next(directory_url* dir_url)
{
	char* url = dir_url->urls[dir_url->index];
	dir_url->index++;

	return url;
}

void
directory_url_insert(directory_url* dir_url, char* url)
{
	dir_url->urls[dir_url->index] 	= calloc(strlen(url) + 1, sizeof(char));

	strcpy(dir_url->urls[dir_url->index], url);
	dir_url->index++;
	dir_url->size++;
	dir_url->urls 			= realloc(dir_url->urls, dir_url->size * sizeof(char*));
}

void
collect_directory_url(GtkTreeIter iter, directory_url* dir_url)
{
	GtkTreeIter 	child;
	GtkTreePath*	path;

	do
	{
		path 			= gtk_tree_model_get_path(GTK_TREE_MODEL(model), &iter);
		bookmark* 	b 	= get_data(path);

		if(strlen(bookmark_url(b)) > 1)
			directory_url_insert(dir_url, bookmark_url(b));

		bookmark_destroy(b);

		if(gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(model), &child, &iter, 0))
			collect_directory_url(child, dir_url);
	}
	while(gtk_tree_model_iter_next(GTK_TREE_MODEL(model), &iter));

	gtk_tree_path_free(path);
}

