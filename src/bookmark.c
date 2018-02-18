#include "bookmark.h"

struct 
bookmark 
{
	char* id;
	char* name;
	char* url;
	char* comment;
	char* tag;
};

bookmark* 
bookmark_new(char* name, char* url, char* comment, char* tag) 
{
	bookmark* b = malloc(sizeof(bookmark));
	
	if(b) 
	{
		b->id	= NULL;

		if(name)
			b->name = name;
		else
			b->name	= NULL;

		if(url)
			b->url = url;
		else
			b->url = NULL;

		if(comment)
			b->comment = comment;
		else
			b->comment = "none";

		if(tag)
			b->tag = tag;
		else
			b->tag = "none";

		return b;
	} 
	else 
		return NULL;
}

void 
bookmark_destroy(bookmark* b) 
{
	if(b) 
	{
		free(b);
		b = NULL;
	}
}

char*
bookmark_id(bookmark* b)
{
	if(b && b->id)
		return b->id;
	else
		return NULL;
}

char*
bookmark_name(bookmark* b)
{
	if(b && b->name)
		return b->name;
	else
		return NULL;
}

char*
bookmark_url(bookmark* b)
{
	if(b && b->url)
		return b->url;
	else
		return NULL;
}

char*
bookmark_comment(bookmark* b)
{
	if(b && b->comment)
		return b->comment;
	else
		return NULL;
}

char*
bookmark_tag(bookmark* b)
{
	if(b && b->tag)
		return b->tag;
	else
		return NULL;
}

int
bookmark_set(bookmark* b, char* name, char* url, char* comment, char* tag) 
{
	if(b && name && url) 
	{
		if(!comment)
			comment = "None";
		
		if(!tag)
			tag = "None";

		b->name    = name;
		b->url     = url;
		b->comment = comment;
		b->tag     = tag;
	}
	else 
		return 1;

	return 0;
}

int
bookmark_set_id(bookmark* b, char* id)
{
	if(b && id)
	{
		b->id = id;
		return 0;
	}

	return 1;
}

int
bookmark_set_name(bookmark* b, char* name)
{
	if(b && name)
	{
		b->name = name;
		return 0;
	}

	return 1;
}

int
bookmark_set_url(bookmark* b, char* url)
{
	if(b && url)
	{
		b->url = url;
		return 0;
	}

	return 1;
}

int
bookmark_set_comment(bookmark* b, char* comment)
{
	if(b && comment)
	{
		b->comment = comment;
		return 0;
	}

	return 1;
}

int
bookmark_set_tag(bookmark* b, char* tag)
{
	if(b && tag)
	{
		b->tag = tag;
		return 0;
	}

	return 1;
}

