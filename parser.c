#include "parser.h"

char* red 	= "\033[31m";
char* green 	= "\033[32m";
char* yellow 	= "\033[33m";
char* blue 	= "\033[34m";
char* magenta 	= "\033[35m";
char* cyan 	= "\033[4;36m";
char* gray 	= "\033[37m";
char* white 	= "\033[38m";
char* reset 	= "\033[0m";

char* id_color;
char* name_color;
char* url_color;
char* comment_color;
char* tag_color;

int color 	= 0;
int verbose	= 0;
sqlite3* db 	= NULL;

void 
help() 
{
	printf("Usage:\n");
	printf("\t-a | --add \n"
		"\t\t[n]ame\t\t= name of the bookmark\n"
		"\t\t[u]rl\t\t= url of the bookmark\n"
		"\t\t[c]omment\t= comment on bookmark\n"
		"\t\t[t]ag\t\t= tag on bookmark\n\n"
		"\t\texample: pmb -a n=test,u=http://test.com\n\n");

	printf("\t-d | --delete\n"
		"\t\t[i]d\t\t= delete bookmark 'id'\n"
		"\t\t[t]ag\t\t= delete bookmarks tagged with tag\n"
		"\t\t[g]reedy\t= make tag deletion greedy\n\n"
		"\t\texample: pmb -d id=10\n"
		"\t\t         pmb -d tag=windows,g\n\n");

	printf("\t-e | --edit\n"
		"\t\t[i]d\t\t= id of bookmark to edit\n"
		"\t\t[n]ame\t\t= name of bookmark to edit\n"
		"\t\t[u]rl\t\t= url of bookmark to edit\n"
		"\t\t[c]omment\t= comment of bookmark to edit\n"
		"\t\t[t]ag\t\t= tag of bookmark to edit\n\n"
		"\t\t[f]ield\t\t= field to edit[all bookmarkks]\n"
		"\t\t[v]alue\t\t= current value of selected field\n"
		"\t\tne[w]-value\t= new value to selected field\n\n"
		"\t\texample: pmb -e id=10,tag=new_tag\n"
		"\t\t         pmb -e field=tag,value=linux,new-value=computer/linux\n\n");

	printf("\t-p | --print\n"
		"\t\t[i]d\t\t= id of bookmark to print\n"
		"\t\t[f]ield\t\t= name, url, comment, tag\n"
		"\t\t[a]ll\t\t= print all\n\n"
		"\t\texample: pmb -p all\n"
		"\t\t         pmb -p field=url\n\n");

	printf("\t-s | --search\n"
		"\t\t[n]ame\t\t= name to search in bookmark database\n"
		"\t\t[u]rl\t\t= url to search in bookmark database\n"
		"\t\t[c]omment\t= comment to search in bookmark database\n"
		"\t\t[t]ag\t\t= tag to search in bookmark database\n"
		"\t\t[e]xport\t= export result to [db]\n"
		"\t\t[d]elete\t= delete results\n"
		"\t\t[f]field\t= printf only specified field\n\n"
		"\t\texample: pmb -s linux\n"
		"\t\t         pmb -s name=linux\n"
		"\t\t         pmb -s name=linux,e=another.db\n"
		"\t\t         pmb -s linux,d\n\n"
		"\t\t         pmb -s linux,field=url\n\n");

	printf("\t-l | --colors\n"
		"\t\t[n]ame\t\t= name color\n"
		"\t\t[u]rl\t\t= url color\n"
		"\t\t[c]omment\t= comment color\n"
		"\t\t[t]ag\t\t= tag color\n\n"
		"\t\texample: pmb s programming -l n=red\n\n");

	printf("\t-c | --color\t\tcolored output\n");

	printf("\t-C | --config\t\tfilename:\topen config file 'filename'\n");

	printf("\t-f | --file\t\tfilename:\topen bookmark file 'filename'\n");

	printf("\t-i | --import\t\tfilename:\timport bookmarks from 'filename'\n");

	printf("\t-H | --html:\t\tprint in html\n");

	printf("\t-h | --help:\t\tshow [this] help\n");

	printf("\t-v | --verbose:\t\tbe verbose\n");

	printf("\t-V | --version:\t\tshow version\n");

	printf("\n");
}

void 
version() 
{
	printf("Poor man's bookmark, by neoncortex, version %s\n", VERSION);
}

void
bookmark_print_field(int i, char* r) 
{
	if(color && isatty(STDOUT_FILENO)) 
	{
		if(i % 2)
			printf("%s%s%s\n", white, r, reset);
		else
			printf("%s%s%s\n", green, r, reset);
	}
	else
		printf("%s\n", r);
}

