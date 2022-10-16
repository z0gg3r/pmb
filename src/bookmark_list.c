#include "bookmark_list.h"

struct bookmark_list {
	int size;
	int position;
	int next;
	char **id;
	char **name;
	char **url;
	char **comment;
	char **tag;
	char **favicon;
};

bookmark_list *bookmark_list_new()
{
	bookmark_list *l = calloc(1, sizeof(bookmark_list));
	check_oom(l, "bookmark_list > bookmark_list_new - l");

	l->position = 0;
	l->next = 0;
	l->size = 1;

	l->id = calloc(l->size, sizeof(char *));
	l->name = calloc(l->size, sizeof(char *));
	l->url = calloc(l->size, sizeof(char *));
	l->comment = calloc(l->size, sizeof(char *));
	l->tag = calloc(l->size, sizeof(char *));
	l->favicon = calloc(l->size, sizeof(char *));

	l->id[0] = NULL;
	l->name[0] = NULL;
	l->url[0] = NULL;
	l->comment[0] = NULL;
	l->tag[0] = NULL;
	l->favicon[0] = NULL;

	return l;
}

void bookmark_list_destroy(bookmark_list *l)
{
	if (l) {
		for (int i = 0; i < l->size - 1; ++i) {
			if (l->id[i]) {
				free(l->id[i]);
				l->id[i] = NULL;
			}

			if (l->name[i]) {
				free(l->name[i]);
				l->name[i] = NULL;
			}

			if (l->url[i]) {
				free(l->url[i]);
				l->url[i] = NULL;
			}

			if (l->comment[i]) {
				free(l->comment[i]);
				l->comment[i] = NULL;
			}

			if (l->tag[i]) {
				free(l->tag[i]);
				l->tag[i] = NULL;
			}

			if (l->favicon[i]) {
				free(l->favicon[i]);
				l->favicon[i] = NULL;
			}
		}

		free(l->id);
		free(l->name);
		free(l->url);
		free(l->comment);
		free(l->tag);
		free(l->favicon);
		free(l);
	}
}

int bookmark_list_enqueue(bookmark_list *l, char *id, char *name, char *url,
			  char *comment, char *tag, char *favicon)
{
	if (id) {
		l->id[l->position] = calloc(strlen(id) + 1, sizeof(char));

		check_oom(l->id[l->position],
			  "bookmark_list_enqueue - l->id[l->position]");

		strncpy(l->id[l->position], id, strlen(id));
	}

	if (name) {
		l->name[l->position] = calloc(strlen(name) + 1, sizeof(char));

		check_oom(l->name[l->position],
			  "bookmark_list_enqueue - l->name[l->position]");

		strncpy(l->name[l->position], name, strlen(name));
	}

	if (url) {
		l->url[l->position] = calloc(strlen(url) + 1, sizeof(char));

		check_oom(l->url[l->position],
			  "bookmark_list_enqueue - l->url[l->position]");

		strncpy(l->url[l->position], url, strlen(url));
	}

	if (comment) {
		l->comment[l->position] =
			calloc(strlen(comment) + 1, sizeof(char));

		check_oom(l->comment[l->position],
			  "bookmark_list_enqueue - l->comment[l->position]");

		strncpy(l->comment[l->position], comment, strlen(comment));
	}

	if (tag) {
		l->tag[l->position] = calloc(strlen(tag) + 1, sizeof(char));

		check_oom(l->tag[l->position],
			  "bookmark_list_enqueue - l->tag[l->position]");

		strncpy(l->tag[l->position], tag, strlen(tag));
	}

	if (favicon) {
		l->favicon[l->position] =
			calloc(strlen(favicon) + 1, sizeof(char));

		check_oom(l->favicon[l->position],
			  "bookmark_list_enqueue - l->favicon[l->position]");

		strncpy(l->favicon[l->position], favicon, strlen(favicon));
	}

	++l->position;

	if (l->position >= l->size - 1) {
		++l->size;
		char **nid = realloc(l->id, l->size * sizeof(char *));
		char **nname = realloc(l->name, l->size * sizeof(char *));
		char **nurl = realloc(l->url, l->size * sizeof(char *));
		char **ncomment = realloc(l->comment, l->size * sizeof(char *));
		char **ntag = realloc(l->tag, l->size * sizeof(char *));
		char **nfavicon = realloc(l->favicon, l->size * sizeof(char *));

		if (nid && nname && nurl && ncomment && ntag) {
			l->id = nid;
			l->name = nname;
			l->url = nurl;
			l->comment = ncomment;
			l->tag = ntag;
			l->favicon = nfavicon;

			l->id[l->size - 1] = NULL;
			l->name[l->size - 1] = NULL;
			l->url[l->size - 1] = NULL;
			l->comment[l->size - 1] = NULL;
			l->tag[l->size - 1] = NULL;
			l->favicon[l->size - 1] = NULL;
		} else {
			check_oom(NULL, "bookmark_list_enqueue - **n realloc");
		}
	}

	return 0;
}

