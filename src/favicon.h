#ifndef FAVICON_H
#define FAVICON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "error.h"

typedef char gchar;
typedef unsigned char guchar;

gchar *download_favicon(char *url);

#endif
