#ifndef OPTIONS_H
#define OPTIONS_H

#include "interface.h"
#include "dialog.h"

#define NAME_OPTIONS_SIZE 32 /* how many options */
#define KEYS_N 21 /* how many keybinds */

/* used to store parsed key options */
struct key_s
{
  unsigned int state;
  char* key;
};

typedef struct key_s key_s;

extern key_s* keys[KEYS_N];
extern char* name_options_l[NAME_OPTIONS_SIZE][3];

/* options */
#define DATABASE_OPTION name_options_l[0][1]
#define TREE_LINES_OPTION name_options_l[1][1]
#define DOWNLOAD_FAVICON_OPTION name_options_l[2][1]
#define ID_FG_OPTION name_options_l[3][1]
#define NAME_FG_OPTION name_options_l[4][1]
#define URL_FG_OPTION name_options_l[5][1]
#define COMMENT_FG_OPTION name_options_l[6][1]
#define ID_FONT_OPTION name_options_l[7][1]
#define NAME_FONT_OPTION name_options_l[8][1]
#define URL_FONT_OPTION name_options_l[9][1]
#define COMMENT_FONT_OPTION name_options_l[10][1]
#define SCROLL_UP_KEY name_options_l[11][1]
#define SCROLL_DOWN_KEY name_options_l[12][1]
#define SCROLL_LEFT_KEY name_options_l[13][1]
#define SCROLL_RIGHT_KEY name_options_l[14][1]
#define SCROLL_TOP_KEY name_options_l[15][1]
#define SCROLL_BOTTOM_KEY name_options_l[16][1]
#define SCROLL_BEGIN_LINE_KEY name_options_l[17][1]
#define SCROLL_END_LINE_KEY name_options_l[18][1]
#define SCROLL_PAGE_UP_KEY name_options_l[19][1]
#define SCROLL_PAGE_DOWN_KEY name_options_l[20][1]
#define EC_KEY name_options_l[21][1]
#define EA_KEY name_options_l[22][1]
#define SEARCH_BAR_KEY name_options_l[23][1]
#define COPY_KEY name_options_l[24][1]
#define RELOAD_KEY name_options_l[25][1]
#define INSERT_KEY name_options_l[26][1]
#define EDIT_KEY name_options_l[27][1]
#define DELETE_KEY name_options_l[28][1]
#define RENAME_KEY name_options_l[29][1]
#define OPTIONS_KEY name_options_l[30][1]
#define OPEN_DATABASE_KEY name_options_l[31][1]

/* key options parsed */
#define SCROLL_UP keys[0]
#define SCROLL_DOWN keys[1]
#define SCROLL_LEFT keys[2]
#define SCROLL_RIGHT keys[3]
#define SCROLL_TOP keys[4]
#define SCROLL_BOTTOM keys[5]
#define SCROLL_PAGE_UP keys[6]
#define SCROLL_PAGE_DOWN keys[7]
#define SCROLL_BEGIN_LINE keys[8]
#define SCROLL_END_LINE keys[9]
#define EC keys[10]
#define EA keys[11]
#define SEARCH_BAR keys[12]
#define COPY keys[13]
#define RELOAD keys[14]
#define INSERT keys[15]
#define EDIT keys[16]
#define DELETE keys[17]
#define RENAME keys[18]
#define OPTIONS keys[19]
#define OPEN_DATABASE keys[20]

/* -- options -- */
void
read_config(); 

/* used in the end of program, just to be a gentleman */
void
destroy_options();

/* -- the option window -- */
void 
options_window(GtkWidget*);

#endif
