#include "toolbox.h"

GtkWidget*
tool_box_new(GtkWidget* main_window) 
{
	short icon_size = GTK_ICON_SIZE_MENU;

	/* add */
	GtkWidget* add_icon 	= gtk_image_new_from_icon_name
		("bookmark_add", icon_size);
	GtkToolItem* add 	= gtk_tool_button_new(add_icon, "Add");
	//gtk_tool_button_set_use_underline(GTK_TOOL_BUTTON(add), TRUE);
	gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(add), "add");

	g_signal_connect(add, "clicked", G_CALLBACK(add_window)
		,main_window);

	/* edit */
	GtkWidget* edit_icon 	= gtk_image_new_from_icon_name
		("document-page-setup", icon_size);
	GtkToolItem* edit 	= gtk_tool_button_new(edit_icon, "Edit");
	//gtk_tool_button_set_use_underline(GTK_TOOL_BUTTON(edit), TRUE);
	gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(edit), "edit");

	g_signal_connect(edit, "clicked", G_CALLBACK(edit)
		,main_window);

	/* delete */
	GtkWidget* delete_icon	= gtk_image_new_from_icon_name
		("edit-delete", icon_size);
	GtkToolItem* delete 	= gtk_tool_button_new(delete_icon, "Delete");
	//gtk_tool_button_set_use_underline(GTK_TOOL_BUTTON(delete), TRUE);
	gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(delete), "delete");

	g_signal_connect(delete, "clicked", G_CALLBACK(delete)
		,main_window);

	/* options */
	GtkWidget* options_icon = gtk_image_new_from_icon_name
		("system-run", icon_size);
	GtkToolItem* options 	= gtk_tool_button_new(options_icon
					,"Option");
	//gtk_tool_button_set_use_underline(GTK_TOOL_BUTTON(options), TRUE);
	gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(options), "options");

	g_signal_connect(options, "clicked", G_CALLBACK(options_window)
		,main_window);

	/* reload */
	GtkWidget* reload_icon 	= gtk_image_new_from_icon_name
		("view-refresh", icon_size);
	GtkToolItem* reload 	= gtk_tool_button_new(reload_icon, "Reload");
	//gtk_tool_button_set_use_underline(GTK_TOOL_BUTTON(reload), TRUE);
	gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(reload)
		,"reload");

	g_signal_connect(reload, "clicked", G_CALLBACK(read_database)
		,NULL);

	/* copy */
	GtkWidget* copy_icon 	= gtk_image_new_from_icon_name
		("gtk-paste", icon_size);
	GtkToolItem* copy 	= gtk_tool_button_new(copy_icon, "Copy");
	//gtk_tool_button_set_use_underline(GTK_TOOL_BUTTON(copy), TRUE);
	gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(copy)
		,"copy to clipboard");

	g_signal_connect(copy, "clicked", G_CALLBACK(copy_to_clipboard)
		,NULL);

	/* tool bar */
	GtkWidget* tool_bar 	= gtk_toolbar_new();
	gtk_toolbar_set_style(GTK_TOOLBAR(tool_bar), GTK_TOOLBAR_BOTH);
	gtk_toolbar_insert(GTK_TOOLBAR(tool_bar), add, 0);
	gtk_toolbar_insert(GTK_TOOLBAR(tool_bar), edit, 1);
	gtk_toolbar_insert(GTK_TOOLBAR(tool_bar), delete, 2);
	gtk_toolbar_insert(GTK_TOOLBAR(tool_bar), reload, 3);
	gtk_toolbar_insert(GTK_TOOLBAR(tool_bar), copy, 4);
	gtk_toolbar_insert(GTK_TOOLBAR(tool_bar), options, 5);

	return tool_bar;
}

