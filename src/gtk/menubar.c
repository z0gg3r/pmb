#include "menubar.h"

GtkWidget*
menu_bar_new(GtkWidget* main_window) 
{
	/* file menu */
	GtkWidget* file_menu 	= gtk_menu_new();
	GtkWidget* file_top	= gtk_menu_item_new_with_mnemonic("_File");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_top), file_menu);

	/* file menu > open item */
	GtkWidget* open 	= gtk_menu_item_new_with_mnemonic("_Open database");
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), open);
	g_signal_connect(GTK_WIDGET(open), "activate"
		,G_CALLBACK(open_database), main_window);

	/* file menu > import menu */
	GtkWidget* import_menu 	= gtk_menu_new();
	GtkWidget* import_top	= gtk_menu_item_new_with_mnemonic("_Import");
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), import_top);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(import_top), import_menu);

	/* file menu > import menu > database item */
	GtkWidget* import 	= gtk_menu_item_new_with_mnemonic("_Database");
	gtk_menu_shell_append(GTK_MENU_SHELL(import_menu), import);
	g_signal_connect(GTK_WIDGET(import), "activate"
		,G_CALLBACK(import_database), NULL);

	/* file menu > import menu > selective import item */
	GtkWidget* sel_import 	= gtk_menu_item_new_with_mnemonic("_Selective import");
	gtk_menu_shell_append(GTK_MENU_SHELL(import_menu), sel_import);
	g_signal_connect(GTK_WIDGET(sel_import), "activate"
		,G_CALLBACK(selective_import_window), NULL);

	/* file menu > export menu */
	GtkWidget* export_menu 	= gtk_menu_new();
	GtkWidget* export_top	= gtk_menu_item_new_with_mnemonic("_Export");
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), export_top);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(export_top), export_menu);

	/* file menu > export menu > html page item*/
	GtkWidget* export_html	= gtk_menu_item_new_with_mnemonic("_Html page");
	gtk_menu_shell_append(GTK_MENU_SHELL(export_menu), export_html);

	g_signal_connect(GTK_WIDGET(export_html), "activate"
		,G_CALLBACK(export_html_page), NULL);

	/* file menu > export menu > selective export item */
	GtkWidget* sel_export 	= gtk_menu_item_new_with_mnemonic("_Selective export");
	gtk_menu_shell_append(GTK_MENU_SHELL(export_menu), sel_export);
	g_signal_connect(GTK_WIDGET(sel_export), "activate"
		,G_CALLBACK(selective_export_window), NULL);

	/* file menu > exit item */
	GtkWidget* exit 	= gtk_menu_item_new_with_mnemonic("E_xit");
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), exit);
	g_signal_connect(GTK_WIDGET(exit), "activate"
		,G_CALLBACK(destroy), NULL);

	/* edit menu */
	GtkWidget* edit_menu 	= gtk_menu_new();
	GtkWidget* edit_top 	= gtk_menu_item_new_with_mnemonic("_Edit");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit_top), edit_menu);

	/* edit menu > reload item */
	GtkWidget* reload	= gtk_menu_item_new_with_mnemonic("_Reload");
	gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), reload);
	g_signal_connect(GTK_WIDGET(reload), "activate"
		,G_CALLBACK(read_database), NULL);

	/* edit menu > copy item */
	GtkWidget* copy		= gtk_menu_item_new_with_mnemonic("_Copy");
	gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), copy);
	g_signal_connect(GTK_WIDGET(copy), "activate"
		,G_CALLBACK(copy_to_clipboard), NULL);

	/* edit menu > insert item */
	GtkWidget* insert 	= gtk_menu_item_new_with_mnemonic("_Insert");
	gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), insert);
	g_signal_connect(GTK_WIDGET(insert), "activate"
		,G_CALLBACK(add_window), NULL);

	/* edit menu > edit item */
	GtkWidget* edit 	= gtk_menu_item_new_with_mnemonic("_Edit");
	gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), edit);
	g_signal_connect(GTK_WIDGET(edit), "activate"
		,G_CALLBACK(edit), NULL);

	/* edit menu > rename directory item */
	GtkWidget* rename_t 	= gtk_menu_item_new_with_mnemonic("_Rename");
	gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), rename_t);
	g_signal_connect(GTK_WIDGET(rename_t), "activate"
		,G_CALLBACK(rename_directory_wrapper), NULL);

	/* edit menu > delete item */
	GtkWidget* delete	= gtk_menu_item_new_with_mnemonic("_Delete");
	gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), delete);
	g_signal_connect(GTK_WIDGET(delete), "activate"
		,G_CALLBACK(delete), NULL);

	/* edit menu > preferences item */
	GtkWidget* preferences	= gtk_menu_item_new_with_mnemonic
		("_Preferences");
	gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), preferences);
	g_signal_connect(GTK_WIDGET(preferences), "activate"
		,G_CALLBACK(options_window), NULL);

	/* view menu */
	GtkWidget* view_menu 	= gtk_menu_new();
	GtkWidget* view_top 	= gtk_menu_item_new_with_mnemonic("_View");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(view_top), view_menu);

	/* view menu > toolbar item */
	GtkWidget* toolbar 	= gtk_menu_item_new_with_mnemonic("_Tool box");
	gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), toolbar);
	g_signal_connect(GTK_WIDGET(toolbar), "activate"
		,G_CALLBACK(widget_hide), "tool_bar");

	/* view menu > statusbar item */
	GtkWidget* statusbar 	= gtk_menu_item_new_with_mnemonic("_Status box");
	gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), statusbar);
	g_signal_connect(GTK_WIDGET(statusbar), "activate"
		,G_CALLBACK(widget_hide), "info_box");

	/* help menu */
	GtkWidget* help_menu	= gtk_menu_new();
	GtkWidget* help_top	= gtk_menu_item_new_with_mnemonic("_Help");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(help_top), help_menu);

	/* help menu > about item */
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

