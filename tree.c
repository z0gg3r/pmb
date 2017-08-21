#include "tree.h"

struct 
directory
{
	int			n_children;
	int			n_bookmark_i;
	int			children_position;
	int			bookmark_i_position;
	char*			name;
	bookmark_i** 		bookmark_i;
	struct directory** 	children;
};

struct 
directory_name_list
{
	int 	size;
	int 	position;
	char** 	list;
};

directory* 
directory_create(char* name)
{
	directory* d 		= malloc(sizeof(directory));

	d->n_children 		= 0;
	d->n_bookmark_i 		= 0;
	d->children_position 	= 0;
	d->bookmark_i_position 	= 0;
	d->name			= name;
	d->bookmark_i 		= calloc(1, sizeof(bookmark_i));
	d->children		= calloc(1, sizeof(directory));
	
	d->bookmark_i[0]		= NULL;
	d->children[0]		= NULL;
	return d;
}

char*
directory_name(directory* d)
{
	if(d)
		return d->name;
	else
		return NULL;
}

void
directory_children_rewind(directory* d)
{
	if(d)
		d->children_position = 0;
}

void
directory_bookmark_rewind(directory* d)
{
	if(d)
		d->bookmark_i_position = 0;
}

void
directory_rewind(directory* d)
{
	if(d)
	{
		d->children_position = 0;
		d->bookmark_i_position = 0;
	}
}

directory*
directory_return_next_children(directory* d)
{
	if(d && d->children_position <= d->n_children)
	{
		d->children_position++;
		return d->children[d->children_position - 1];
	}
	else
		return NULL;
}

bookmark_i*
directory_return_next_bookmark(directory* d)
{
	if(d && d->bookmark_i_position <= d->n_bookmark_i)
	{
		d->bookmark_i_position++;
		return d->bookmark_i[d->bookmark_i_position - 1];
	}
	else
		return NULL;
}

void
directory_destroy(directory* d)
{
	if(d)
	{
		directory_rewind(d);

		if(d->n_bookmark_i)
		{
			bookmark_i* b = NULL;

			while((b = directory_return_next_bookmark(d)))
			{
				if(b)
				{
					free(d->bookmark_i[d->bookmark_i_position]);	
					d->bookmark_i[d->bookmark_i_position] = NULL;
				}
			}
		}
		
		if(d->n_children)
		{
			directory* ret = NULL;

			while((ret = directory_return_next_children(d)))
			{
				if(ret)
				{
					directory_destroy(directory_return_next_children(d));
					free(d->children[d->children_position]);
					d->children[d->children_position] = NULL;
				}
			}
		}

		free(d->children);
		d->children = NULL;

		free(d->bookmark_i);
		d->bookmark_i = NULL;

		free(d);
		d = NULL;
	}
}

void
directory_add_children(directory* d, directory* children)
{
	if(d && children)
	{
		d->children[d->n_children] 	= children;
		d->n_children++;
		d->children 			= realloc(d->children
							,(d->n_children + 1) * sizeof(directory));
		d->children[d->n_children] 	= NULL;
	}
}

void
directory_add_bookmark(directory* d, bookmark_i* b)
{
	if(d && b)
	{
		d->bookmark_i[d->n_bookmark_i] 	= b;
		d->n_bookmark_i++;
		d->bookmark_i 			= realloc(d->bookmark_i
							,(d->n_bookmark_i + 1) * sizeof(bookmark_i));
		d->bookmark_i[d->n_bookmark_i] 	= NULL;
	}
}

int
directory_delete_children(directory* d, int index)
{
	if(d && index <= d->n_children)
	{
		directory_rewind(d);
		directory_destroy(d->children[index]);

		d->children[index] 		= NULL;
		directory* 	ret 		= NULL;
		directory** 	new_children 	= calloc(d->n_children - 1, sizeof(directory*));
		int 		j 		= 0;

		while((ret = directory_return_next_children(d)))
		{
			//printf("-> %d, %s\n", d->n_children, directory_name(ret));
		
			if(ret)
			{
				printf("i'm there\n");
				new_children[j] = ret;
				free(d->children[d->children_position]);
				d->children[d->children_position] = NULL;
				++j;
			}
		}

		free(d->children);
		d->children = new_children;
		d->n_children--;
		return 0;
	}

	return 1;
}