void
bookmark_print(bookmark_list* bl, int field) 
{
	for(int i = 0; i < bookmark_list_get_size(bl) - 1; ++i) 
	{
		char** result = bookmark_list_return_next(bl);

		if(field)
		{
			if(field == 1) 
				bookmark_print_field(i, result[0]);

			if(field == 2) 
				bookmark_print_field(i, result[1]);

			if(field == 3) 
				bookmark_print_field(i, result[2]);

			if(field == 4) 
				bookmark_print_field(i, result[3]);

			if(field == 5) 
				bookmark_print_field(i, result[4]);
		}
		else 
		{
			if(verbose)
			{
				if(color && isatty(STDOUT_FILENO))
					printf("id\t\t%s%s%s\n"
						"name\t\t%s%s%s\n"
						"url\t\t%s%s%s\n"
						"comment\t\t%s%s%s\n"
						"tag\t\t%s%s%s\n\n"
						,id_color, result[0], reset 
						,name_color, result[1], reset
						,url_color, result[2], reset
						,comment_color, result[3], reset
						,tag_color, result[4], reset);
				else
					printf("id\t\t%s\n"
						"name\t\t%s\n"
						"url\t\t%s\n"
						"comment\t\t%s\n"
						"tag\t\t%s\n\n"
						,result[0], result[1], result[2], result[3]
						,result[4]);
			}
			else
			{
				if(color && isatty(STDOUT_FILENO))
					printf("%s%s%s\n"
						"%s%s%s\n"
						"%s%s%s\n"
						"%s%s%s\n"
						"%s%s%s\n\n"
						,id_color, result[0], reset
						,name_color, result[1], reset
						,url_color, result[2], reset
						,comment_color, result[3], reset
						,tag_color, result[4], reset);
				else
					printf("%s\n"
						"%s\n"
						"%s\n"
						"%s\n"
						"%s\n\n"
						,result[0], result[1], result[2], result[3]
						,result[4]);
			}
		}
	}
}

void
print_folder_icon(int* canvas)
{
	printf(
		"<canvas id='canvas_%d' width='20' height='14'></canvas>\n"
		"<script>\n"
    		"var ctx = document.getElementById(\"canvas_%d\").getContext(\"2d\");\n"
		"// #layer1\n"
		"ctx.save();\n"
		"ctx.transform(1.000000, 0.000000, 0.000000, 1.000000, -33.095309, -103.342050);\n"
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
		"</script>\n", *canvas, *canvas);
		++(*canvas);
}

int
html_tree_table_row(directory* d, int depth, int* canvas)
{
	if(d)
	{
		printf("<tr>\n");

		for(int i = 0; i < depth; ++i)
			printf("<td></td>\n");

		printf("<td>\n");
		print_folder_icon(canvas);
		printf("<span style='color:brown; font-weight:bold;'>"
			"%s</span></td>\n"
			,directory_name(d));

		printf("</tr>\n");

		bookmark* b = NULL;

		while((b = directory_return_next_bookmark(d)))
		{
			printf("<tr>\n");

			for(int i = 0; i < depth + 1; ++i)
				printf("<td></td>\n");

			printf("<td width=\'200\'>\n""<a href=\'%s\'>%s</a></td>\n</tr>\n"
				,bookmark_url(b), bookmark_name(b));

			bookmark_destroy(b);
		}

		printf("<tr></tr>\n");
		return ++depth;
	}

	return 0;
}

void
html_tree_branch(directory* d, int depth, int* canvas)
{
	if(d)
	{
		directory_rewind(d);
		directory* ret = NULL;

		depth = html_tree_table_row(d, depth, canvas);

		while((ret = directory_return_next_children(d)))
		{
			directory_rewind(ret);
			html_tree_table_row(ret, depth, canvas);
			html_tree_branch(directory_return_next_children(ret), depth + 1, canvas);
		}

		directory_destroy(ret);
	}
}

