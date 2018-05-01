#include "html.h"

/* -- encoded gnome icons with base64 -- */
/* gnome/16x16/status/starred.png */
char* html_starred_icon =
  "iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABHNCSVQICAgIfAhkiAAAAAlwSFlz"
  "AAAN1wAADdcBQiibeAAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAAAUdEVY"
  "dEF1dGhvcgBKYWt1YiBTdGVpbmVy5vv3LwAAAm9JREFUOI2VUktIVGEYPfe/81BTRxtHTWfCwNJs"
  "YT4xIje5yVoaZY9FkF5oUwQtq52r2riIfmiRhWFQGFgJtRBCTRClkczUsKFpZnRmnOedude59/5/"
  "q0Bh7sKz/L5zDpzvfAKlFGa4fELqAoCxZTpnxrGYqgEIBPfAwQH0mXGI2eLKcclFbO7zgqX0Qv8x"
  "qWLfBgYwYKt7aBFrrlmYFTf3ZTAYkkhmh9yxOHpFa/lFMaOQu4ODkpCPK6yOo5MTVIPDyQEnIagB"
  "0FjZdP9s2ZHbNl1ZQMr/StteH5niAtZgIMA5IhARZcCWMD+GcwLH2xJXi6264SopKK4TiLUcor0J"
  "MH4jG3mMwsohsJwPTE9ASft4eO01S0UWc5yhjywm6WRWRlc0+D0ux/3QmQNqcgp6bAiG6oVI7NBi"
  "jyCH30NRRKRjIURCy3Eli1OLMv1IAGCF0aVoUmtemh5eD60+NzStCmHfKBgcEAp7seWbhMZqEfVP"
  "6D++Dv+KyTsnf+jUu+eImzYajGR469Lc6BqxJlFS4YEankD87wd4Gi+h1OmBd+bFhppibUFCA3lb"
  "UEppJqvCbhFiAKtHLqMindAQ2wrCZgeUHRRtltG0eY0bkoNbit1ggGHUIJpQ9JSc0GX5IJR0AEUO"
  "j4utS5W7JXteeVvB6aNuh7HtX+CfPnvVZDzxlAvgB4pmb53pbrFX1br4z5U/3U7gTV4DVUXP3Gy4"
  "YHo68E3TcN3dTpcBwD8vPRt/N/NSJLZWTUcPdhnsiWAwNGRVbeDBE7T9FwOAp4OuVDXT9qySu6Hp"
  "qDeNYFXQf6ibpmgH8uJwJx0JfZHGd8/+AZMBHx1uqe9MAAAAAElFTkSuQmCC";

/* gnome/16x16/places/folder.png */
char* html_folder_icon =
  "iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABHNCSVQICAgIfAhkiAAAAAlwSFlz"
  "AAAN1wAADdcBQiibeAAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAAAOdEVY"
  "dFRpdGxlAEZvbGRlcnMhVKpgQQAAABd0RVh0QXV0aG9yAExhcG8gQ2FsYW1hbmRyZWnfkRoqAAAA"
  "KXRFWHREZXNjcmlwdGlvbgBCYXNlZCBvZiBKYWt1YiBTdGVpbmVyIGRlc2lnbqCEBXMAAAGuSURB"
  "VDiNpZG/alRBFMZ/M3v3T0xioltsCguFSCIiaBmICGJjIWhaO4s0LvgAgr0PoM8gKBJfQEUbURAj"
  "KirEwhVUNCxmvffOnZl7jsUmq0U2cPGDYeCcMz++M59RVf5Hpnvx2DWsvQmm+W9DVfsqrN5ae3Nv"
  "T8DVS8e3zq2sTteSOlKWiAxPng548XgtC0W+dPvB+9fjAIkxTIgKH9efjYoNUzDX3GRpcXIfTK7f"
  "ubG86+NBrg8TiyH7vTUq1kzgYPKTo6evMNOZH2s9+oznd6+fTdozDeN7j2iXDoB6a5rF5cu09nf4"
  "1vswFtA5NE+IQtI+0KydOt9lanYOABUhHfTp/+ixV0IxOHwUkjJEGhOzfPn0duzwbvLOEaKS+CiE"
  "IqeMoRIgFPnQQYiCdxkx+EqAwqXDPwhRKFxOGao58Hk2BPigFC4lVlzB5Sk+6M4KjrKMGMywa3bG"
  "DLCdxCgQRYHCbTuIiuR5ZhutKayxGFvD2r+3qqJSIiKIlKgKIsKvze9ERRKR2pONdy/PHF44Yaig"
  "r583VMU+Nd0LC0dss3k/BH+yCqBeb7ySolj5A0Ys6Y3vGnPVAAAAAElFTkSuQmCC";

