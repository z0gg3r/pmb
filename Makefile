# gcc
CC=gcc
CCFLAGS=-Wall -Wextra -ggdb -lcurl $(CFLAGS)
GLIBFLAGS =$(shell pkg-config --cflags --libs glib-2.0)
SQLITE_FLAGS=-l sqlite3

# directories
SRC_DIR=$(shell pwd)
INC_DIR=./src
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

# create build directory
$(shell if [ ! -e "./build" ]; then mkdir build; fi)

default: $(PMB)

all: default

$(BUILD_DIR)/%.o: $(INC_DIR)/%.c $(PMB_HEADERS)
	$(CC) $(CCFLAGS) $(GLIBFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(GPMB_DIR)/%.c $(GPMB_HEADERS)
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c $< -o $@

$(PMB): $(PMB_OBJ) $(PMB).c
	$(CC) $(CCFLAGS) $(GLIBFLAGS) $(SQLITE_FLAGS) \
	$(PMB_OBJ) $(PMB).c -o $(PMB)

run:$(PMB)
	./$(PMB) $(args)

install:$(PMB)
	-cp $(PMB) $(DEST_DIR)

clean:
	-rm $(PMB)
	-rm -rf $(BUILD_DIR)

valgrind_pmb:
	valgrind --leak-check=full --track-origins=yes -show-leak-kinds=all --log-file=log.txt -v ./pmb -s linux
