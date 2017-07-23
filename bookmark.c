#define _GNU_SOURCE 	/* asprintf, secure_getenv */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include "bookmark.h"

char* date(char* buffer, int bufsize) 
{
	const char* 	format = "%A %d/%m/%Y - %T";
	time_t 		t = time(NULL);
	struct tm* 	tmp;

	if(t) 
	{
		tmp = localtime(&t);

		if(tmp) 
		{
			strftime(buffer, bufsize, format, tmp);
			return buffer;
		}
	}

	return NULL;
}

struct 
bookmark 
{
	char* name;
	char* url;
	char* comment;
	char* tag;
};

struct 
bookmark_list 
{
	int 	size;
	int 	position;
	int 	next;
	char** 	id;
	char** 	name;
	char** 	url;
	char** 	comment;
	char** 	tag;
};

bookmark_list*
bookmark_list_create() 
{
	bookmark_list* l = calloc(1, sizeof(bookmark_list));

	if(l) 
	{
		l->position 	= 0;
		l->next 	= 0;
		l->size 	= 1;

		l->id 		= calloc(l->size, sizeof(char*));
		l->name 	= calloc(l->size, sizeof(char*));
		l->url 		= calloc(l->size, sizeof(char*));
		l->comment 	= calloc(l->size, sizeof(char*));
		l->tag 		= calloc(l->size, sizeof(char*));
		
		l->id[0] 	= NULL;
		l->name[0] 	= NULL;
		l->url[0] 	= NULL;
		l->comment[0] 	= NULL;
		l->tag[0] 	= NULL;

		return l;
	}

	return NULL;
}

void
bookmark_list_destroy(bookmark_list *l) 
{
	if(l) 
	{
		for(int i = 0; i < l->size - 1; ++i) 
		{
			if(l->id[i]) 
			{
				free(l->id[i]);
				l->id[i] = NULL;
			}

			if(l->name[i]) 
			{
				free(l->name[i]);
				l->name[i] = NULL;
			}

			if(l->url[i]) 
			{
				free(l->url[i]);
				l->url[i] = NULL;
			}

			if(l->comment[i]) 
			{
				free(l->comment[i]);
				l->comment[i] = NULL;
			}

			if(l->tag[i]) 
			{
				free(l->tag[i]);
				l->tag[i] = NULL;
			}
		}

		free(l->id);
		free(l->name);
		free(l->url);
		free(l->comment);
		free(l->tag);
		free(l);
	}
}

int
bookmark_list_enqueue(
	bookmark_list* l
	,char* id
	,char* name
	,char* url
	,char* comment
	,char* tag) 
{
	if(id) 
	{
		l->id[l->position] = calloc(strlen(id) + 1, sizeof(int));
		strncpy(l->id[l->position], id, strlen(id));
	}

	if(name) 
	{
		l->name[l->position] = calloc(strlen(name) + 1, sizeof(char));
		strncpy(l->name[l->position], name, strlen(name));
	}

	if(url) 
	{
		l->url[l->position] = calloc(strlen(url) + 1, sizeof(char));
		strncpy(l->url[l->position], url, strlen(url));
	}

	if(comment) 
	{
		l->comment[l->position] = calloc(strlen(comment) + 1, sizeof(char));
		strncpy(l->comment[l->position], comment, strlen(comment));
	}

	if(tag) 
	{
		l->tag[l->position] = calloc(strlen(tag) + 1, sizeof(char));
		strncpy(l->tag[l->position], tag, strlen(tag));
	}

	++l->position;

	if(l->position >= l->size - 1) 
	{
		++l->size;
		char** nid 	= realloc(l->id, l->size * sizeof(char*));
		char** nname 	= realloc(l->name, l->size * sizeof(char*));
		char** nurl 	= realloc(l->url, l->size * sizeof(char*));
		char** ncomment = realloc(l->comment, l->size * sizeof(char*));
		char** ntag 	= realloc(l->tag, l->size * sizeof(char*));

		if(nid && nname && nurl && ncomment && ntag) 
		{
			l->id 		= nid;
			l->name 	= nname;
			l->url 		= nurl;
			l->comment 	= ncomment;
			l->tag 		= ntag;

			l->id[l->size - 1] 	= NULL;
			l->name[l->size - 1] 	= NULL;
			l->url[l->size - 1] 	= NULL;
			l->comment[l->size - 1] = NULL;
			l->tag[l->size - 1] 	= NULL;
		}
		else 
			return 1;
	}

	return 0;
}