int
bookmark_html_tree(bookmark_list* bl) 
{
	if(bl) 
	{
		bookmark_list* 	bl 	= bookmark_db_query(db, 0, NULL);
		directory* 	root 	= create_tree_from_bookmark_list(bl, "root");	
		directory* 	child 	= NULL;
		directory* 	ret 	= NULL;
		int		canvas	= 0;

		directory_rewind(root);

		printf("<!DOCTYPE html>\n"
			"<html>\n"
			"<head>\n"
			"<meta charset=\'UTF-8\'>\n"
			"<style>\n"
			"html, body, table { font-size: 12px; }\n"
			"h3 { margin: 0; border: 0; color: steelblue; }\n"
			"</style>\n"
			"</head>\n"
			"<body bgcolor=mintcream>\n");

		printf("<h3>Bookmarks:</h3>"
			"<div style='border:1px solid green;"
			"margin-bottom: 10px;"
			"margin-top: 10px;"
			"border-radius:3px;"
			"background-color:lavender;"
			"padding:4px;'>\n"
			"<table>\n");

		html_tree_table_row(root, 0, &canvas);

		while((child = directory_return_next_children(root)))
		{
			directory_rewind(child);
			html_tree_table_row(child, 1, &canvas);

			while((ret = directory_return_next_children(child)))
				html_tree_branch(ret, 2, &canvas);
		}

		char buff[50];

		printf("</table>\n</div>\n<br />\nGenerated in: "
			"<span style='color:green;'>%s</span>\n"
			,date(buff, 50));
		
		printf("</body>\n</html>\n");

		directory_destroy(ret);
		directory_destroy(child);
		directory_destroy(root);
		bookmark_list_destroy(bl);

		return 0;
	}

	return 1;
}

char* 
find_color(char* color)
{
	if((!strcmp(color, "red")
	||(!strcmp(color, "Red"))))
		return red;

	else if((!strcmp(color, "blue")
	||(!strcmp(color, "Blue"))))
		return blue;

	else if((!strcmp(color, "cyan")
	||(!strcmp(color, "Cyan"))))
		return cyan;

	else if((!strcmp(color, "green")
	||(!strcmp(color, "Green"))))
		return green;
		
	else if((!strcmp(color, "reset")
	||(!strcmp(color, "Reset"))))
		return reset;

	else if((!strcmp(color, "white")
	||(!strcmp(color, "White"))))
		return white;

	else if((!strcmp(color, "magenta")
	||(!strcmp(color, "Magenta"))))
		return magenta;

	else if((!strcmp(color, "gray")
	||(!strcmp(color, "Gray"))))
		return gray;

	else if((!strcmp(color, "yellow")
	||(!strcmp(color, "Yellow"))))
		return yellow;

	else
		return reset;
}

int
parse_config_file(char* optarg)
{
	char** res = NULL;

	if(optarg)
		res 	= read_config(optarg);
	else
		return 1;

	if(res)
	{
		if(res[0])
		{
			int r = strtol(res[0], NULL, 10);	

			if(r == 0 || r == 1)
				color = r;
			else
			{
				printf("unknown color parameter\nshoud be 0 or 1\n");
				exit(EXIT_FAILURE);
			}
		}

		if(res[1])
		{
			int r = strtol(res[1], NULL, 10);	

			if(r == 0 || r == 1)
				verbose = r;
			else
			{
				printf("unknown verbose parameter\nshoud be 0 or 1\n");
				exit(EXIT_FAILURE);
			}
		}

		if(res[2])
			id_color 	= find_color(res[2]);

		if(res[3])
			name_color 	= find_color(res[3]);

		if(res[4])
			url_color 	= find_color(res[4]);

		if(res[5])
			comment_color 	= find_color(res[5]);

		if(res[6])
			tag_color 	= find_color(res[6]);

	}
	else
		return 1;

	return 0;
}

int
add_bookmark(char* optarg) 
{
	if(db) 
	{
		char*	name 	= NULL;
		char*	url 	= NULL;
		char*	comment	= NULL;
		char*	tag	= NULL;
		char*	value   = NULL;
		char*	subopts = optarg;

		bookmark* b = NULL;

		enum 
		{ 
			name_option
			,sname_option
			,url_option
			,surl_option
			,comment_option
			,scomment_option
			,tag_option
			,stag_option
		};

		char* const sub_options[] = 
		{
			[name_option] 		= NAME
			,[sname_option] 	= "n"
			,[url_option] 		= URL
			,[surl_option] 		= "u"
			,[comment_option] 	= COMMENT
			,[scomment_option] 	= "c"
			,[tag_option] 		= TAG
			,[stag_option] 		= "t"
			,NULL
		};

		while(*subopts != '\0')
			switch(getsubopt(&subopts, sub_options, &value)) 
			{
				case name_option:
				case sname_option:
					if(value) 
						name = value;
					else
						printf("add: name needs an argument\n");

					break;
				case url_option:
				case surl_option:
					if(value) 
						url = value;
					else
						printf("add: url needs an argument\n");

					break;
				case comment_option:
				case scomment_option:
					if(value) 
						comment = value;
					else
						printf("add: comment needs an argument\n");

					break;
				case tag_option:
				case stag_option:
					if(value) 
						tag = value;
					else
						printf("add: tag needs an argument\n");

					break;		
				default:
					printf("need at least name and url to create a bookmark.\n");
					break;
			}

		if(name && url) 
		{
			b = bookmark_create(name, url, comment, tag);

			if(b) 
			{
				if(bookmark_db_write(b, db))
					printf("failed to write bookmark to database\n");

				bookmark_destroy(b);
				return 0;
			}
			else
				printf("error creating bookmark\n");
		}
		else 
			printf("need at least name and url to create a bookmark.\n");
	}
	else 
	{
		printf("error: database is closed\n");
		return 1;
	}

	return 0;
}

