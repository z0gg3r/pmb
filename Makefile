# gcc
CC=gcc
CCFLAGS=-Wall -Wextra -std=c11 -ggdb -lcurl
GTKFLAGS =$(shell pkg-config --cflags --libs gtk+-3.0)
GLIBFLAGS =$(shell pkg-config --cflags --libs glib-2.0)
SQLITE_FLAGS=-l sqlite3

# directories
SRC_DIR=$(shell pwd)
INC_DIR=./src
GPMB_DIR=./src/gtk
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
FAVICON=favicon
ERROR=error

GPMB=gpmb
G_TREEVIEW=treeview
G_KEYPRESS=keypress
G_MENUBAR=menubar
G_TOOLBOX=toolbox
G_DIALOG=dialog
G_ADD=add
G_DELETE=delete
G_EDIT=edit
G_FILE=file
G_OPTIONS=options
G_SEARCHBOX=searchbox
G_TAGBOX=tagbox
G_INTERFACE=interface

PMB_HEADERS=$(INC_DIR)/$(BOOKMARK).h $(INC_DIR)/$(BOOKMARK_LIST).h \
	$(INC_DIR)/$(DATABASE).h $(INC_DIR)/$(TREE).h \
	$(INC_DIR)/$(HTML).h $(INC_DIR)/$(FAVICON).h \
	$(INC_DIR)/$(PARSER).h $(INC_DIR)/$(OPTION).h \
	$(INC_DIR)/$(ERROR).h

PMB_OBJ=$(BUILD_DIR)/$(BOOKMARK).o $(BUILD_DIR)/$(BOOKMARK_LIST).o \
	$(BUILD_DIR)/$(DATABASE).o $(BUILD_DIR)/$(TREE).o \
	$(BUILD_DIR)/$(HTML).o $(BUILD_DIR)/$(FAVICON).o \
	$(BUILD_DIR)/$(PARSER).o $(BUILD_DIR)/$(OPTION).o \
	$(BUILD_DIR)/$(ERROR).o

GPMB_HEADERS=$(GPMB_DIR)/$(G_TREEVIEW).h $(GPMB_DIR)/$(G_KEYPRESS).h \
	$(GPMB_DIR)/$(G_MENUBAR).h $(GPMB_DIR)/$(G_TOOLBOX).h \
	$(GPMB_DIR)/$(G_DIALOG).h $(GPMB_DIR)/$(G_ADD).h \
	$(GPMB_DIR)/$(G_DELETE).h $(GPMB_DIR)/$(G_EDIT).h \
	$(GPMB_DIR)/$(G_FILE).h $(GPMB_DIR)/$(G_OPTIONS).h \
	$(GPMB_DIR)/$(G_SEARCHBOX).h $(GPMB_DIR)/$(G_TAGBOX).h \
	$(GPMB_DIR)/$(G_INTERFACE).h

GPMB_OBJ=$(BUILD_DIR)/$(G_TREEVIEW).o $(BUILD_DIR)/$(G_KEYPRESS).o \
	$(BUILD_DIR)/$(G_MENUBAR).o $(BUILD_DIR)/$(G_TOOLBOX).o \
	$(BUILD_DIR)/$(G_DIALOG).o $(BUILD_DIR)/$(G_ADD).o \
	$(BUILD_DIR)/$(G_DELETE).o $(BUILD_DIR)/$(G_EDIT).o \
	$(BUILD_DIR)/$(G_FILE).o $(BUILD_DIR)/$(G_OPTIONS).o \
	$(BUILD_DIR)/$(G_SEARCHBOX).o $(BUILD_DIR)/$(G_TAGBOX).o \
	$(BUILD_DIR)/$(G_INTERFACE).o \
	$(BUILD_DIR)/$(BOOKMARK).o $(BUILD_DIR)/$(BOOKMARK_LIST).o \
	$(BUILD_DIR)/$(DATABASE).o $(BUILD_DIR)/$(TREE).o \
	$(BUILD_DIR)/$(FAVICON).o $(BUILD_DIR)/$(HTML).o \
	$(BUILD_DIR)/$(ERROR).o

# create build directory
$(shell if [ ! -e "./build" ]; then mkdir build; fi)

default: $(PMB) $(GPMB)

all: default

$(BUILD_DIR)/%.o: $(INC_DIR)/%.c $(PMB_HEADERS)
	$(CC) $(CCFLAGS) $(GLIBFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(GPMB_DIR)/%.c $(GPMB_HEADERS)
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c $< -o $@

$(PMB): $(PMB_OBJ) $(PMB).c
	$(CC) $(CCFLAGS) $(GLIBFLAGS) $(SQLITE_FLAGS) \
	$(PMB_OBJ) $(PMB).c -o $(PMB)

$(GPMB): $(GPMB_OBJ) $(GPMB).c
	$(CC) $(CCFLAGS) $(GTKFLAGS) $(GLIBFLAGS) $(SQLITE_FLAGS) \
	$(GPMB_OBJ) $(GPMB).c -o $(GPMB)

run:$(PMB)
	./$(PMB) $(args)

grun:$(GPMB)
	./$(GPMB) $(args)

install:$(PMB)
	-cp $(PMB) $(DEST_DIR)
	-cp $(GPMB) $(DEST_DIR)

clean:
	-rm $(PMB)
	-rm $(GPMB)
	-rm -rf $(BUILD_DIR)

valgrind_gpmb:
	valgrind --leak-check=full --track-origins=yes --log-file=log.txt -v ./gpmb 

valgrind_pmb:
	valgrind --leak-check=full --track-origins=yes --log-file=log.txt -v ./pmb -s linux
