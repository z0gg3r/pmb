#include "tagbox.h"

GtkWidget* tag_box = NULL;

static int
compare_tags(GtkTreeModel* m, GtkTreePath* path, GtkTreeIter* iter, void* data)
{
	bookmark* 	b 		= get_data(NULL);
	char*		b_tag		= bookmark_tag(b);
	char*		b_id		= bookmark_id(b);
	char* 		t_path		= NULL;

	bookmark_destroy(b);

	gtk_tree_model_get(GTK_TREE_MODEL(m), iter, 0, &t_path, -1);

	/* get full path of selected row */
	GtkTreeIter	n_iter, p_iter;
	unsigned int 	size 		= 1;
	char**		parents 	= calloc(size, sizeof(char*));
	char* 		full_path 	= calloc(1, sizeof(char));

	gtk_tree_model_get_iter(GTK_TREE_MODEL(model), &n_iter, selected_path);

	while(gtk_tree_model_iter_parent(GTK_TREE_MODEL(model), &p_iter, &n_iter))
	{
		GtkTreePath*	n_path 	= gtk_tree_model_get_path(GTK_TREE_MODEL(model), &p_iter);
		bookmark* 	tb 	= get_data(n_path);
		parents[size - 1] 	= calloc(1, (strlen(bookmark_id(tb)) + 1) * sizeof(char));
		parents[size - 1] 	= bookmark_id(tb);
		size++;
		parents 		= realloc(parents, size * sizeof(char*));
		n_iter 			= p_iter;

		bookmark_destroy(tb);
		gtk_tree_path_free(n_path);
	}
	
	for(unsigned int i = size - 1; i > 0; --i)
	{
		char* temp 	= parents[i - 1];
		char* full_copy = strdup(full_path);
		int   size	= ((strlen(temp) * sizeof(char))
				 	+ (strlen(full_copy) * sizeof(char) + 3) 
					* sizeof(char));

		full_path = realloc(full_path, size);
		snprintf(full_path, size - 1, "%s/%s", full_copy, temp);
		free(parents[i - 1]);
		free(full_copy);
	}

	char* full_copy = strdup(full_path);
	int   full_size	= ((strlen(full_copy) * sizeof(char)) 
			 	+ (strlen(b_id) * sizeof(char) + 3) 
				* sizeof(char));

	full_path = realloc(full_path, full_size);
	snprintf(full_path, full_size - 1, "%s/%s", full_copy, b_id);
	
	char* full_path_copy = strdup(full_path);
	strsep(&full_path, "//");
	
	free(full_copy);
	free(parents);

	if(!(strcmp(b_tag, t_path))
	||(!(strcmp(full_path, t_path))))
	{
		gtk_combo_box_set_active_iter(GTK_COMBO_BOX(tag_box), iter);
		free(full_path_copy);
		return 1;
	}

	return 0;
}

GtkWidget*
tag_box_new()
{
	tag_box 			= gtk_combo_box_text_new();
	bookmark_list*	bl		= bookmark_db_query(db, 0, NULL);
	bookmark*	b		= NULL;
	char*		last_tag	= NULL;

	while((b = bookmark_list_return_next_bookmark(bl)))
	{
		char* tag = bookmark_tag(b);

		if((last_tag && strcmp(last_tag, tag)) 
		|| (!last_tag))
		{
			last_tag = bookmark_tag(b);

			gtk_combo_box_text_append
				(GTK_COMBO_BOX_TEXT(tag_box)
				,NULL
				,last_tag);
		}

		bookmark_destroy(b);
	}

	if(bl)
		bookmark_list_destroy(bl);
	
	GtkTreeIter iter;	
	GtkTreeModel* tag_model = gtk_combo_box_get_model(GTK_COMBO_BOX(tag_box));
	gtk_tree_model_get_iter_first(GTK_TREE_MODEL(tag_model), &iter);
	gtk_combo_box_set_active_iter(GTK_COMBO_BOX(tag_box), &iter);
	gtk_tree_model_foreach(GTK_TREE_MODEL(tag_model), compare_tags, NULL);

	return tag_box;
}