void 
bookmark_list_align(bookmark_list* l) 
{
	int c = 1;

	while(c < l->size) 
	{
		if(l->id[c - 1]) 
		{
			free(l->id[c -1]);
			l->id[c - 1] = NULL;
		}

		if(l->name[c - 1]) 
		{
			free(l->name[c -1]);
			l->name[c - 1] = NULL;
		}

		if(l->url[c - 1]) 
		{
			free(l->url[c -1]);
			l->url[c - 1] = NULL;
		}

		if(l->comment[c - 1]) 
		{
			free(l->comment[c -1]);
			l->comment[c - 1] = NULL;
		}

		if(l->tag[c - 1]) 
		{
			free(l->tag[c -1]);
			l->tag[c - 1] = NULL;
		}

		l->id[c - 1] 		= l->id[c];
		l->name[c - 1] 		= l->name[c];
		l->url[c - 1] 	 	= l->url[c];
		l->comment[c - 1] 	= l->comment[c];
		l->tag[c - 1] 		= l->tag[c];
		++c;
	}

	--l->size;
	--l->position;
}

char** 
bookmark_list_dequeue(bookmark_list* l) 
{
	if(l->size) 
	{
		char** ret = calloc(4, sizeof(char*));
		ret[0] = l->id[0];
		ret[1] = l->name[0];
		ret[2] = l->url[0];
		ret[3] = l->comment[0];
		ret[4] = l->tag[0];

		bookmark_list_align(l);

		if(l->size) 
		{
			char **nid 	= realloc(l->id, l->size * sizeof(char*));
			char **nname 	= realloc(l->name, l->size * sizeof(char*));
			char **nurl 	= realloc(l->url, l->size * sizeof(char*));
			char **ncomment = realloc(l->comment, l->size * sizeof(char*));
			char **ntag 	= realloc(l->tag, l->size * sizeof(char*));

			if(nid && nname && nurl && ncomment && ntag) 
			{
				l->id 		= nid;
				l->name 	= nname;
				l->url 		= nurl;
				l->comment 	= ncomment;
				l->tag 		= ntag;
			}
			else 
				return NULL;
		}
	
		return ret;
	}

	return NULL;
}

char**
bookmark_list_return_next(bookmark_list* l) 
{
	if(l->next < l->size) 
	{
		char** ret = calloc(4, sizeof(char*));
		ret[0] = l->id[l->next];
		ret[1] = l->name[l->next];
		ret[2] = l->url[l->next];
		ret[3] = l->comment[l->next];
		ret[4] = l->tag[l->next];

		l->next++;
		return ret;
	}
	else
		return NULL;
}

void
bookmark_list_rewind(bookmark_list* l) 
{
	l->next = 0;
}

int 
bookmark_list_get_size(bookmark_list* l) 
{
	if(l->size)
		return l->size;
	else
		return 0;
}

int
bookmark_list_get_position(bookmark_list* l) 
{
	if(l->position)
		return l->position;
	else
		return 0;
}

int
bookmark_list_get_next_position(bookmark_list* l) 
{
	if(l->next)
		return l->next;
	else
		return 0;
}

