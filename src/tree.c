#include "tree.h"

struct 
directory
{
  int n_children;
  int n_bookmark;
  int children_position;
  int bookmark_position;
  char*	name;
  bookmark** bookmark;
  struct directory** children;
};

struct 
directory_name_list
{
  int size;
  int position;
  char** list;
};

directory* 
directory_new(char* name)
{
  directory* d = malloc(sizeof(directory));

  check_oom(d, "directory_new - b");
  
  if(d)
    {
      d->n_children = 0;
      d->n_bookmark = 0;
      d->children_position = 0;
      d->bookmark_position = 0;
      d->name = name;
      d->bookmark = calloc(1, sizeof(bookmark*));
      d->children = calloc(1, sizeof(directory*));
		
      d->bookmark[0] = NULL;
      d->children[0] = NULL;
      return d;
    }

  return NULL;
}

char*
directory_name(directory* d)
{
  if(d)
    {
      return d->name;
    }
  else
    {
      return NULL;
    }
}

int
directory_childrens(directory* d)
{
  if(d)
    {
      return d->n_children;
    }
  else
    {
      return 0;
    }
}

int
directory_children_position(directory* d)
{
  if(d)
    {
      return d->children_position;
    }
  else
    {
      return 0;
    }
}

int
directory_bookmarks(directory* d)
{
  if(d)
    {
      return d->n_bookmark;
    }
  else
    {
      return 0;
    }
}

int
directory_bookmark_position(directory* d)
{
  if(d)
    {
      return d->bookmark_position;
    }
  else
    {
      return 0;
    }
}

void
directory_children_rewind(directory* d)
{
  if(d)
    {
      d->children_position = 0;
    }
}

void
directory_bookmark_rewind(directory* d)
{
  if(d)
    {
      d->bookmark_position = 0;
    }
}

void
directory_rewind(directory* d)
{
  if(d)
    {
      d->children_position = 0;
      d->bookmark_position = 0;
    }
}

directory*
directory_next_children(directory* d)
{
  if(d && d->children_position <= d->n_children)
    {
      d->children_position++;
      return d->children[d->children_position - 1];
    }
  else
    {
      return NULL;
    }
}

bookmark*
directory_next_bookmark(directory* d)
{
  if(d && d->bookmark_position <= d->n_bookmark)
    {
      d->bookmark_position++;
      return d->bookmark[d->bookmark_position - 1];
    }
  else
    {
      return NULL;
    }
}

void
directory_destroy(directory* d)
{
  if(d)
    {
      directory_rewind(d);

      if(d->n_bookmark)
	{
	  bookmark* b = NULL;

	  while((b = directory_next_bookmark(d)))
	    {
	      if(b)
		{
		  bookmark_destroy(b);
		  free(d->bookmark[d->bookmark_position]);
		  d->bookmark[d->bookmark_position] = NULL;
		}
	    }

	  free(b);
	}
		
      if(d->n_children)
	{
	  directory* ret = NULL;

	  while((ret = directory_next_children(d)))
	    {
	      if(ret)
		{
		  directory_destroy(directory_next_children(d));
		  free(d->children[d->children_position]);
		  d->children[d->children_position] = NULL;
		}
	    }

	  free(ret);
	}

      free(d->children);
      free(d->bookmark);
      free(d);
    }
}

void
directory_add_children(directory* d, directory* children)
{
  if(d && children)
    {
      d->children[d->n_children] = children;
      d->n_children++;
      d->children = realloc(d->children, (d->n_children + 1)
			    * sizeof(directory));
      check_oom(d->children,
		"directory > directory_add_children - d->children");
      d->children[d->n_children] = NULL;
    }
}

void
directory_add_bookmark(directory* d, bookmark* b)
{
  if(d && b)
    {
      d->bookmark[d->n_bookmark] = b;
      d->n_bookmark++;
      d->bookmark = realloc(d->bookmark, (d->n_bookmark + 1)
			    * sizeof(bookmark*));
      check_oom(d->bookmark
		,"directory > directory_add_bookmark - d->bookmark");
      d->bookmark[d->n_bookmark] = NULL;
    }
}