int
edit_bookmark(char* optarg)
{
	if(db) 
	{
		int 	id 	= 0;
		char*	name 	= NULL;
		char*	url 	= NULL;
		char*	comment	= NULL;
		char*	tag 	= NULL;
		char*	field	= NULL;
		char*	c_value	= NULL;
		char*	n_value = NULL;
		char*	value 	= NULL;
		char*	subopts	= optarg;

		enum 
		{ 
			id_option
			,sid_option
			,name_option
			,sname_option
			,url_option
			,surl_option
			,comment_option
			,scomment_option
			,tag_option
			,stag_option
			,field_option
			,sfield_option
			,value_option
			,svalue_option
			,new_value_option
			,snew_value_option
		};

		char* const sub_options[] = 
		{
			[id_option] 		= ID
			,[sid_option] 		= "i"
			,[name_option] 		= NAME
			,[sname_option] 	= "n"
			,[url_option] 		= URL
			,[surl_option] 		= "u"
			,[comment_option] 	= COMMENT
			,[scomment_option] 	= "c"
			,[tag_option] 		= TAG
			,[stag_option] 		= "t"
			,[field_option] 	= "field"
			,[sfield_option] 	= "f"
			,[value_option] 	= "value"
			,[svalue_option] 	= "v"
			,[new_value_option] 	= "new-value"
			,[snew_value_option] 	= "w"
			,NULL
		};

		while(*subopts != '\0')
			switch(getsubopt(&subopts, sub_options, &value)) 
			{
				case id_option:
				case sid_option:
					if(value) 
					{
						id = strtol(value, NULL, 10);	

						if((!(id < INT_MAX - 1))
						||(!(id > INT_MIN)))
						{
							printf("integer too big\n");
							exit(EXIT_FAILURE);
						}
					}
					else
						printf("edit: id needs an argument\n");

					break;
				case name_option:
				case sname_option:
					if(value) 
						name = value;
					else
						printf("edit: name needs an argument\n");

					break;
				case url_option:
				case surl_option:
					if(value) 
						url = value;
					else
						printf("edit: url needs an argument\n");

					break;
				case comment_option:
				case scomment_option:
					if(value) 
						comment = value;
					else
						printf("edit: comment needs an argument\n");

					break;
				case tag_option:
				case stag_option:
					if(value) 
						tag = value;
					else
						printf("edit: tag needs an argument\n");

					break;		
				case field_option:
				case sfield_option:
					if(value) 
						field = value;	
					else
						printf("edit: field needs an argument\n");

					break;
				case value_option:
				case svalue_option:
					if(value) 
						c_value = value;
					else
						printf("edit: value needs an argument\n");

					break;
				case new_value_option:
				case snew_value_option:
					if(value) 
						n_value = value;
					else
						printf("edit: new-value needs an argument\n");

					break;
				default:
					break;
			}

		if(field && !strncmp(field, "name", 4) && c_value && n_value) 
		{
			if(bookmark_db_edit_bulk(db, 0, c_value, n_value))
				printf("can't edit bookmarks named '%s'\n", c_value);
		}

		if(field && !strncmp(field, "url", 3) && c_value && n_value) 
		{
			if(bookmark_db_edit_bulk(db, 1, c_value, n_value))
				printf("can't edit bookmarks with url '%s'\n", c_value);
		}

		if(field && !strncmp(field, "comment", 7) && c_value && n_value) 
		{
			if(bookmark_db_edit_bulk(db, 2, c_value, n_value))
				printf("can't edit bookmarks with comment '%s'\n", c_value);
		}

		if(field && !strncmp(field, "tag", 3) && c_value && n_value) 
		{
			if(bookmark_db_edit_bulk(db, 3, c_value, n_value))
				printf("can't edit bookmarks with tag '%s'\n", c_value);
		}

		if(name && id) 
		{
			if(bookmark_db_edit(db, id, 0, name))
				printf("can't edit name of bookmark id=%d\n", id);
		}

		if(url && id) 
		{
			if(bookmark_db_edit(db, id, 1, url))
				printf("can't edit url bookmark id=%d\n", id);
		}

		if(comment && id) 
		{
			if(bookmark_db_edit(db, id, 2, comment))
				printf("can't edit comment of bookmark id=%d\n", id);
		}

		if(tag && id) 
		{
			if(bookmark_db_edit(db, id, 3, tag))
				printf("can't edit tag of bookmark id=%d\n", id);
		}
	}
	else
	{
		printf("error: database is closed\n");
		return 1;
	}

	return 0;
}

