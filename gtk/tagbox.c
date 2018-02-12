#include "tagbox.h"

GtkWidget* tag_box = NULL;

static int
compare_tags(GtkTreeModel* m, GtkTreePath* path, GtkTreeIter* iter, void* data)
{
	bookmark* 	b 		= get_data(NULL);
	char*		b_tag		= bookmark_tag(b);
	char* 		t_path		= NULL;
	char*		full_path	= get_full_path(b);

	bookmark_destroy(b);

	gtk_tree_model_get(GTK_TREE_MODEL(m), iter, 0, &t_path, -1);

	if(!(strcmp(b_tag, t_path))
	||(!(strcmp(full_path, t_path))))
	{
		gtk_combo_box_set_active_iter(GTK_COMBO_BOX(tag_box), iter);
		free(full_path);
		return 1;
	}

	return 0;
}

GtkWidget*
tag_box_new()
{
	tag_box 			= gtk_combo_box_text_new_with_entry();
	GtkWidget* 	entry 		= gtk_bin_get_child(GTK_BIN(tag_box));
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
	
	b = get_data(NULL);
	gtk_entry_set_text(GTK_ENTRY(entry), get_full_path(b));
	bookmark_destroy(b);
	
	return tag_box;
}

