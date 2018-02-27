#include "html.h"

void
print_folder_icon(int* canvas, FILE* fp)
{
  char* icon = "<canvas id='canvas_%d' width='20' height='14'></canvas>\n"
    "<script>\n"
    "var ctx = document.getElementById(\"canvas_%d\").getContext(\"2d\");\n"
    "// #layer1\n"
    "ctx.save();\n"
    "ctx.transform(1.000000, 0.000000, 0.000000, 1.000000, -33.095309"
    ", -103.342050);\n"
    "// #path3684\n"
    "ctx.beginPath();\n"
    "ctx.lineJoin = 'miter';\n"
    "ctx.strokeStyle = 'rgb(0, 0, 0)';\n"
    "ctx.lineCap = 'butt';\n"
    "ctx.lineWidth = 0.010153;\n"
    "ctx.fillStyle = 'rgb(233, 198, 175)';\n"
    "ctx.moveTo(33.118982, 103.594940);\n"
    "ctx.lineTo(36.208020, 117.952770);\n"
    "ctx.lineTo(51.344308, 117.952770);\n"
    "ctx.lineTo(47.946367, 103.361480);\n"
    "ctx.fill();\n"
    "ctx.stroke();\n"
    "// #path3686\n"
    "ctx.beginPath();\n"
    "ctx.lineJoin = 'miter';\n"
    "ctx.strokeStyle = 'rgb(0, 0, 0)';\n"
    "ctx.lineCap = 'butt';\n"
    "ctx.lineWidth = 0.010153;\n"
    "ctx.fillStyle = 'rgb(211, 141, 95)';\n"
    "ctx.moveTo(48.667141, 106.163010);\n"
    "ctx.lineTo(53.815540, 106.046280);\n"
    "ctx.lineTo(51.344308, 117.952770);\n"
    "ctx.fill();\n"
    "ctx.stroke();\n"
    "ctx.restore();\n"
    "</script>\n";

  if(fp)
    {
      fprintf(fp, icon, *canvas, *canvas);
    }
  else
    {
      printf(icon, *canvas, *canvas);
    }

  ++(*canvas);
}

int
html_tree_table_row(directory* d, int depth, int* canvas, FILE* fp)
{
  if(d)
    {
      bookmark* b = NULL;
      char* open_ul = "<ul>\n";
      char* close_ul = "</ul>\n";
      char* span_dir_name = "<span style='color:#056B24;'>%s</span>\n";
      char* link_open = "<li><a href=\'%s\'>%s";
      char* link_close = "</a></li>\n";
      char* comment = " - %s";
 
      if(fp)
	{
	  for(int i = 0; i <= depth; ++i)
	    {
	      fprintf(fp, open_ul);
	    }

	  print_folder_icon(canvas, fp);
	  fprintf(fp, span_dir_name, directory_name(d));
	  fprintf(fp, open_ul);

	  while((b = directory_next_bookmark(d)))
	    {
	      fprintf(fp, link_open
		      ,bookmark_url(b), bookmark_name(b));

	      if(strcmp(bookmark_comment(b), "none"))
		{
		  fprintf(fp, comment, bookmark_comment(b));
		}

	      fprintf(fp, link_close);
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

	  print_folder_icon(canvas, fp);
	  printf(span_dir_name, directory_name(d));
	  printf(open_ul);

	  while((b = directory_next_bookmark(d)))
	    {
	      printf(link_open
		     ,bookmark_url(b), bookmark_name(b));

	      if(strcmp(bookmark_comment(b), "none"))
		{
		  printf(comment, bookmark_comment(b));
		}

	      printf(link_close);
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
html_tree_branch(directory* d, int depth, int* canvas, FILE* fp)
{
  if(d)
    {
      directory_rewind(d);

      directory* ret = NULL;
      depth = html_tree_table_row(d, depth, canvas, fp);

      while((ret = directory_next_children(d)))
	{
	  directory_rewind(ret);
	  html_tree_table_row(ret, depth, canvas, fp);
	  html_tree_branch(directory_next_children(ret), depth + 1, canvas
			   ,fp);
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
      int canvas	= 0;
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

      //html_tree_table_row(root, 0, &canvas);

      while((child = directory_next_children(root)))
	{
	  directory_rewind(child);
	  html_tree_table_row(child, 0, &canvas, fp);

	  while((ret = directory_next_children(child)))
	    {
	      html_tree_branch(ret, 1, &canvas, fp);
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
