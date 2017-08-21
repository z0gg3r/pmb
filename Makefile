# variables
CC=gcc
CCFLAGS=-Wall -Wextra -std=c11 -l sqlite3 -ggdb
GTKFLAGS = `pkg-config --cflags --libs gtk+-3.0`
SRC_DIR=`pwd`
DEST_DIR=/usr/local/bin

# source files
PMB=pmb
PARSER=parser
OPTION=option
BOOKMARK=bookmark
TREE=tree
GPMB=gpmb

# recipes
# -- bflags = flags for bookmark.c build
# -- pflags = flags for pmb.c build
# -- gflags = flags for gpmb.c
# -- args = command line arguments for run pmb/gpmb

default:
	$(CC) $(CCFLAGS) -c $(BOOKMARK).c $(bflags)
	$(CC) $(CCFLAGS) -c $(PARSER).c 
	$(CC) $(CCFLAGS) -c $(OPTION).c 
	$(CC) $(CCFLAGS) -c $(TREE).c 
	$(CC) $(CCFLAGS) $(BOOKMARK).o $(PARSER).o $(OPTION).o $(TREE).o $(PMB).c -o $(PMB) $(pflags)
	#$(CC) $(CCFLAGS) $(GTKFLAGS) $(BOOKMARK).o $(GPMB).c -o $(GPMB) $(gflags)

$(BOOKMARK):$(BOOKMARK).c
	$(CC) $(CCFLAGS) -c $(BOOKMARK).c $(bflags)

$(PMB):$(PMB).c
	$(CC) $(CCFLAGS) $(BOOKMARK).o $(PMB).c -o $(PMB) $(pflags)

$(GPMB):$(GPMB).c
	$(CC) $(CCFLAGS) $(GTKFLAGS) $(BOOKMARK).o $(GPMB).c -o $(GPMB) $(gflags)

run:$(PMB)
	./$(PMB) $(args)

grun:$(GPMB)
	./$(GPMB) $(args)

install:$(PMB)
	cp "$(PWD)/$(PMB)" $(DEST_DIR)
	#cp "$(PWD)/$(GPMB)" $(DEST_DIR)

clean:$(PMB)
	rm $(PMB)
	rm $(GPMB)
	rm *.o