int
open_file(char* optarg)
{
	if(optarg) 
	{
		//bookmark_db_close(db);	
		db = bookmark_db_open(optarg);

		if(!db) 
		{
			printf("error opening database file=%s\n", optarg);
			//exit(EXIT_FAILURE);
			return 1;
		}
	}
	else
	{
		printf("file needs an argument\n");
		return 1;
	}

	return 0;
}

int
import(char* optarg)
{
	if(optarg)
	{
		sqlite3* i_db = bookmark_db_open(optarg);

		if(i_db)
			bookmark_db_import(db, i_db);
		else
			return 1;
	}
	else
		return 1;

	return 0;
}

int
search(char* optarg)
{
	if(db) 
	{
		int 	del	= 0;
		char*	subopts	= optarg;
		char*	value 	= NULL;
		char*	export	= NULL;
		char*	field	= NULL;

		bookmark_list* bl = NULL;
		enum 
		{ 
			name_option
			,sname_option
			,url_option
			,surl_option
			,comment_option
			,scomment_option
			,tag_option
			,stag_option
			,export_option
			,sexport_option
			,delete_option
			,sdelete_option
			,field_option
			,sfield_option
		};

		char* const sub_options[] = 
		{
			[name_option] 		= NAME
			,[sname_option] 	= "n"
			,[url_option] 		= URL
			,[surl_option] 		= "u"
			,[comment_option] 	= COMMENT
			,[scomment_option] 	= "c" 
			,[tag_option] 		= TAG
			,[stag_option] 		= "t"
			,[export_option]	= "export"
			,[sexport_option]	= "e"
			,[delete_option] 	= "delete"
			,[sdelete_option]	= "d"
			,[field_option]		= "field"
			,[sfield_option]	= "f"
			,NULL
		};

		while(*subopts != '\0')
			switch(getsubopt(&subopts, sub_options, &value)) 
			{
				case name_option:
				case sname_option:
					if(value) 
						bl = bookmark_db_search(db, NAME, value);
					else
						printf("search: name needs an argument\n");

					break;
				case url_option:
				case surl_option:
					if(value) 
						bl = bookmark_db_search(db, URL, value);
					else
						printf("search: url needs an argument\n");

					break;
				case comment_option:
				case scomment_option:
					if(value) 
						bl = bookmark_db_search(db, COMMENT, value);
					else
						printf("search: comment needs an argument\n");

					break;
				case tag_option:
				case stag_option:
					if(value) 
						bl = bookmark_db_search(db, TAG, value);
					else
						printf("search: tag needs an argument\n");

					break;		
				case export_option:
				case sexport_option:
					if(value) 
						export = value;
					else
						printf("search: export needs an argument\n");

					break;		
				case delete_option:
				case sdelete_option:
					del = 1;
					break;		
				case field_option:
				case sfield_option:
					if(value)
						field = value;
					else
						printf("search field needs an argument\n");

					break;		
				default:
					if(value) 
						bl = bookmark_db_search(db, NULL, value);
					else
						printf("search needs an argument\n");

					break;
			}

		if(bl) 
		{
			int f = 0;

			if(field)
			{
				if(!strncmp(field, ID, strlen(ID)) 
				||(!strncmp(field, "i", 1))) 
					f = 1;

				if(!strncmp(field, NAME, strlen(NAME)) 
				||(!strncmp(field, "n", 1))) 
					f = 2;

				if(!strncmp(field, URL, strlen(URL)) 
				||(!strncmp(field, "u", 1))) 
					f = 3;

				if(!strncmp(field, COMMENT, strlen(COMMENT)) 
				||(!strncmp(field, "c", 1))) 
					f = 4;

				if(!strncmp(field, TAG, strlen(TAG)) 
				||(!strncmp(field, "t", 1))) 
					f = 5;
			}

			bookmark_print(bl, f);

			if(export)
			{
				sqlite3* e_db = bookmark_db_open(export);

				if(e_db)
				{
					bookmark_list_rewind(bl);

					for(int i = 0; i < bookmark_list_get_size(bl) - 1; ++i) 
					{
						char** 		result	= bookmark_list_return_next(bl);
						bookmark* 	b	= bookmark_create(result[1], result[2]
										,result[3], result[4]);

						if(bookmark_db_write(b, e_db))
							printf("search: failed to export bookmark\n");

						bookmark_destroy(b);
					}

					bookmark_db_close(e_db);
				}
				else
					printf("failed to open database %s\n", export);
			}

			if(del)
			{
				bookmark_list_rewind(bl);

				for(int i = 0; i < bookmark_list_get_size(bl) - 1; ++i) 
				{
					char** 	result 	= bookmark_list_return_next(bl);
					int 	id 	= strtol(result[0], NULL, 10);	
					bookmark_db_delete(db, id);
				}
			}

			bookmark_list_destroy(bl);
		}
	}
	else
	{
		printf("error: database is closed\n");
		return 1;
	}

	return 0;
}