bookmark* 
bookmark_create() 
{
	bookmark* b = malloc(sizeof(bookmark));
	
	if(b) 
	{
		b->name      = NULL;
		b->url       = NULL;
		b->comment   = NULL;
		b->tag 	     = NULL;

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

int
bookmark_add(bookmark* b, char* name, char* url, char* comment, char* tag) 
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
bookmark_db_table_create(sqlite3* db) 
{
	if(db) 
	{
		char* 		err 	= 0;
		const char* 	table 	= "CREATE TABLE bookmark(id INTEGER PRIMARY KEY"
			", name text, url text, comment text, tag text, unique(url));";

		sqlite3_exec(db, table, 0, 0, &err);
		
		if(err) 
		{
			printf("%s\n", err);
			return 1;
		}
	} 
	else 
		return 1;

	return 0;
}

sqlite3*
bookmark_db_open(char *db_name) 
{
	sqlite3 *db = NULL;

	if(db_name) 
	{
		const int r = sqlite3_open(db_name, &db);

		if(r) 
			return NULL;

		else /* we going to verify if table bookmark exist on db */
		{
			int 		step;
			sqlite3_stmt* 	res;
			const char* 	sql = "SELECT name FROM sqlite_master WHERE"
				 	" name = 'bookmark' AND type = 'table'";

			int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
			
			if(rc == SQLITE_OK) 
			{
				step = sqlite3_step(res);
				sqlite3_finalize(res);

				/* the table do not exist, create */
				if(step != SQLITE_ROW) 
				{
					rc = bookmark_db_table_create(db);

					if(rc) 
					{
						printf("error creating table on database\n");
						return NULL;
					}
				}
			} 
			else 
			{
				sqlite3_finalize(res);
				return NULL;
			}
		}
	} 
	else 
		return NULL;
	
	return db;
}

void
bookmark_db_close(sqlite3* db) 
{
	if(db) 
	{
		sqlite3_close(db);
		db = NULL;
	}
}

int 
bookmark_db_write(bookmark* b, sqlite3* db) 
{
	if(b && db) 
	{
		const char* 	sql = "INSERT INTO bookmark (name, url, comment, tag) VALUES(?,?,?,?)";
		sqlite3_stmt* 	res;

		if((sqlite3_prepare_v2(db, sql, -1, &res, 0)) == SQLITE_OK) 
		{
			if(b->name)
				sqlite3_bind_text(res, 1, b->name, -1, NULL);

			if(b->url)
				sqlite3_bind_text(res, 2, b->url, -1, NULL);

			if(b->comment)
				sqlite3_bind_text(res, 3, b->comment, -1, NULL);

			if(b->tag)
				sqlite3_bind_text(res, 4, b->tag, -1, NULL);

			sqlite3_step(res);
		}

		sqlite3_finalize(res);
	}
	
	return 0;
}

int	/* import i_db bookmarks into db */
bookmark_db_import(sqlite3* db, sqlite3* i_db)
{
	if(db && i_db)
	{
		char** result;
		bookmark_list* bl = bookmark_db_query(i_db, 0, NULL);

		if(bl)
		{
			for(int i = 0; i < bookmark_list_get_size(bl) - 1; ++i)
			{
				bookmark* b = bookmark_create();
				result 	    = bookmark_list_return_next(bl);

				if(bookmark_add(b, result[1], result[2]
				,result[3], result[4]))
					return 1;

				if(bookmark_db_write(b, db))
					return 1;

				bookmark_destroy(b);
			}

			bookmark_list_destroy(bl);
		}
		else
			return 1;
	}
	else
		return 1;

	return 0;
}

int 	/* used to verify if an id exist on database */
bookmark_db_id(sqlite3* db, int id) 
{
	if((db && id) 
	&&(id < (INT_MAX - 1))
	&&(id > (INT_MIN)))
	{
		int 		ret = 0; 
		const char*	sql = "SELECT * FROM bookmark WHERE id = ?";
		sqlite3_stmt* 	res;

		if((sqlite3_prepare_v2(db, sql, -1, &res, 0)) == SQLITE_OK) 
		{
			sqlite3_bind_int(res, 1, id);

			if((sqlite3_step(res)) == SQLITE_ROW) 
				ret = sqlite3_column_int(res, 0);
		}

		sqlite3_finalize(res);
		return ret;	
	}

	return 0;	
}

int
bookmark_db_delete(sqlite3* db, int id) 
{
	if((db && id)
	&&(id < (INT_MAX - 1))
	&&(id > (INT_MIN)))
	{
		const char*	sql = "DELETE FROM bookmark WHERE id = ?";
		sqlite3_stmt*	res;

		if((sqlite3_prepare_v2(db, sql, -1, &res, 0)) == SQLITE_OK) 
		{
			if(bookmark_db_id(db, id)) 
			{
				sqlite3_bind_int(res, 1, id);
				sqlite3_step(res);
			} 
			else 
			{
				printf("invalid id for delete\n");
				return 1;
			}
		} 
		else 
			return 1;

		sqlite3_finalize(res);
		return 0;
	}

	return 1;
}

int
bookmark_db_delete_tag(sqlite3* db, char* tag, int greedy)
{
	if((db && tag) 
	&&(greedy < (INT_MAX - 1))
	&&(greedy > (INT_MIN)))
	{
		if(greedy)
		{
			char* sql_head = "DELETE FROM bookmark WHERE";

			char* sql = calloc(
				(strlen(sql_head) 
				+ strlen(" tag LIKE '%%%%'") 
				+ strlen(tag) + 1)
				, sizeof(char));

			if(sql) 
			{
				snprintf(sql, strlen(sql) - 1, "%s tag LIKE '%%%s%%'", sql_head, tag);
				sqlite3_stmt* 	res;

				if((sqlite3_prepare_v2(db, sql, -1, &res, 0)) == SQLITE_OK) 
					sqlite3_step(res);
				else 
					return 1;

				free(sql);
				sqlite3_finalize(res);
			}
		}
		else
		{
			char* sql = "DELETE FROM bookmark WHERE tag = ?";
			sqlite3_stmt* 	res;

			if((sqlite3_prepare_v2(db, sql, -1, &res, 0)) == SQLITE_OK) 
			{
				sqlite3_bind_text(res, 1, tag, -1, NULL);
				sqlite3_step(res);
			} 
			else 
				return 1;

			sqlite3_finalize(res);
		}

		return  0;
	} 

	return 1;
}

int 
bookmark_db_edit(sqlite3 *db, int id, int field, char *str) 
{
	if((db && id) 
	&&(id < (INT_MAX - 1))
	&&(id > (INT_MIN))
	&&(field < (INT_MAX - 1))
	&&(field > (INT_MIN)))
	{
		char* sql;

		switch(field) 
		{
			case 0:
				sql = "UPDATE bookmark SET name = ? WHERE id = ?";
				break;
			case 1:
				sql = "UPDATE bookmark SET url = ? WHERE id = ?";
				break;
			case 2:
				sql = "UPDATE bookmark SET comment = ? WHERE id = ?";
				break;
			case 3:
				sql = "UPDATE bookmark SET tag = ? WHERE id = ?";
				break;
			default:
				return 1;
		}

		int 		ret = 0; 
		sqlite3_stmt*	res;

		if((sqlite3_prepare_v2(db, sql, -1, &res, 0)) == SQLITE_OK) 
		{
			if(bookmark_db_id(db, id)) 
			{
				sqlite3_bind_text(res, 1, str, -1, NULL);
				sqlite3_bind_int(res, 2, id);
				sqlite3_step(res);
			}
			else 
			{
				printf("invalid id for edit\n");
				ret = 1;
			}
		}	
		else 
			ret = 1;

		sqlite3_finalize(res);
		return ret;
	}

	return 1;
}

int
bookmark_db_edit_bulk(sqlite3* db, int field, char* cv, char* nv) 
{
	if((db) 
	&&(field < (INT_MAX - 1))
	&&(field > (INT_MIN)))
	{
		char* 		sql;
		int 		ret = 0; 
		sqlite3_stmt*	res;

		switch(field) 
		{
			case 0:
				sql = "UPDATE bookmark SET name = ? WHERE name = ?";
				break;
			case 1:
				sql = "UPDATE bookmark SET url = ? WHERE url = ?";
				break;
			case 2:
				sql = "UPDATE bookmark SET comment = ? WHERE comment = ?";
				break;
			case 3:
				sql = "UPDATE bookmark SET tag = ? WHERE tag = ?";
				break;
			default:
				return 1;
		}

		if((sqlite3_prepare_v2(db, sql, -1, &res, 0)) == SQLITE_OK) 
		{
			sqlite3_bind_text(res, 1, nv, -1, NULL);
			sqlite3_bind_text(res, 2, cv, -1, NULL);
			sqlite3_step(res);
		}	
		else 
			ret = 1;

		sqlite3_finalize(res);
		return ret;
	}
	
	return 1;
}

bookmark_list*
search_db(sqlite3* db, char* field, char* str, char* sql) 
{
	if(sql && db) 
	{
		sqlite3_stmt* 	res;
		bookmark_list* 	bl = bookmark_list_create();

		if((sqlite3_prepare_v2(db, sql, -1, &res, 0)) == SQLITE_OK) 
		{
			if(str) 
				sqlite3_bind_text(res, 1, str, -1, NULL);

			while((sqlite3_step(res)) == SQLITE_ROW) 
			{
				if(field) 
				{
					if(!strncmp(field, ID, strlen(ID))) 
					{
						if(sqlite3_column_text(res, 0)) 
							bookmark_list_enqueue(bl
								,(char*)sqlite3_column_text(res, 0)
								,NULL
								,NULL
								,NULL
								,NULL);
					}
						
					if(!strncmp(field, NAME, strlen(NAME))) 
					{
						if(sqlite3_column_text(res, 1)) 
							bookmark_list_enqueue(bl
								,NULL
								,(char*)sqlite3_column_text(res, 1)
								,NULL
								,NULL
								,NULL);
					}

					if(!strncmp(field, URL, strlen(URL))) 
					{
						if(sqlite3_column_text(res, 2))
							bookmark_list_enqueue(bl
								,NULL
								,NULL
								,(char*)sqlite3_column_text(res, 2)
								,NULL
								,NULL);
					}

					if(!strncmp(field, COMMENT, strlen(COMMENT))) 
					{
						if(sqlite3_column_text(res, 3))
							bookmark_list_enqueue(bl
								,NULL
								,NULL
								,NULL
								,(char*)sqlite3_column_text(res, 3)
								,NULL);
					}

					if(!strncmp(field, TAG, strlen(TAG))) 
					{
						if(sqlite3_column_text(res, 4)) 
							bookmark_list_enqueue(bl
								,NULL
								,NULL
								,NULL
								,NULL
								,(char*)sqlite3_column_text(res, 4));
					}
				}
				else 
					bookmark_list_enqueue(bl
						,(char*)sqlite3_column_text(res, 0)
						,(char*)sqlite3_column_text(res, 1)
						,(char*)sqlite3_column_text(res, 2)
						,(char*)sqlite3_column_text(res, 3)
						,(char*)sqlite3_column_text(res, 4));
			}
		} 
		else 
			return NULL;

		sqlite3_finalize(res);

		if(bl->position)
			return bl;
		else
			return NULL;
	}

	return NULL;
}

bookmark_list* 
bookmark_db_query(sqlite3* db, int id, char* field) 
{
	if(db) 
	{
		bookmark_list* 	bl;
		char* 		sql = "SELECT * FROM bookmark ORDER BY tag ASC";

		if((id) 
		&&(id < (INT_MAX - 1))
		&&(id > (INT_MIN)))
		{
			sql 	= "SELECT * FROM bookmark WHERE id = ?";
			char*	s_id = NULL;
			asprintf(&s_id, "%i", id); /*XXX*/

			bl = search_db(db, field, s_id, sql);
			free(s_id);
		} 
		else
			bl = search_db(db, field, NULL, sql);
		
		if(bl)
			return bl;

		return NULL;
	}
	
	return NULL;
}

bookmark_list*
bookmark_db_search(sqlite3* db, char* field, char* str) 
{
	if(str && db) 
	{
		bookmark_list* 	bl 	= NULL;
		const char* 	sql 	= "SELECT * FROM bookmark WHERE";

		if(field && !strncmp(field, NAME, strlen(NAME))) 
		{
			char* search_name = calloc(
				(strlen(sql) 
				+ strlen(" name LIKE '%%%%'") 
				+ strlen(str) + 1)
				, sizeof(char));

			if(search_name) 
			{
				snprintf(search_name, strlen(search_name) - 1
					,"%s name LIKE '%%%s%%'", sql, str);

				bl = search_db(db, NULL, str, search_name);
				free(search_name);
				search_name = NULL;
			}
			else 
			{
				printf("error allocating memory\n");
				return NULL;
			}
		}
		else if(field && !strncmp(field, URL, strlen(URL)))  
		{
			char* search_url = calloc(
				(strlen(sql) 
				+ strlen(" url LIKE '%%%%'") 
				+ strlen(str) + 1)
				, sizeof(char));

			if(search_url) 
			{
				snprintf(search_url, strlen(search_url) - 1
					,"%s url LIKE '%%%s%%'", sql, str);

				bl = search_db(db, NULL, str, search_url);
				free(search_url);
				search_url = NULL;
			}
			else 
			{
				printf("error allocating memory\n");
				return NULL;
			}	
		}
		else if(field && !strncmp(field, COMMENT, strlen(COMMENT))) 
		{
			char* search_comment = calloc(
				(strlen(sql) 
				+ strlen(" comment LIKE '%%%%'") 
				+ strlen(str) + 1)
				, sizeof(char));

			if(search_comment) 
			{
				snprintf(search_comment, strlen(search_comment) - 1
					,"%s comment LIKE '%%%s%%'", sql, str);

				bl = search_db(db, NULL, str, search_comment);
				free(search_comment);
				search_comment = NULL;
			}
			else 
			{
				printf("error allocating memory\n");
				return NULL;
			}
		}
		else if(field && !strncmp(field, TAG, strlen(TAG))) 
		{
			char* search_tag = calloc(
				(strlen(sql) 
				+ strlen(" tag LIKE '%%%%'") 
				+ strlen(str) + 1)
				, sizeof(char));

			if(search_tag) 
			{
				snprintf(search_tag, strlen(search_tag) - 1
					,"%s tag LIKE '%%%s%%'", sql, str);

				bl = search_db(db, NULL, str, search_tag);
				free(search_tag);
				search_tag = NULL;
			}
			else 
			{
				printf("error allocating memory\n");
				return NULL;
			}
		}
		else 
		{ 
			char* search = calloc(
				(strlen(sql) 
				+ strlen(" name LIKE '%%%s%%' "
					"OR url LIKE '%%%s%%' "
					"OR comment LIKE '%%%s%%' "
					"OR tag LIKE '%%%s%%'") 
				+ strlen(str) * 4 + 1)
				, sizeof(char));

			if(search) 
			{
				snprintf(search, strlen(search) - 1
					,"%s  name LIKE '%%%s%%' "
					"OR url LIKE '%%%s%%' "
					"OR comment LIKE '%%%s%%' "
					"OR tag LIKE '%%%s%%'"
					,sql, str, str, str, str);

				bl = search_db(db, NULL, str, search);
				free(search);
				search = NULL;
			}
			else 
			{
				printf("error allocating memory\n");
				return NULL;
			}
		}

		return bl;
	} 
	else 
		return NULL;
}

