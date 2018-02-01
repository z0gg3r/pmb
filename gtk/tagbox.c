#include "tagbox.h"

GtkWidget*
tag_box_new()
{
	GtkWidget* 	tag_box 	= gtk_combo_box_text_new();
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

	return tag_box;
}

