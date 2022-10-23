LDADD = -lsqlite3

if LIBCURL
LDADD+=-lcurl
endif

bin_PROGRAMS = pmb
pmb_SOURCES = src/pmb.c src/bookmark.c src/bookmark.h src/bookmark_list.c src/bookmark_list.h src/database.c src/database.h src/error.c src/error.h src/html.c src/html.h src/option.c src/option.h src/parser.c src/parser.h src/tree.c src/tree.h
man1_MANS = pmb.1

if LIBCURL
pmb_SOURCES += src/favicon.c src/favicon.h src/base64.c src/base64.h
endif