int
directory_delete_bookmark(directory* d, int index)
{
	if(d && index <= d->n_children)
	{
		free(d->bookmark_i[index]);
		d->bookmark_i[index] = NULL;

		int i, j = 0;
		bookmark_i** new_bookmark_i = calloc(d->n_bookmark_i, sizeof(bookmark_i*));

		for(i = 0; i < d->n_bookmark_i; ++i)
		{
			if(d->bookmark_i[i])
			{
				new_bookmark_i[j] = d->bookmark_i[i];
				free(d->bookmark_i[i]);
				++j;
			}
		}

		free(d->bookmark_i);
		d->bookmark_i = new_bookmark_i;
		d->n_bookmark_i--;
		directory_rewind(d);
		return 0;
	}

	return 1;
}

directory*
directory_edit_name(directory* d, char* name)
{
	if(d && name)
	{
		d->name = name;
		return d;
	}

	return NULL;
}

directory*
directory_contain_children(directory* d, char* children)
{
	if(d && children)
	{
		directory* 	ret 	= NULL;

		while((ret = directory_return_next_children(d)))
		{
			if(!(strcmp((directory_name(ret)), children)))
				return d;
		}
	}

	return NULL;
}

bookmark_i*
directory_contain_bookmark(directory* d, int id)
{
	if(d && id)
	{
		printf("%s\n", directory_name(d));
		directory* 	ret 	= NULL;
		bookmark_i*	b	= NULL;

		directory_rewind(d);
		//printf("container:: %s\n", directory_name(d));

		while((b = directory_return_next_bookmark(d)))
			if(b && id == (strtol(b->id, NULL, 10)))
				return b;

		while((ret = directory_return_next_children(d)))
			if((b = directory_contain_bookmark(ret, id)))
				return b;
	}

	return NULL;
}



directory_name_list*
directory_name_list_create()
{
	directory_name_list* d = malloc(sizeof(directory_name_list));

	d->size 	= 0;
	d->position	= 0;
	d->list 	= calloc(1, sizeof(char*));
	d->list[0] 	= NULL;

	return d;
}

void
directory_name_list_destroy(directory_name_list* d)
{
	if(d)
	{
		free(d->list);
		free(d);
	}
}

void
directory_name_list_add_dir(directory_name_list* d, char* name)
{
	if(d && name)
	{
		d->list[d->size] 	= name;
		d->size++;
		d->list 		= realloc(d->list, (d->size + 1) * sizeof(char*));
		d->list[d->size] 	= NULL;
	}
}

char*
directory_name_list_return_next(directory_name_list* d)
{
	if(d && d->position <= d->size - 1)
	{
		d->position++;
		return d->list[d->position - 1];
	}
	else
		return NULL;
}

void
directory_name_list_rewind(directory_name_list* d)
{
	if(d)
		d->position = 0;
}

directory_name_list*
dismember(bookmark_i* b)
{
	if(b)
	{
		char* 			name;
		char* 			path 	= malloc(strlen(b->tag) * sizeof(char));
		directory_name_list* 	l	= directory_name_list_create();

		strcpy(path, b->tag);

		while((name = strsep(&path, "/")))
			directory_name_list_add_dir(l, name);

		free(path);
		return l;
	}
	else
		return NULL;
}

char*
directory_name_list_pop(directory_name_list* l)
{
	if(l && l->size >= 0)
	{
		char* last = l->list[l->size];
		l->list[l->size] = NULL;
		l->size--;
		
		return last;
	}

	return NULL;
}

directory**
create_directory_tree_from_list(directory_name_list *l, int index)
{
	if(l)
	{
		if(index)
			l->position = index;
		else
			l->position = 0;

		char* 		ret 	= directory_name_list_return_next(l);
		directory* 	head 	= directory_create(ret);
		directory*	tail	= head;
		directory**	list	= calloc(2, sizeof(directory*));

		while((ret = directory_name_list_return_next(l)))
		{
			directory* tmp = directory_create(ret);
			directory_add_children(tail, tmp);
			tail = tmp;
		}

		if(head)
		{
			list[0] = head;
			list[1] = tail;
			directory_name_list_destroy(l);
			return list;
		}
	}

	return NULL;
}

