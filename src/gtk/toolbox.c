#include "toolbox.h"

#define TOOLS_N 6

static void
tool_item_generator(GtkWidget* bar, int index, char* name
		    ,char* tooltip, char* icon, int icon_size
		    ,void* function(), void* function_arg)
{
  GtkWidget* icon_t = gtk_image_new_from_icon_name
    (icon, icon_size);
  
  GtkToolItem* item_t = gtk_tool_button_new(icon_t, name);
  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(item_t), tooltip);
  
  g_signal_connect
    (item_t, "clicked", G_CALLBACK(function), function_arg);

  gtk_toolbar_insert(GTK_TOOLBAR(bar), item_t, index);
}

GtkWidget*
tool_box_new(GtkWidget* main_window) 
{
  GtkWidget* tool_bar = gtk_toolbar_new();
  gtk_toolbar_set_style(GTK_TOOLBAR(tool_bar), GTK_TOOLBAR_BOTH);

  void* tool_itens[TOOLS_N][5] =
    {
      {"Add", "add", "bookmark_add", add_window, NULL}
      ,{"Rename", "rename directories", "folder"
	,rename_directory_wrapper, NULL}
      ,{"Delete", "delete", "edit-delete", delete, NULL}
      ,{"Reload", "reload", "view-refresh", read_database, NULL}
      ,{"Copy", "copy to clipboard", "gtk-paste"
	,copy_to_clipboard, NULL}
      ,{"Preferences", "preferences", "system-run"
	,options_window, NULL}
    };

  for(int i = 0; i < TOOLS_N; ++i)
    {
      tool_item_generator
	(tool_bar, i, tool_itens[i][0], tool_itens[i][1]
	 ,tool_itens[i][2], GTK_ICON_SIZE_MENU
	 ,(void*)tool_itens[i][3]
	 ,(void*)tool_itens[i][4]);
    }

  return tool_bar;
}