int
print_bookmark(char* optarg)
{
	if(db) 
	{
		int 	id 	= 0;
		char* 	field 	= NULL;

		char* 	subopts = optarg;
		char* 	value 	= NULL;

		bookmark_list* bl = NULL;


		enum 
		{ 
			id_option
			,sid_option
			,field_option
			,sfield_option
			,all_option
			,sall_option
		};

		char* const sub_options[] = 
		{
			[id_option] 		= ID
			,[sid_option] 		= "i"
			,[field_option] 	= "field"
			,[sfield_option] 	= "f"
			,[all_option] 		= "all"
			,[sall_option] 		= "a"
			,NULL
		};

		while(*subopts != '\0')
			switch(getsubopt(&subopts, sub_options, &value)) 
			{
				case id_option:
				case sid_option:
					if(value) 
					{
						id = strtol(value, NULL, 10);	

						if((!(id < INT_MAX - 1))
						||(!(id > INT_MIN)))
						{
							printf("integer too big\n");
							exit(EXIT_FAILURE);
						}
					}
					else
						printf("print: id needs an argument\n");

					break;
				case field_option:
				case sfield_option:
					if(value) 
						field = value;
					else
						printf("print: field needs an argument\n");

					break;		
				case all_option:
				case sall_option:
					bl = bookmark_db_query(db, 0 ,NULL);

					if(bl) 
						bookmark_print(bl, 0);
					else
						printf("failed to print bookmarks\n");

					break;
				default:
					break;
			}

		/* identifier + name or url or comment or tag */
		if(field) 
		{
			if(!strncmp(field, NAME, strlen(NAME)) 
			||(!strncmp(field, "n", 1))) 
			{
				if(id) 
					bl = bookmark_db_query(db, id, NAME);
				else
					bl = bookmark_db_query(db, 0, NAME);

				if(bl) 
					bookmark_print(bl, 2);
				else
				{
					if(id)
						printf("failed to print name of bookmark id=%d\n", id);
					else
						printf("failed to print name\n");
				}

				return 0;
			}

			if(!strncmp(field, URL, strlen(URL))
			||(!strncmp(field, "u", 1))) 
			{
				if(id) 
					bl = bookmark_db_query(db, id, URL);
				else
					bl = bookmark_db_query(db, 0, URL);

				if(bl) 
					bookmark_print(bl, 3);
				else
				{
					if(id)
						printf("failed to print url of bookmark id=%d\n", id);
					else
						printf("failed to print url\n");
				}

				return 0;
			}

			if(!strncmp(field, COMMENT, strlen(COMMENT))
			||(!strncmp(field, "c", 1))) 
			{
				if(id) 
					bl = bookmark_db_query(db, id, COMMENT);
				else
					bl = bookmark_db_query(db, 0, COMMENT);

				if(bl) 
					bookmark_print(bl, 4);
				else
				{
					if(id)
						printf("failed to print comment of bookmark id=%d\n", id);
					else
						printf("failed to print comment\n");
				}

				return 0;
			}

			if(!strncmp(field, TAG, strlen(TAG)) 
			||(!strncmp(field, "t", 1))) 
			{
				if(id) 
					bl = bookmark_db_query(db, id, TAG);
				else
					bl = bookmark_db_query(db, 0, TAG);

				if(bl) 
					bookmark_print(bl, 5);
				else
				{
					if(id)
						printf("failed to print tag of bookmark id=%d\n", id);
					else
						printf("failed to print tag\n");
				}

				return 0;
			}
		}

		/* only identifier */
		if(id) 
		{
			bl = bookmark_db_query(db, id, NULL);

			if(bl) 
				bookmark_print(bl, 0);
			else
				printf("failed to print bookmark id=%d\n", id);

			return 0;
		}

		if(bl)
			bookmark_list_destroy(bl);
	}
	else
	{
		printf("error: database is closed\n");
		return 1;
	}

	return 0;
}

