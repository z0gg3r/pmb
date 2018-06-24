#include "menubar.h"

#define MENU_ITEMS_N 19

static void
menu_item_generator(char* name, void* function()
		    ,void* function_arg, GtkWidget* menu)
{
  GtkWidget* item = gtk_menu_item_new_with_mnemonic(name);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);

  if(function)
    {
      g_signal_connect
	(GTK_WIDGET(item), "activate", G_CALLBACK(function)
	 ,function_arg);
    }
}

GtkWidget*
menu_bar_new(GtkWidget* main_window) 
{
  /* file menu */
  GtkWidget* file_menu 	= gtk_menu_new();
  GtkWidget* file_top = gtk_menu_item_new_with_mnemonic("_File");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_top), file_menu);

  /* file menu > import menu */
  GtkWidget* import_menu 	= gtk_menu_new();
  GtkWidget* import_top	= gtk_menu_item_new_with_mnemonic("_Import");
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), import_top);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(import_top), import_menu);

  /* file menu > export menu */
  GtkWidget* export_menu = gtk_menu_new();
  GtkWidget* export_top	= gtk_menu_item_new_with_mnemonic("_Export");
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), export_top);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(export_top), export_menu);

  /* edit menu */
  GtkWidget* edit_menu = gtk_menu_new();
  GtkWidget* edit_top = gtk_menu_item_new_with_mnemonic("_Edit");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit_top), edit_menu);

  /* view menu */
  GtkWidget* view_menu = gtk_menu_new();
  GtkWidget* view_top = gtk_menu_item_new_with_mnemonic("_View");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(view_top), view_menu);

  /* help menu */
  GtkWidget* help_menu = gtk_menu_new();
  GtkWidget* help_top = gtk_menu_item_new_with_mnemonic("_Help");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(help_top), help_menu);

  /* help menu > about item */
  GtkWidget* about = gtk_menu_item_new_with_mnemonic("_About");
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), about);
  
  /* menu bar */
  GtkWidget *menu_bar = gtk_menu_bar_new();
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_top);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), edit_top);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), view_top);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help_top);

  void* menu_items[MENU_ITEMS_N][4] =
  {
    {"_Open database", open_database, main_window, file_menu}
    ,{"_Database", import_database, NULL, import_menu}
    ,{"_Selective import", selective_import_window, NULL, import_menu}
    ,{"_Html page", export_html_page, NULL, export_menu}
    ,{"_Selective export", selective_export_window, NULL, export_menu}
    ,{"E_xit", destroy, NULL, file_menu}
    ,{"_Reload", read_database, NULL, edit_menu}
    ,{"_Copy", copy_to_clipboard, NULL, edit_menu}
    ,{"_Insert", add_window, NULL, edit_menu}
    ,{"_Edit", edit, NULL, edit_menu}
    ,{"_Rename", rename_directory_wrapper, NULL, edit_menu}
    ,{"_Delete", delete, NULL, edit_menu}
    ,{"Get _Favicons", get_all_favicons, NULL, edit_menu}
    ,{"_Preferences", options_window, NULL, edit_menu}
    ,{"_Tool bar", widget_hide, "toolbar", view_menu}
    ,{"_Status bar", widget_hide, "statusbar", view_menu}
    ,{"_Headers", widget_hide, "headers", view_menu}
    ,{"_Searchbar", widget_hide, "searchbar", view_menu}
    ,{"_About", NULL, NULL, help_menu}
  };

  for(int i = 0; i < MENU_ITEMS_N; ++i)
    {
      menu_item_generator
	(menu_items[i][0]
	 ,(void*)menu_items[i][1]
	 ,(void*)menu_items[i][2]
	 ,menu_items[i][3]);
    }
  
  return menu_bar;
}
