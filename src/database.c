#include "database.h"

char* date(char* buffer, int bufsize) 
{
  const char* format = "%A %d/%m/%Y - %T";
  time_t t = time(NULL);
  struct tm* tmp;

  if(t) 
    {
      tmp = localtime(&t);

      if(tmp) 
	{
	  strftime(buffer, bufsize, format, tmp);
	  return buffer;
	}
    }

  free(tmp);
  return NULL;
}

int 
bookmark_db_table_create(sqlite3* db) 
{
  if(db) 
    {
      char* err = 0;
      const char* table = "CREATE TABLE bookmark(id INTEGER PRIMARY KEY"
	",name text, url text, comment text, tag text, favicon text"
	",unique(url));";

      sqlite3_exec(db, table, 0, 0, &err);
		
      if(err) 
	{
	  printf("%s\n", err);
	  return 1;
	}
    } 
  else
    {
      return 1;
    }

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
	{
	  return NULL;
	}

      else /* verify if table bookmark exist on db */
	{
	  int step;
	  sqlite3_stmt* res;
	  const char* sql = "SELECT name FROM sqlite_master WHERE"
	    " name = 'bookmark' AND type = 'table'";

	  int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
			
	  if(rc == SQLITE_OK) 
	    {
	      step = sqlite3_step(res);
	      sqlite3_finalize(res);

	      /* if not, create */
	      if(step != SQLITE_ROW) 
		{
		  rc = bookmark_db_table_create(db);

		  if(rc) 
		    {
		      printf("error creating table on"
			     " database\n");
		      return NULL;
		    }
		}
	      else
		{
		  /* verify if column favicon exist in bookmark table */
		  sql = "SELECT * from bookmark";
		  rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
		  step = sqlite3_step(res);

		  const char* favicon_column = sqlite3_column_name(res, 5);
		  sqlite3_finalize(res);
				
		  if(!favicon_column)
		    {
		      sql = "ALTER TABLE bookmark ADD COLUMN favicon text";
		      rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
		      step = sqlite3_step(res);

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
    {
      return NULL;
    }
	
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
      const char* sql = "INSERT INTO bookmark (name, url, comment, tag, favicon)"
	" VALUES(?,?,?,?,?)";

      sqlite3_stmt* res;

      if((sqlite3_prepare_v2(db, sql, -1, &res, 0)) == SQLITE_OK) 
	{
	  if(bookmark_name(b))
	    {
	      sqlite3_bind_text(res, 1, bookmark_name(b), -1, NULL);
	    }

	  if(bookmark_url(b))
	    {
	      sqlite3_bind_text(res, 2, bookmark_url(b), -1, NULL);
	    }

	  if(bookmark_comment(b))
	    {
	      sqlite3_bind_text(res, 3, bookmark_comment(b), -1, NULL);
	    }

	  if(bookmark_tag(b))
	    {
	      sqlite3_bind_text(res, 4, bookmark_tag(b), -1, NULL);
	    }

	  if(bookmark_favicon(b))
	    {
	      sqlite3_bind_text(res, 5, bookmark_favicon(b), -1, NULL);
	    }

	  sqlite3_step(res);
	}

      sqlite3_finalize(res);
    }
	
  return 0;
}

int /* import i_db bookmarks into db */
bookmark_db_import(sqlite3* db, sqlite3* i_db)
{
  if(db && i_db)
    {
      bookmark_list* bl = bookmark_db_query(i_db, 0, NULL);

      if(bl)
	{
	  bookmark* b = NULL;

	  while((b = bookmark_list_return_next_bookmark(bl)))
	    {
	      if(bookmark_db_write(b, db))
		{
		  return 1;
		}

	      bookmark_destroy(b);
	    }

	  bookmark_list_destroy(bl);
	}
      else
	{
	  return 1;
	}
    }
  else
    {
      return 1;
    }

  return 0;
}

static int /* used to verify if an id exist on database */
bookmark_db_id(sqlite3* db, int id) 
{
  if((db && id) 
     &&(id < (INT_MAX - 1))
     &&(id > (INT_MIN)))
    {
      int ret = 0; 
      const char* sql = "SELECT * FROM bookmark WHERE id = ?";
      sqlite3_stmt* res;

      if((sqlite3_prepare_v2(db, sql, -1, &res, 0)) == SQLITE_OK) 
	{
	  sqlite3_bind_int(res, 1, id);

	  if((sqlite3_step(res)) == SQLITE_ROW)
	    {
	      ret = sqlite3_column_int(res, 0);
	    }
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
      const char* sql = "DELETE FROM bookmark WHERE id = ?";
      sqlite3_stmt* res;

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
	{
	  return 1;
	}

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

	  char* sql = calloc((strlen(sql_head) 
			      + strlen(" tag LIKE '%%%%'") 
			      + strlen(tag) + 1)
			     ,sizeof(char));

	  if(sql) 
	    {
	      snprintf(sql, strlen(sql) - 1, "%s tag LIKE '%%%s%%'"
		       ,sql_head, tag);
	      sqlite3_stmt* res;

	      if((sqlite3_prepare_v2(db, sql, -1, &res, 0)) == SQLITE_OK)
		{
		  sqlite3_step(res);
		}
	      else
		{
		  return 1;
		}

	      free(sql);
	      sqlite3_finalize(res);
	    }
	}
      else
	{
	  char* sql = "DELETE FROM bookmark WHERE tag = ?";
	  sqlite3_stmt* res;

	  if((sqlite3_prepare_v2(db, sql, -1, &res, 0)) == SQLITE_OK) 
	    {
	      sqlite3_bind_text(res, 1, tag, -1, NULL);
	      sqlite3_step(res);
	    } 
	  else
	    {
	      return 1;
	    }

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
	case 4:
	  sql = "UPDATE bookmark SET favicon = ? WHERE id = ?";
	  break;
	default:
	  return 1;
	}

      int ret = 0; 
      sqlite3_stmt* res;

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
	{
	  ret = 1;
	}

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
      char* sql;
      int ret = 0; 
      sqlite3_stmt* res;

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
	{
	  ret = 1;
	}

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
      sqlite3_stmt* res;
      bookmark_list* bl = bookmark_list_new();

      if((sqlite3_prepare_v2(db, sql, -1, &res, 0)) == SQLITE_OK) 
	{
	  if(str)
	    {
	      sqlite3_bind_text(res, 1, str, -1, NULL);
	    }

	  while((sqlite3_step(res)) == SQLITE_ROW) 
	    {
	      if(field) 
		{
		  char* queue_args[6];

		  queue_args[0] = NULL;
		  queue_args[1] = NULL;
		  queue_args[2] = NULL;
		  queue_args[3] = NULL;
		  queue_args[4] = NULL;
		  queue_args[5] = NULL;			  		  
		  
		  if(!strncmp(field, ID, strlen(ID))) 
		    {
		      if(sqlite3_column_text(res, 0))
			{
			  queue_args[0] = (char*)sqlite3_column_text(res, 0);
			}
		    }
						
		  if(!strncmp(field, NAME, strlen(NAME))) 
		    {
		      if(sqlite3_column_text(res, 1))
			{
			  queue_args[1] = (char*)sqlite3_column_text(res, 1);
			}
		    }

		  if(!strncmp(field, URL, strlen(URL))) 
		    {
		      if(sqlite3_column_text(res, 2))
			{
			  queue_args[2] = (char*)sqlite3_column_text(res, 2);
			}
		    }

		  if(!strncmp(field, COMMENT, strlen(COMMENT))) 
		    {
		      if(sqlite3_column_text(res, 3))
			{
			  queue_args[3] = (char*)sqlite3_column_text(res, 3);
			}
		    }

		  if(!strncmp(field, TAG, strlen(TAG))) 
		    {
		      if(sqlite3_column_text(res, 4))
			{
			  queue_args[4] = (char*)sqlite3_column_text(res, 4);
			}
		    }

		  bookmark_list_enqueue
		    (bl
		     ,queue_args[0]
		     ,queue_args[1]
		     ,queue_args[2]
		     ,queue_args[3]
		     ,queue_args[4]
		     ,queue_args[5]);
		}
	      else
		{
		  bookmark_list_enqueue
		    (bl
		     ,(char*)sqlite3_column_text(res, 0)
		     ,(char*)sqlite3_column_text(res, 1)
		     ,(char*)sqlite3_column_text(res, 2)
		     ,(char*)sqlite3_column_text(res, 3)
		     ,(char*)sqlite3_column_text(res, 4)
		     ,(char*)sqlite3_column_text(res, 5));
		}
	    }
	} 
      else
	{
	  return NULL;
	}

      sqlite3_finalize(res);

      if(bookmark_list_get_position(bl))
	{
	  return bl;
	}
      else
	{
	  return NULL;
	}
    }

  return NULL;
}

bookmark_list* 
bookmark_db_query(sqlite3* db, int id, char* field) 
{
  if(db) 
    {
      bookmark_list* bl;
      char* sql = "SELECT * FROM bookmark ORDER BY tag ASC";

      if((id) 
	 &&(id < (INT_MAX - 1))
	 &&(id > (INT_MIN)))
	{
	  sql = "SELECT * FROM bookmark WHERE id = ?";
	  char*	s_id = NULL;
	  asprintf(&s_id, "%i", id); /*XXX*/

	  bl = search_db(db, field, s_id, sql);
	  free(s_id);
	} 
      else
	{
	  bl = search_db(db, field, NULL, sql);
	}
		
      if(bl)
	{
	  return bl;
	}

      return NULL;
    }
	
  return NULL;
}

bookmark_list*
bookmark_db_search(sqlite3* db, char* field, char* str) 
{
  if(str && db) 
    {
      bookmark_list* bl = NULL;
      const char* sql = "SELECT * FROM bookmark WHERE";

      if(field && !strncmp(field, NAME, strlen(NAME))) 
	{
	  char* search_name = calloc((strlen(sql) 
				      + strlen(" name LIKE '%%%%'") 
				      + strlen(str) + 1)
				     ,sizeof(char));

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
	  char* search_url = calloc((strlen(sql) 
				     + strlen(" url LIKE '%%%%'") 
				     + strlen(str) + 1)
				    ,sizeof(char));

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
	  char* search_comment = calloc((strlen(sql) 
					 + strlen(" comment LIKE '%%%%'") 
					 + strlen(str) + 1)
					,sizeof(char));

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
	  char* search_tag = calloc((strlen(sql) 
				     + strlen(" tag LIKE '%%%%'") 
				     + strlen(str) + 1)
				    ,sizeof(char));

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
	  char* search = calloc((strlen(sql) 
				 + strlen(" name LIKE '%%%s%%' "
					  "OR url LIKE '%%%s%%' "
					  "OR comment LIKE '%%%s%%' "
					  "OR tag LIKE '%%%s%%'") 
				 + strlen(str) * 4 + 1)
				,sizeof(char));

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
    {
      return NULL;
    }
}
