/* based on: https://curl.haxx.se/libcurl/c/getinmemory.html */

#include "favicon.h"

struct favicon_mem {
	guchar *memory;
	size_t size;
};

static size_t favicon_fetch(void *contents, size_t size, size_t nmemb,
			    void *userp)
{
	size_t realsize = size * nmemb;
	struct favicon_mem *mem = (struct favicon_mem *)userp;

	mem->memory = realloc(mem->memory, mem->size + realsize + 1);
	check_oom(mem->memory, "favicon - mem->memory");

	memcpy(&(mem->memory[mem->size]), contents, realsize);

	mem->size += realsize;
	mem->memory[mem->size] = '\0';

	return realsize;
}

gchar *download_favicon(char *url)
{
	if (!url) {
		return NULL;
	}

	struct favicon_mem favicon;
	favicon.memory = malloc(1);
	check_oom(favicon.memory,
		  "favicon > download_favicon - favicon.memory");
	favicon.size = 0;

	unsigned int size = 0;
	gchar *favicon_enc = NULL;
	char *g_url = "https://www.google.com/s2/favicons?domain_url=";

	size = strlen(g_url) + strlen(url) + 1;

	char *full_url = calloc(size, sizeof(char));
	check_oom(full_url, "favicon > download_favicon - full_url");

	snprintf(full_url, size, "%s%s", g_url, url);

	CURL *curl_handle = curl_easy_init();
	CURLcode res;

	curl_easy_setopt(curl_handle, CURLOPT_URL, full_url);
	curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, favicon_fetch);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&favicon);
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "pmb/0.1");
	res = curl_easy_perform(curl_handle);

	if (res != CURLE_OK) {
		fprintf(stderr, "libcurl: %s\n", curl_easy_strerror(res));
	} else {
		favicon_enc = g_base64_encode(favicon.memory, favicon.size);
	}

	curl_easy_cleanup(curl_handle);
	free(favicon.memory);

	if (favicon_enc) {
		return favicon_enc;
	}

	return NULL;
}
