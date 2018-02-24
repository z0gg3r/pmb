#ifndef FAVICON_H
#define FAVICON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <glib.h>

gchar*
download_favicon(char* url);

#endif