/* build the favicon tag */
char*
favicon_tag(bookmark* b)
{
  char* favicon = NULL;
  char* favicon_img = NULL;
  char* favicon_string = "<image src='data:image/png;base64,";
  
  if(strcmp(bookmark_favicon(b), "none"))
    {
      favicon = bookmark_favicon(b);
    }
  else
    {
      favicon = html_starred_icon;      
    }

  int size = strlen(favicon_string) + strlen(favicon) + 3;
  favicon_img = malloc(size);
  snprintf(favicon_img, size, "%s%s'>", favicon_string, favicon);
  
  return favicon_img;
}

int
html_tree_table_row(directory* d, int depth, FILE* fp)
{
  if(d)
    {
      bookmark* b = NULL;
      char* open_ul = "<ul>\n";
      char* close_ul = "</ul>\n";
      char* span_dir_name = "<span style='color:#056B24;'>%s</span>\n";
      char* link_open = "<li>%s<a href=\'%s\'>%s";
      char* link_close = "</a></li>\n";
      char* comment = " - %s";
      char* favicon_img = NULL;
      
      if(fp)
	{
	  for(int i = 0; i <= depth; ++i)
	    {
	      fprintf(fp, open_ul);
	    }

	  fprintf(fp, "<img src=data:image/png;base64,%s>", html_folder_icon);
	  fprintf(fp, span_dir_name, directory_name(d));
	  fprintf(fp, open_ul);

	  while((b = directory_next_bookmark(d)))
	    {
	      favicon_img = favicon_tag(b);
	      
	      fprintf(fp, link_open
		      ,favicon_img, bookmark_url(b), bookmark_name(b));

	      if(strcmp(bookmark_comment(b), "none"))
		{
		  fprintf(fp, comment, bookmark_comment(b));
		}

	      fprintf(fp, link_close);
	      free(favicon_img);
	    }

	  for(int i = 0; i <= depth + 2; ++i)
	    {
	      fprintf(fp, close_ul);
	    }

	  free(b);
	  return ++depth;
	}
      else
	{
	  for(int i = 0; i <= depth; ++i)
	    {
	      printf(open_ul);
	    }

	  printf("<img src=data:image/png;base64,%s>", html_folder_icon);
	  printf(span_dir_name, directory_name(d));
	  printf(open_ul);

	  while((b = directory_next_bookmark(d)))
	    {
	      favicon_img = favicon_tag(b);
	      
	      printf(link_open
		     ,favicon_img, bookmark_url(b), bookmark_name(b));

	      if(strcmp(bookmark_comment(b), "none"))
		{
		  printf(comment, bookmark_comment(b));
		}

	      printf(link_close);
	      free(favicon_img);
	    }

	  for(int i = 0; i <= depth + 2; ++i)
	    {
	      printf(close_ul);
	    }

	  free(b);
	  return ++depth;
	}
    }

  return 0;
}

void
html_tree_branch(directory* d, int depth, FILE* fp)
{
  if(d)
    {
      directory_rewind(d);

      directory* ret = NULL;
      depth = html_tree_table_row(d, depth, fp);

      while((ret = directory_next_children(d)))
	{
	  directory_rewind(ret);
	  html_tree_table_row(ret, depth, fp);
	  html_tree_branch(directory_next_children(ret), depth + 1, fp);
	}

      free(ret);
    }
}

int
bookmark_html_tree(bookmark_list* bl, FILE* fp) 
{
  if(bl) 
    {
      bookmark_list* bl = bookmark_db_query(g_db, 0, NULL);
      directory* root = create_tree_from_bookmark_list(bl, "root");	
      directory* child = NULL;
      directory* ret = NULL;
      char buf[64]; 

      char* page_top = "<!DOCTYPE html>\n"
	"<html>\n"
	"<head>\n"
	"<meta charset=\'UTF-8\'>\n"
	"<style>\n"
	"html, body, table { font-size: 12px; }\n"
	"h3 { margin: 0; border: 0; color: steelblue; }\n"
	"</style>\n"
	"</head>\n"
	"<body bgcolor=mintcream>\n"
	"<h3>Bookmarks:</h3>\n"
	"<div style='border:1px solid green;"
	"margin-bottom: 10px;"
	"margin-top: 10px;"
	"border-radius:3px;"
	"background-color:lavender;"
	"padding:4px;'>\n";

      char* page_bottom = "</div>\n<br />\nGenerated in: "
	"<span style='color:green;'>%s</span>\n"
	"</body>\n</html>\n";

      directory_rewind(root);

      if(fp)
	{
	  fprintf(fp, page_top);
	}
      else
	{
	  printf(page_top);
	}

      while((child = directory_next_children(root)))
	{
	  directory_rewind(child);
	  html_tree_table_row(child, 0, fp);

	  while((ret = directory_next_children(child)))
	    {
	      html_tree_branch(ret, 1, fp);
	    }
	}

      if(fp)
	{
	  fprintf(fp, page_bottom ,date(buf, 64));
	}
      else
	{
	  printf(page_bottom ,date(buf, 64));
	}

      directory_destroy(ret);
      directory_destroy(child);
      directory_destroy(root);
      bookmark_list_destroy(bl);

      return 0;
    }

  return 1;
}
