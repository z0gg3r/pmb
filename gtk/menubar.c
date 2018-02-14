#include "menubar.h"

GtkWidget*
menu_bar_new(GtkWidget* main_window) 
{
	/*
	GMenu* file_menu = g_menu_new();
	g_menu_insert(file_menu, 0, "File", NULL);
	g_menu_insert(file_menu, 1, "Exit", NULL);

	GMenu* edit_menu = g_menu_new();
	g_menu_insert(edit_menu, 0, "Insert", NULL);
	g_menu_insert(edit_menu, 1, "Edit", NULL);
	g_menu_insert(edit_menu, 2, "Delete", NULL);

	GMenu* help_menu = g_menu_new();
	g_menu_insert(help_menu, 0, "About", NULL);

	GMenu* menu = g_menu_new();
	g_menu_append_submenu(menu, "File", file_menu);
	g_menu_append_submenu(menu, "Edit", edit_menu);
	g_menu_append_submenu(menu, "Help", help_menu);

	*/

	/* file menu */
	GtkWidget* file_menu 	= gtk_menu_new();
	GtkWidget* file_top	= gtk_menu_item_new_with_mnemonic("_File");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_top), file_menu);

	GtkWidget* open 	= gtk_menu_item_new_with_mnemonic("_Open");
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), open);
	g_signal_connect(GTK_WIDGET(open), "activate"
		,G_CALLBACK(open_database), main_window);

	GtkWidget* exit 	= gtk_menu_item_new_with_mnemonic("E_xit");
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), exit);
	g_signal_connect(GTK_WIDGET(exit), "activate"
		,G_CALLBACK(destroy), NULL);

	/* edit menu */
	GtkWidget* edit_menu 	= gtk_menu_new();
	GtkWidget* edit_top 	= gtk_menu_item_new_with_mnemonic("_Edit");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit_top), edit_menu);

	GtkWidget* reload	= gtk_menu_item_new_with_mnemonic("_Reload");
	gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), reload);
	g_signal_connect(GTK_WIDGET(reload), "activate"
		,G_CALLBACK(read_database), NULL);

	GtkWidget* copy		= gtk_menu_item_new_with_mnemonic("_Copy");
	gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), copy);
	g_signal_connect(GTK_WIDGET(copy), "activate"
		,G_CALLBACK(copy_to_clipboard), NULL);

	GtkWidget* insert 	= gtk_menu_item_new_with_mnemonic("_Insert");
	gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), insert);
	g_signal_connect(GTK_WIDGET(insert), "activate"
		,G_CALLBACK(add_window), main_window);

	GtkWidget* edit 	= gtk_menu_item_new_with_mnemonic("_Edit");
	gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), edit);
	g_signal_connect(GTK_WIDGET(edit), "activate"
		,G_CALLBACK(edit), main_window);

	GtkWidget* rename_t 	= gtk_menu_item_new_with_mnemonic("_Rename");
	gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), rename_t);
	g_signal_connect(GTK_WIDGET(rename_t), "activate"
		,G_CALLBACK(rename_directory_wrapper), main_window);

	GtkWidget* delete	= gtk_menu_item_new_with_mnemonic("_Delete");
	gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), delete);
	g_signal_connect(GTK_WIDGET(delete), "activate"
		,G_CALLBACK(delete), main_window);

	GtkWidget* preferences	= gtk_menu_item_new_with_mnemonic
		("_Preferences");
	gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), preferences);
	g_signal_connect(GTK_WIDGET(preferences), "activate"
		,G_CALLBACK(options_window), main_window);

	/* view menu */
	GtkWidget* view_menu 	= gtk_menu_new();
	GtkWidget* view_top 	= gtk_menu_item_new_with_mnemonic("_View");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(view_top), view_menu);

	GtkWidget* toolbar 	= gtk_menu_item_new_with_mnemonic("_Tool box");
	gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), toolbar);
	g_signal_connect(GTK_WIDGET(toolbar), "activate"
		,G_CALLBACK(toolbox_hide), tool_box);

	/* help menu */
	GtkWidget* help_menu	= gtk_menu_new();
	GtkWidget* help_top	= gtk_menu_item_new_with_mnemonic("_Help");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(help_top), help_menu);

	GtkWidget* about	= gtk_menu_item_new_with_mnemonic("_About");
	gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), about);

	/* menu bar */
	GtkWidget *menu_bar 	= gtk_menu_bar_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_top);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), edit_top);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), view_top);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help_top);

	return menu_bar;
}