int
delete_bookmark(char* optarg)
{
	if(db)
	{
		int	id	= 0;
		int	greedy	= 0;
		char* 	tag	= NULL;

		char* 	subopts = optarg;
		char* 	value 	= NULL;

		enum 
		{ 
			id_option
			,sid_option
			,tag_option
			,stag_option
			,greedy_option
			,sgreedy_option
		};

		char* const sub_options[] = 
		{
			[id_option] 		= ID
			,[sid_option] 		= "i"
			,[tag_option] 		= TAG
			,[stag_option] 		= "t"
			,[greedy_option] 	= "greedy"
			,[sgreedy_option] 	= "g"
			,NULL
		};

		while(*subopts != '\0')
			switch(getsubopt(&subopts, sub_options, &value)) 
			{
				case id_option:
				case sid_option:
				{
					if(value)
					{
						id = strtol(value, NULL, 10);	

						if((!(id < INT_MAX - 1))
						||(!(id > INT_MIN)))
						{
							printf("integer too big\n");
							exit(EXIT_FAILURE);
						}
					}

					break;
				}
				case tag_option:
				case stag_option:
					if(value)
						tag = value;
					else
					{
						printf("delete: tag needs argument\n");
						return 1;
					}

					break;
				case greedy_option:
				case sgreedy_option:
					greedy = 1;
					break;		
				default:
					break;
			}

		if(id)
		{
			if(bookmark_db_delete(db, id))
				printf("failed to delete the bookmark "
					"id=%d\n", id);
		}

		if(tag)
		{
			if(greedy)
			{
				if(bookmark_db_delete_tag(db, tag, 1))
					printf("failed to delete the bookmark "
						"tags=%s\n", tag);
			}
			else
			{
				if(bookmark_db_delete_tag(db, tag, 0))
					printf("failed to delete the bookmark "
						"tag=%s\n", tag);
			}
		}
	}
	else
	{
		printf("error: database is closed\n");
		return 1;
	}

	return 0;
}

int
html(char* optarg) 
{
	if(db) 
	{
		bookmark_list *bl = bookmark_db_query(db, 0, NULL);

		if(bl) 
		{
			bookmark_html_tree(bl);
			bookmark_list_destroy(bl);
		}
	}
	else
		return 1;

	return 0;
}

int
color_list(char* optarg)
{
	if(optarg)
	{
		char* 	subopts = optarg;
		char* 	value 	= NULL;

		enum 
		{ 
			id_option
			,sid_option
			,name_option
			,sname_option
			,url_option
			,surl_option
			,comment_option
			,scomment_option
			,tag_option
			,stag_option
		};

		char* const sub_options[] = 
		{
			[id_option] 		= ID
			,[sid_option] 		= "i"
			,[name_option] 		= NAME
			,[sname_option]		= "n"
			,[url_option] 		= URL
			,[surl_option] 		= "u"
			,[comment_option] 	= COMMENT
			,[scomment_option] 	= "c"
			,[tag_option] 		= TAG
			,[stag_option] 		= "t"
			,NULL
		};

		while(*subopts != '\0')
			switch(getsubopt(&subopts, sub_options, &value)) 
			{
				case id_option:
				case sid_option:
				{
					if(value)
						id_color = find_color(value);

					break;
				}
				case name_option:
				case sname_option:
				{
					if(value)
						name_color = find_color(value);

					break;
				}
				case url_option:
				case surl_option:
				{
					if(value)
						url_color = find_color(value);

					break;
				}
				case comment_option:
				case scomment_option:
				{
					if(value)
						comment_color = find_color(value);

					break;
				}
				case tag_option:
				case stag_option:
				{
					if(value)
						tag_color = find_color(value);

					break;
				}
				default:
					break;
			}
	}

	return 0;
}

int
set_color(char* optarg)
{
	color ? (color = 0) : (color = 1);
	return 0;
}

int
set_verbose(char* optarg)
{
	verbose ? (verbose = 0) : (verbose = 1);
	return 0;
}