int
directory_delete_children(directory* d, int index)
{
  if(d && index <= d->n_children)
    {
      directory_rewind(d);
      directory_destroy(d->children[index]);

      d->children[index] = NULL;
      directory* ret = NULL;
      directory** new_children = calloc(d->n_children - 1
					,sizeof(directory*));

      check_oom(new_children
		,"directory > directory_delete_children - new_children");
      
      int j = 0;

      while((ret = directory_next_children(d)))
	{
	  if(ret)
	    {
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
      free(d->bookmark[index]);
      d->bookmark[index] = NULL;

      int i, j = 0;
      bookmark** new_bookmark = calloc(d->n_bookmark
				       ,sizeof(bookmark*));

      check_oom(new_bookmark
		,"directory > directory_delete_bookmark - new_bookmark");
      
      for(i = 0; i < d->n_bookmark; ++i)
	{
	  if(d->bookmark[i])
	    {
	      new_bookmark[j] = d->bookmark[i];
	      free(d->bookmark[i]);
	      ++j;
	    }
	}

      free(d->bookmark);
      d->bookmark = new_bookmark;
      d->n_bookmark--;
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
      directory* ret = NULL;
      //directory* rret	= NULL;

      directory_rewind(d);

      while((ret = directory_next_children(d)))
	{
	  directory_rewind(ret);

	  if(!(strcmp((directory_name(ret)), children)))
	    {
	      return ret;
	    }

	  /*
	    if((rret = directory_contain_children(ret, children)))
	    return rret;
	  */
	}
    }

  return NULL;
}

bookmark*
directory_contain_bookmark(directory* d, int id)
{
  if(d && id)
    {
      directory* ret = NULL;
      bookmark*	b = NULL;

      directory_rewind(d);

      while((b = directory_next_bookmark(d)))
	{
	  if(b && id == (strtol((bookmark_id(b)), NULL, 10)))
	    {
	      return b;
	    }
	}

      while((ret = directory_next_children(d)))
	{
	  if((b = directory_contain_bookmark(ret, id)))
	    {
	      return b;
	    }
	}
    }

  return NULL;
}

directory_name_list*
directory_name_list_new()
{
  directory_name_list* d = malloc(sizeof(directory_name_list));

  if(d)
    {
      d->size = 0;
      d->position = 0;
      d->list = calloc(1, sizeof(char*));
      check_oom(d->list, "directory > directory_name_list_new - d->list");
      d->list[0] = NULL;
      return d;
    }

  return NULL;
}

void
directory_name_list_destroy(directory_name_list* l)
{
  if(l)
    {
      free(l->list);
      free(l);
    }
}

int
directory_name_list_size(directory_name_list* l)
{
  if(l)
    {
      return l->size;
    }

  return 0;
}

int
directory_name_list_position(directory_name_list* l)
{
  if(l)
    {
      return l->position;
    }

  return 0;
}

void
directory_name_list_add_dir(directory_name_list* l, char* name)
{
  if(l && name)
    {
      l->list[l->size] 	= name;
      l->size++;
      l->list = realloc(l->list, (l->size + 1) 
			* sizeof(char*));
      check_oom(l->list
		,"directory > directory_name_list_add_dir - l->list");
      l->list[l->size] = NULL;
    }
}

char*
directory_name_list_return_next(directory_name_list* l)
{
  if(l && l->position <= l->size - 1)
    {
      l->position++;
      return l->list[l->position - 1];
    }
  else
    {
      return NULL;
    }
}

char*
directory_name_list_return_last(directory_name_list* l)
{
  if(l && l->size)
    {
      return l->list[l->size - 1];
    }
  else
    {
      return NULL;
    }
}

void
directory_name_list_rewind(directory_name_list* l)
{
  if(l)
    l->position = 0;
}

directory_name_list*
dismember(bookmark* b)
{
  if(b)
    {
      char* name = NULL;
      char* tag	= bookmark_tag(b);
      char* path = malloc(strlen(tag) * sizeof(char) + 1);
      check_oom(path, "directory > dismember - path");
      char* path_bkp = NULL;
      directory_name_list* l = directory_name_list_new();

      strcpy(path, tag);
      path_bkp = strdup(path);
      
      while((name = strsep(&path, "/")))
	{
	  directory_name_list_add_dir(l, name);
	}

      free(path_bkp);

      if(l)
	{
	  return l;
	}
      else
	{
	  return NULL;
	}
    }
  else
    {
      return NULL;
    }
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
create_directory_tree_from_list(directory_name_list* l, int index)
{
  if(l)
    {
      if(index)
	{
	  l->position = index;
	}
      else
	{
	  l->position = 0;
	}

      char* ret = directory_name_list_return_next(l);
      directory* head = directory_new(ret);
      directory* tail = head;
      directory** list = calloc(2, sizeof(directory*));
      check_oom(list, "directory > create_direcory_tree_from_list - list");

      while((ret = directory_name_list_return_next(l)))
	{
	  directory* tmp = directory_new(ret);
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
directory_add_children_from_list(directory* d, directory_name_list* l
				 ,bookmark* b)
{
  if(d && l && b)
    {
      directory_rewind(d);

      char* ret = directory_name_list_return_next(l);
      directory* child = directory_contain_children(d, ret);;
      directory** list = NULL;
		
      if(child)
	{
	  if(l->size == l->position)
	    {
	      directory_add_bookmark(child, b);
	    }
	  else
	    {
	      directory_add_children_from_list
		(d->children[d->children_position - 1], l, b);
	    }
	}
      else
	{
	  if(l->position > 1)
	    {
	      list = create_directory_tree_from_list
		(l, l->position - 1);
	    }
	  else
	    {
	      list = create_directory_tree_from_list(l, 0);
	    }

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

      char* ret = directory_name_list_return_next(l);
      directory* child = directory_contain_children(d, ret);;

      if(child)
	{
	  char* last = l->list[l->size - 1];

	  if(!(strcmp(last, directory_name
		      (d->children[d->children_position - 1]))))
	    {
	      directory_delete_children(d, d->children_position - 1);
	    }
	  else
	    {
	      directory_delete_last_children_from_list
		(d->children[d->children_position - 1], l);
	    }
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
directory_edit_last_children_name_from_list(directory* d
					    ,directory_name_list* l
					    ,char* name)
{
  if(d && l)
    {
      directory_rewind(d);

      char* ret = directory_name_list_return_next(l);
      directory* child = directory_contain_children(d, ret);;

      if(child)
	{
	  char* last = l->list[l->size - 1];

	  if(!(strcmp(last, directory_name
		      (d->children[d->children_position - 1]))))
	    {
	      directory_edit_name(d->children[d->children_position - 1], name);
	    }
	  else
	    {
	      directory_edit_last_children_name_from_list
		(d->children[d->children_position - 1], l, name);
	    }
	}
    }
}

void
directory_edit_name_from_list(directory* d, char* name, char* new_name
			      ,directory_name_list* l)
{
  if(d && l)
    {
      directory_rewind(d);

      char* ret = directory_name_list_return_next(l);
      directory* child = directory_contain_children(d, ret);;

      if(child)
	{
	  if(!(strcmp(ret, name)))
	    {
	      directory_edit_name
		(d->children[d->children_position - 1], new_name);
	    }
	  else
	    {
	      directory_edit_name_from_list
		(d->children[d->children_position - 1], name, new_name, l);
	    }
	}
    }
}

void
directory_move_children(directory* receiver, directory* donator, char* name)
{
  if(receiver && donator && name)
    {
      directory* ret = NULL;

      directory_rewind(donator);
      directory_rewind(receiver);

      while((ret = directory_next_children(donator)))
	{
	  if(!(strcmp(name, (directory_name(ret)))))
	    {
	      directory_add_children(receiver, ret);
	      directory_delete_children(donator
					,donator->n_children);
	    }
	}
    }
}

void
directory_move_children_all(directory* receiver, directory* donator)
{
  if(receiver && donator)
    {
      directory* ret = NULL;

      directory_rewind(donator);
      directory_rewind(receiver);

      while((ret = directory_next_children(donator)))
	{
	  directory_add_children(receiver, ret);
	  directory_delete_children(donator
				    ,donator->n_children);
	}
    }
}

directory*
create_tree_from_bookmark_list(bookmark_list* bl, char* name)
{
  if(bl && name)
    {
      bookmark* b = NULL;
      directory* d = directory_new(name);

      while((b = bookmark_list_return_next_bookmark(bl)))
	{
	  directory_add_children_from_list(d, dismember(b), b);
	}

      return d;
    }

  return NULL;
}
