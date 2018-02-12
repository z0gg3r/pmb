# gcc
CC=gcc
CCFLAGS=-Wall -Wextra -std=c11 -ggdb
GTKFLAGS =$(shell pkg-config --cflags --libs gtk+-3.0)
SQLITE_FLAGS=-l sqlite3

# directories
SRC_DIR=$(shell pwd)
INC_DIR=./include
GPMB_DIR=./gtk
BUILD_DIR=./build
DEST_DIR=/usr/local/bin

# source files
PMB=pmb
PARSER=parser
OPTION=option
BOOKMARK=bookmark
BOOKMARK_LIST=bookmark_list
DATABASE=database
TREE=tree
HTML=html

GPMB=gpmb
G_TREEVIEW=treeview
G_KEYPRESS=keypress
G_MENUBAR=menubar
G_TOOLBOX=toolbox
G_DIALOG=dialog
G_ADD=add
G_DELETE=delete
G_EDIT=edit
G_OPTIONS=options
G_SEARCHBOX=searchbox
G_TAGBOX=tagbox
G_INTERFACE=interface
#G_DIR_URL=directory_url

#PMB_SOURCE_FILES=$(INC_DIR)/$(BOOKMARK).c $(INC_DIR)/$(PARSER).c \
#	$(INC_DIR)/$(OPTION).c $(INC_DIR)/$(TREE).c \
#	$(INC_DIR)/$(HTML).c 

#GPMB_SOURCE_FILES=$(GPMB_DIR)/$(G_DIR_URL).c $(GPMB_DIR)/$(G_TREEVIEW).c \
#	$(GPMB_DIR)/$(G_KEYPRESS).c $(GPMB_DIR)/$(G_MENUBAR).c \
#	$(GPMB_DIR)/$(G_TOOLBOX).c $(GPMB_DIR)/$(G_DIALOG).c \
#	$(GPMB_DIR)/$(G_ADD).c $(GPMB_DIR)/$(G_DELETE).c \
#	$(GPMB_DIR)/$(G_EDIT).c $(GPMB_DIR)/$(G_OPTIONS).c \
#	$(GPMB_DIR)/$(G_SEARCHBOX).c $(GPMB_DIR)/$(G_TAGBOX).c \
#	$(GPMB_DIR)/$(G_INTERFACE).c

# create build directory
$(shell if [ ! -e "./build" ]; then mkdir build; fi)

define BUILD_DEP
	$(CC) $(CCFLAGS) -c $(INC_DIR)/$(BOOKMARK).c \
	-o $(BUILD_DIR)/$(BOOKMARK).o
	$(CC) $(CCFLAGS) -c $(INC_DIR)/$(BOOKMARK_LIST).c \
	-o $(BUILD_DIR)/$(BOOKMARK_LIST).o
	$(CC) $(CCFLAGS) -c $(INC_DIR)/$(DATABASE).c \
	-o $(BUILD_DIR)/$(DATABASE).o
	$(CC) $(CCFLAGS) -c $(INC_DIR)/$(PARSER).c \
	-o $(BUILD_DIR)/$(PARSER).o
	$(CC) $(CCFLAGS) -c $(INC_DIR)/$(OPTION).c \
	-o $(BUILD_DIR)/$(OPTION).o
	$(CC) $(CCFLAGS) -c $(INC_DIR)/$(TREE).c \
	-o $(BUILD_DIR)/$(TREE).o
	$(CC) $(CCFLAGS) -c $(INC_DIR)/$(HTML).c \
	-o $(BUILD_DIR)/$(HTML).o
endef

define BUILD_GPMB_DEP
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c $(GPMB_DIR)/$(G_TREEVIEW).c \
	-o $(BUILD_DIR)/$(G_TREEVIEW).o
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c $(GPMB_DIR)/$(G_KEYPRESS).c \
	-o $(BUILD_DIR)/$(G_KEYPRESS).o
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c $(GPMB_DIR)/$(G_MENUBAR).c \
	-o $(BUILD_DIR)/$(G_MENUBAR).o
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c $(GPMB_DIR)/$(G_TOOLBOX).c \
	-o $(BUILD_DIR)/$(G_TOOLBOX).o
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c $(GPMB_DIR)/$(G_DIALOG).c \
	-o $(BUILD_DIR)/$(G_DIALOG).o
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c $(GPMB_DIR)/$(G_ADD).c \
	-o $(BUILD_DIR)/$(G_ADD).o
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c $(GPMB_DIR)/$(G_DELETE).c \
	-o $(BUILD_DIR)/$(G_DELETE).o
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c $(GPMB_DIR)/$(G_EDIT).c \
	-o $(BUILD_DIR)/$(G_EDIT).o
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c $(GPMB_DIR)/$(G_OPTIONS).c \
	-o $(BUILD_DIR)/$(G_OPTIONS).o
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c $(GPMB_DIR)/$(G_SEARCHBOX).c \
	-o $(BUILD_DIR)/$(G_SEARCHBOX).o
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c $(GPMB_DIR)/$(G_TAGBOX).c \
	-o $(BUILD_DIR)/$(G_TAGBOX).o
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c $(GPMB_DIR)/$(G_INTERFACE).c \
	-o $(BUILD_DIR)/$(G_INTERFACE).o
endef