void
directory_add_children_from_list(directory* d, directory_name_list* l, bookmark_i* b)
{
	if(d && l && b)
	{
		directory_rewind(d);

		char* 		ret 	= directory_name_list_return_next(l);
		directory*	child	= directory_contain_children(d, ret);;
		directory** 	list 	= NULL;
		
		if(child)
		{
			if(l->size == l->position)
				directory_add_bookmark(d, b);
			else
				directory_add_children_from_list(d->children[d->children_position - 1], l, b);
		}
		else
		{
			if(l->position > 1)
				list = create_directory_tree_from_list(l, l->position - 1);
			else
				list = create_directory_tree_from_list(l, 0);

			if(list)
			{
				directory_add_bookmark(list[1], b);
				directory_add_children(d, list[0]);
				free(list);
			}
		}
	}
}

void
directory_delete_last_children_from_list(directory* d, directory_name_list* l)
{
	if(d && l)
	{
		directory_rewind(d);

		char* 		ret 	= directory_name_list_return_next(l);
		directory*	child	= directory_contain_children(d, ret);;

		if(child)
		{
			char* last = l->list[l->size - 1];

			//printf("%s, %s\n", last, directory_name(d->children[d->children_position - 1]));

			if(!(strcmp(last, directory_name(d->children[d->children_position - 1]))))
			{
				printf("entered\n");
				directory_delete_children(d, d->children_position - 1);
			}
			else
				directory_delete_last_children_from_list(d->children[d->children_position - 1], l);
		}
	}
}

void
directory_delete_path(directory* d, directory_name_list* l)
{
	if(l)
	{
		for(;l->size;)
		{
			directory_delete_last_children_from_list(d, l);
			directory_name_list_pop(l);
			directory_name_list_rewind(l);
		}

		free(l);
	}
}

void
directory_edit_last_children_name_from_list(directory* d, directory_name_list* l, char* name)
{
	if(d && l)
	{
		directory_rewind(d);

		char* 		ret 	= directory_name_list_return_next(l);
		directory*	child	= directory_contain_children(d, ret);;

		if(child)
		{
			char* last = l->list[l->size - 1];

			if(!(strcmp(last, directory_name(d->children[d->children_position - 1]))))
				directory_edit_name(d->children[d->children_position - 1], name);
			else
				directory_edit_last_children_name_from_list(d->children[d->children_position - 1], l, name);
		}
	}
}

void
directory_edit_name_from_list(directory* d, char* name, char* new_name, directory_name_list* l)
{
	if(d && l)
	{
		directory_rewind(d);

		char* 		ret 	= directory_name_list_return_next(l);
		directory*	child	= directory_contain_children(d, ret);;

		if(child)
		{
			if(!(strcmp(ret, name)))
				directory_edit_name(d->children[d->children_position - 1], new_name);
			else
				directory_edit_name_from_list(d->children[d->children_position - 1], name, new_name, l);
		}
	}
}

void
directory_move_children(directory* receiver, directory* donator, char* name)
{
	if(receiver && donator && name)
	{
		directory*	ret	= NULL;

		directory_rewind(donator);
		directory_rewind(receiver);

		while((ret = directory_return_next_children(donator)))
		{
			if(!(strcmp(name, (directory_name(ret)))))
			{
				directory_add_children(receiver, ret);
				directory_delete_children(donator, donator->n_children);
			}
		}
	}
}

void
directory_move_children_all(directory* receiver, directory* donator)
{
	if(receiver && donator)
	{
		directory*	ret	= NULL;

		directory_rewind(donator);
		directory_rewind(receiver);

		while((ret = directory_return_next_children(donator)))
		{
			directory_add_children(receiver, ret);
			directory_delete_children(donator, donator->n_children);
		}
	}
}