int bookmark_list_enqueue_bookmark(bookmark_list *l, bookmark *b)
{
	char *id = bookmark_id(b);
	char *name = bookmark_name(b);
	char *url = bookmark_url(b);
	char *comment = bookmark_comment(b);
	char *tag = bookmark_tag(b);
	char *favicon = bookmark_favicon(b);
	int res =
		bookmark_list_enqueue(l, id, name, url, comment, tag, favicon);

	return res;
}

void bookmark_list_align(bookmark_list *l)
{
	int c = 1;

	while (c < l->size) {
		if (l->id[c - 1]) {
			free(l->id[c - 1]);
			l->id[c - 1] = NULL;
		}

		if (l->name[c - 1]) {
			free(l->name[c - 1]);
			l->name[c - 1] = NULL;
		}

		if (l->url[c - 1]) {
			free(l->url[c - 1]);
			l->url[c - 1] = NULL;
		}

		if (l->comment[c - 1]) {
			free(l->comment[c - 1]);
			l->comment[c - 1] = NULL;
		}

		if (l->tag[c - 1]) {
			free(l->tag[c - 1]);
			l->tag[c - 1] = NULL;
		}

		if (l->favicon[c - 1]) {
			free(l->favicon[c - 1]);
			l->favicon[c - 1] = NULL;
		}

		l->id[c - 1] = l->id[c];
		l->name[c - 1] = l->name[c];
		l->url[c - 1] = l->url[c];
		l->comment[c - 1] = l->comment[c];
		l->tag[c - 1] = l->tag[c];
		l->favicon[c - 1] = l->favicon[c];
		++c;
	}

	--l->size;
	--l->position;
}

char **bookmark_list_dequeue(bookmark_list *l)
{
	if (l->size) {
		char **ret = calloc(5, sizeof(char *));
		ret[0] = l->id[0];
		ret[1] = l->name[0];
		ret[2] = l->url[0];
		ret[3] = l->comment[0];
		ret[4] = l->tag[0];
		ret[5] = l->favicon[0];

		bookmark_list_align(l);

		if (l->size) {
			char **nid = realloc(l->id, l->size * sizeof(char *));
			char **nname =
				realloc(l->name, l->size * sizeof(char *));
			char **nurl = realloc(l->url, l->size * sizeof(char *));
			char **ncomment =
				realloc(l->comment, l->size * sizeof(char *));
			char **ntag = realloc(l->tag, l->size * sizeof(char *));
			char **nfavicon =
				realloc(l->tag, l->size * sizeof(char *));

			if (nid && nname && nurl && ncomment && ntag) {
				l->id = nid;
				l->name = nname;
				l->url = nurl;
				l->comment = ncomment;
				l->tag = ntag;
				l->favicon = nfavicon;
			} else {
				check_oom(
					NULL,
					"bookmark_list_dequeue - **n realloc");
				return NULL;
			}
		}

		return ret;
	}

	return NULL;
}

char **bookmark_list_return_next(bookmark_list *l)
{
	if (l->next < l->size - 1) {
		char **ret = calloc(5, sizeof(char *));
		ret[0] = l->id[l->next];
		ret[1] = l->name[l->next];
		ret[2] = l->url[l->next];
		ret[3] = l->comment[l->next];
		ret[4] = l->tag[l->next];
		ret[5] = l->favicon[l->next];

		l->next++;
		return ret;
	}

	return NULL;
}

bookmark *bookmark_list_return_next_bookmark(bookmark_list *l)
{
	if (l->next < l->size - 1) {
		bookmark *b = bookmark_new();

		bookmark_set_id(b, l->id[l->next]);
		bookmark_set_name(b, l->name[l->next]);
		bookmark_set_url(b, l->url[l->next]);
		bookmark_set_comment(b, l->comment[l->next]);
		bookmark_set_tag(b, l->tag[l->next]);
		bookmark_set_favicon(b, l->favicon[l->next]);

		l->next++;
		return b;
	}

	return NULL;
}

void bookmark_list_rewind(bookmark_list *l)
{
	l->next = 0;
}

int bookmark_list_get_size(bookmark_list *l)
{
	if (l->size) {
		return l->size;
	}

	return 0;
}

int bookmark_list_get_position(bookmark_list *l)
{
	if (l->position) {
		return l->position;
	}

	return 0;
}

int bookmark_list_get_next_position(bookmark_list *l)
{
	if (l->next) {
		return l->next;
	}

	return 0;
}