define BUILD_GPMB
	$(CC) $(CCFLAGS) $(GTKFLAGS) $(SQLITE_FLAGS) \
	$(BUILD_DIR)/$(BOOKMARK).o \
	$(BUILD_DIR)/$(BOOKMARK_LIST).o \
	$(BUILD_DIR)/$(DATABASE).o \
	$(BUILD_DIR)/$(TREE).o \
	$(BUILD_DIR)/$(G_TREEVIEW).o \
	$(BUILD_DIR)/$(G_KEYPRESS).o \
	$(BUILD_DIR)/$(G_DIALOG).o \
	$(BUILD_DIR)/$(G_ADD).o \
	$(BUILD_DIR)/$(G_DELETE).o \
	$(BUILD_DIR)/$(G_EDIT).o \
	$(BUILD_DIR)/$(G_OPTIONS).o \
	$(BUILD_DIR)/$(G_MENUBAR).o \
	$(BUILD_DIR)/$(G_TOOLBOX).o \
	$(BUILD_DIR)/$(G_SEARCHBOX).o \
	$(BUILD_DIR)/$(G_TAGBOX).o \
	$(BUILD_DIR)/$(G_INTERFACE).o\
	$(GPMB).c -o $(GPMB)
endef

define BUILD_PMB
	$(CC) $(CCFLAGS) $(SQLITE_FLAGS) \
	$(BUILD_DIR)/$(BOOKMARK).o \
	$(BUILD_DIR)/$(BOOKMARK_LIST).o \
	$(BUILD_DIR)/$(DATABASE).o \
	$(BUILD_DIR)/$(PARSER).o \
	$(BUILD_DIR)/$(OPTION).o \
	$(BUILD_DIR)/$(TREE).o \
	$(BUILD_DIR)/$(HTML).o\
	$(PMB).c -o $(PMB)
endef

default:
	$(BUILD_DEP)
	$(BUILD_PMB)
	$(BUILD_GPMB_DEP)
	$(BUILD_GPMB)

run:$(PMB)
	./$(PMB) $(args)

grun:$(GPMB)
	./$(GPMB) $(args)

install:$(PMB)
	cp "$(PWD)/$(PMB)" $(DEST_DIR)
	cp "$(PWD)/$(GPMB)" $(DEST_DIR)

clean:$(PMB)
	rm $(PMB)
	rm $(GPMB)
	rm -r $(BUILD_DIR)

$(BOOKMARK):$(BOOKMARK).c
	$(CC) $(CCFLAGS) -c $(INC_DIR)/$(BOOKMARK).c 

$(GPMB):
	$(BUILD_GPMB)

$(PMB):
	$(BUILD_PMB)

$(G_TREEVIEW):$(GPMB_DIR)/$(G_TREEVIEW).c
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c $(GPMB_DIR)/$(G_TREEVIEW).c \
	-o $(BUILD_DIR)/$(G_TREEVIEW).o
	$(BUILD_GPMB)

$(G_ADD):$(GPMB_DIR)/$(G_ADD).c
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c $(GPMB_DIR)/$(G_ADD).c \
	-o $(BUILD_DIR)/$(G_ADD).o
	$(BUILD_GPMB)

$(G_DELETE):$(GPMB_DIR)/$(G_DELETE).c
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c $(GPMB_DIR)/$(G_DELETE).c \
	-o $(BUILD_DIR)/$(G_DELETE).o
	$(BUILD_GPMB)

$(G_EDIT):$(GPMB_DIR)/$(G_EDIT).c
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c $(GPMB_DIR)/$(G_EDIT).c \
	-o $(BUILD_DIR)/$(G_EDIT).o
	$(BUILD_GPMB)

$(G_DIALOG):$(GPMB_DIR)/$(G_DIALOG).c
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c $(GPMB_DIR)/$(G_DIALOG).c \
	-o $(BUILD_DIR)/$(G_DIALOG).o
	$(BUILD_GPMB)

$(G_MENUBAR):$(GPMB_DIR)/$(G_MENUBAR).c
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c $(GPMB_DIR)/$(G_MENUBAR).c \
	-o $(BUILD_DIR)/$(G_MENUBAR).o
	$(BUILD_GPMB)

$(G_OPTIONS):$(GPMB_DIR)/$(G_OPTIONS).c
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c $(GPMB_DIR)/$(G_OPTIONS).c \
	-o $(BUILD_DIR)/$(G_OPTIONS).o
	$(BUILD_GPMB)

$(G_KEYPRESS):$(GPMB_DIR)/$(G_KEYPRESS).c
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c $(GPMB_DIR)/$(G_KEYPRESS).c \
	-o $(BUILD_DIR)/$(G_KEYPRESS).o
	$(BUILD_GPMB)

$(G_TOOLBOX):$(GPMB_DIR)/$(G_TOOLBOX).c
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c $(GPMB_DIR)/$(G_TOOLBOX).c \
	-o $(BUILD_DIR)/$(G_TOOLBOX).o
	$(BUILD_GPMB)

$(G_TAGBOX):$(GPMB_DIR)/$(G_TAGBOX).c
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c $(GPMB_DIR)/$(G_TAGBOX).c \
	-o $(BUILD_DIR)/$(G_TAGBOX).o
	$(BUILD_GPMB)

$(G_INTERFACE):$(GPMB_DIR)/$(G_INTERFACE).c
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c $(GPMB_DIR)/$(G_INTERFACE).c \
	-o $(BUILD_DIR)/$(G_INTERFACE).o
	$(BUILD_GPMB)

#$(G_DIR_URL):$(GPMB_DIR)/$(G_DIR_URL).c
#	$(CC) $(CCFLAGS) $(GTKFLAGS) -c $(GPMB_DIR)/$(G_DIR_URL).c \
#	-o $(BUILD_DIR)/$(G_DIR_URL).o
#	$(BUILD_GPMB)

$(TREE):
	$(CC) $(CCFLAGS) -c $(INC_DIR)/$(TREE).c \
	-o $(BUILD_DIR)/$(TREE).o

valgrind_gpmb:
	valgrind --leak-check=full --track-origins=yes --log-file=log.txt -v ./gpmb 

valgrind_pmb:
	valgrind --leak-check=full --track-origins=yes --log-file=log.txt -v ./pmb 