void
parse_options(int argc, char* argv[], cl_option_list* option
	,cl_option_list* command) 
{
	int c, option_index 	= 0; 	/* getopt */

	static struct option long_options[] = 
	{
		{"add", 	1, 0, 'a'}
		,{"color", 	0, 0, 'c'}
		,{"colors",	1, 0, 'l'}
		,{"config", 	1, 0, 'C'}
		,{"delete", 	1, 0, 'd'}
		,{"edit", 	1, 0, 'e'}
		,{"open-file", 	1, 0, 'f'}
		,{"import", 	1, 0, 'i'}
		,{"help", 	0, 0, 'h'}
		,{"html", 	0, 0, 'H'}
		,{"print", 	1, 0, 'p'}
		,{"search", 	1, 0, 's'}
		,{"verbose", 	0, 0, 'V'}
		,{"version", 	0, 0, 'V'}
		,{NULL, 	0, NULL, 0}
	};

	while((c = getopt_long(argc, argv, "a:C:e:d:s:f:i:p:l:chHVv", long_options
		,&option_index))) 
	{
		if(c == -1) 
			break;	

		switch(c) 
		{
			case 'a': /* add */
			{
				cl_option* opt = create_option(add_bookmark
					,optarg);
				option_list_add(command, opt);	

				break;
			}
			case 'C': /* config */
			{
				cl_option* opt = create_option(parse_config_file
					,optarg);
				option_list_add(option, opt);	

				break;
			}
			case 'c': /* color */
			{
				cl_option* opt = create_option(set_color
					,optarg);
				option_list_add(option, opt);	

				break;
			}
			case 'd': /* delete */
			{
				cl_option* opt = create_option
					(delete_bookmark, optarg);
				option_list_add(command, opt);	

				break;
			}
			case 'e': /* edit */
			{
				cl_option* opt = create_option(edit_bookmark
					,optarg);
				option_list_add(command, opt);	

				break;
			}
			case 'f': /* open file */
			{
				cl_option* opt = create_option(open_file
					,optarg);
				option_list_add(option, opt);	

				break;
			}
			case 'H': /* html */
			{
				cl_option* opt = create_option(html, optarg);
				option_list_add(command, opt);	

				break;
			}
			case 'h': /* help */
				help();
				version();
				exit(EXIT_SUCCESS);

			case 'i': /* import file */
			{
				cl_option* opt = create_option(import
					,optarg);
				option_list_add(command, opt);	

				break;
			}
			case 'l': /* color */
			{
				cl_option* opt = create_option(color_list
					,optarg);
				option_list_add(option, opt);	

				break;
			}
			case 'p': /* print */
			{
				cl_option* opt = create_option(print_bookmark
					,optarg);
				option_list_add(command, opt);	

				break;
			}
			case 's': /* search */
			{
				cl_option* opt = create_option(search, optarg);
				option_list_add(command, opt);	

				break;
			}
			case 'v': /* verbose */
			{
				cl_option* opt = create_option(set_verbose
					,optarg);
				option_list_add(option, opt);	

				break;
			}
			case 'V': /* version */
				version();
				exit(EXIT_SUCCESS);
			default: /* anything else */
				printf("unknown option\n");
				help();
				exit(EXIT_FAILURE);
		}
	}
}

char**
read_config(char* filename) 
{
	int 	st;
	FILE* 	fp = fopen(filename, "r");

	if(fp)
	{
		int 	size 	= 1;
		char*	option 	= calloc(size, sizeof(char));
		char** 	ret 	= calloc(7, sizeof(char*));

		ret[0] 		= NULL;
		ret[1] 		= NULL;

		while(!feof(fp))
		{
			st = fgetc(fp);

			if(st == '\n')
			{
				if(!(strcmp(option, "\0")) 	/* blank line */
				||(option[0] == ' ')
				||(option[0] == '\t')
				||(option[0] == '#'))		/* comment */
					goto new_option;

				char* str = strsep(&option, "=");

				if(!(strcmp(str, "color")))
					ret[0] = strsep(&option, "=");

				else if(!(strcmp(str, "verbose")))
					ret[1] = strsep(&option, "=");

				else if(!(strcmp(str, "id_color")))
					ret[2] = strsep(&option, "=");

				else if(!(strcmp(str, "name_color")))
					ret[3] = strsep(&option, "=");

				else if(!(strcmp(str, "url_color")))
					ret[4] = strsep(&option, "=");

				else if(!(strcmp(str, "comment_color")))
					ret[5] = strsep(&option, "=");

				else if(!(strcmp(str, "tag_color")))
					ret[6] = strsep(&option, "=");

				else
				{
					printf("unknown option: %s\n", str);
					exit(EXIT_FAILURE);
				}

				new_option:
				free(option);
				size 	= 1;
				option 	= calloc(size, sizeof(char));
			}
			else
			{
				option[size - 1] = st;
				option 		 = realloc(option, ++size * sizeof(char));
				option[size - 1] = '\0';
			}
		}

		fclose(fp);
		return ret;
	}
	else
		return NULL;
}

