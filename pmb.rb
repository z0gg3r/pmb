#!/usr/bin/env ruby

require 'gtk3'
require 'sqlite3'

class Db
	def initialize path
		begin
			@db = SQLite3::Database.open path
		rescue
			puts 		"can't open database"
			exit
		end

		@db.execute 	"CREATE TABLE IF NOT EXISTS bookmark" \
					"(id INTEGER PRIMARY KEY" \
					",name text" \
					",url text" \
					",comment text" \
					",tag tex)"
	end

	def insert data
		stm 		= @db.prepare "INSERT INTO bookmark " \
				"(name, url, comment, tag) VALUES(?,?,?,?)"
	
		stm.bind_param 	1, data[0]
		stm.bind_param 	2, data[1]
		stm.bind_param 	3, data[2]
		stm.bind_param 	4, data[3]
		rs 		= stm.execute
	end

	def delete id
		stm 		= @db.prepare "DELETE FROM bookmark " \
					"WHERE id = ?"	

		stm.bind_param	1, id
		rs 		= stm.execute
	end

	def delete_tag tag
		stm 		= @db.prepare "DELETE FROM bookmark " \
					"WHERE tag = ?"

		stm.bind_param 	1, tag
		rs 		= stm.execute
	end

	def edit id, field, value
		if field == "name" \
		or field == "url" \
		or field == "comment" \
		or field == "tag"
			stm 		= @db.prepare "UPDATE bookmark " \
						"SET #{field} = ? WHERE id = ?"

			stm.bind_param 	1, value
			stm.bind_param 	2, id
			rs 		= stm.execute
		end
	end

	def edit_tag tag, new_tag
		stm 		= @db.prepare "UPDATE bookmark " \
					"SET tag = ? WHERE tag = ?"

		stm.bind_param 	1, new_tag
		stm.bind_param 	2, tag
		rs 		= stm.execute
	end

	def find id
		stm 		= @db.prepare "SELECT * FROM bookmark " \
					"WHERE id = ? ORDER BY tag ASC"

		stm.bind_param 	1, id
		rs		= stm.execute
	end

	def fetch 
		stm 		= @db.prepare "SELECT * from bookmark " \
					"ORDER BY tag ASC"

		rs 		= stm.execute
	end
end

class Actions
	def self.clipboard store, tree
		url 	= store.get_value \
				(tree.selection).selected, 2

		Process.detach spawn \
			"echo #{url} | xclip -i -selection 'primary']"

		Process.detach spawn \
			"echo #{url} | xclip -i -selection 'secoundary']"

		Process.detach spawn \
			"echo #{url} | xclip -i -selection 'clipboard']"
	end

	def self.store_feed db, store, tree
		store.clear
		store.feed 		db.fetch
		tree.expand_all
	end

	def self.insert db, store, tree
		Gtk_Insert.new 		db, store, tree
	end

	def self.edit db, store, tree
		Gtk_Edit.new 		db, store, tree
	end

	def self.delete db, store, tree
		Gtk_Delete.new 		db, store, tree
	end

	def self.error message, fatal
		Gtk_Error.new 		message, fatal
	end
end

class Store < Gtk::TreeStore
	def feed result
		last 			= nil
		number_of_items 	= 0

		for i in result do
			if !last or i[4] != last
				iter = insert nil, number_of_items \
					+= 1 \
					,[" " \
					," " \
					," " \
					,"#{i[4]}"]
			end

			insert iter, number_of_items += 1 \
				,["#{i[0]}" \
				,"#{i[1]}" \
				,"#{i[2]}" \
				,"" \
				,"#{i[3]}"]

			last 		= i[4]
		end
	end
end

class Gtk_Window < Gtk::Window
	def initialize 
		super
		set_wmclass 		"Poor man's bookmark", "Pmb"
		set_title 		"Poor man's bookmark"
		set_type_hint 		:dialog


		signal_connect "destroy" do
			destroy
		end
	end
end

class Gtk_Error < Gtk_Window
	def initialize message, fatal
		super()
		error_title 		= Gtk::Label.new 
		error_title.set_markup	"<span foreground='red' " \
						"font-weight='heavy' " \
						"size='large'>" \
						"Error \n" \
						"</span>"

		error 				= Gtk::Label.new message
		close_button 			= Gtk::Button.new :label => "_Close"
		close_button.use_underline 	= true

		close_button.signal_connect "clicked" do
			if fatal
				Gtk.main_quit
			else
				destroy
			end
		end

		vbox 			= Gtk::Box.new :vertical, 2
		vbox.pack_start 	error_title
		vbox.pack_start 	error
		vbox.pack_start 	close_button \
						,:expand => false \
						,:fill => false \
						,:padding => 0 \

		add vbox
		show_all
	end
end

class Gtk_Insert < Gtk_Window
	def initialize db, store, tree
		super()
		set_title 			"Poor man's bookmark - Insert"

		name_entry 			= Gtk::Entry.new
		url_entry 			= Gtk::Entry.new
		comment_entry 			= Gtk::Entry.new
		tag_entry 			= Gtk::Entry.new
		insert_button 			= Gtk::Button.new :label => "_Insert"
		insert_button.use_underline 	= true
		cancel_button 			= Gtk::Button.new :label => "_Cancel"
		cancel_button.use_underline 	= true


		if tree.selection.selected
			tag_entry.set_text store.get_value \
				(tree.selection).selected, 3
		end

		insert_button.signal_connect "clicked" do
			if (name_entry.text).size > 0 and (url_entry.text).size > 0
				name		= name_entry.text
				url 		= url_entry.text

				if (comment_entry.text).size == 0
					comment 	= "none"
				else
					comment 	= comment_entry.text
				end

				if (tag_entry.text).size == 0
					tag 		= "none"
				else
					tag 		= tag_entry.text
				end

				db.insert 		[name, url, comment, tag]
				Actions.store_feed 	db, store, tree
				destroy
			else
				Actions.error "Need at least name and url", false
			end
		end

		cancel_button.signal_connect "clicked" do
			destroy
		end

		grid 			= Gtk::Grid.new
		grid.set_property 	"row-homogeneous", true
		grid.set_property 	"column-homogeneous", true

								#e   #t   #l    #a
		grid.attach (Gtk::Label.new "Name"), 		0,   0,   20,   1
		grid.attach name_entry, 			20,  0,   20,   1
		grid.attach (Gtk::Label.new "Url"), 		0,   1,   20,   1
		grid.attach url_entry,				20,  1,   20,   1
		grid.attach (Gtk::Label.new "Comment"),		0,   2,   20,   1
		grid.attach comment_entry,			20,  2,   20,   1
		grid.attach (Gtk::Label.new "Tag"), 		0,   3,   20,   1
		grid.attach tag_entry,				20,  3,   20,   1
		grid.attach insert_button,			0,   4,   20,   1
		grid.attach cancel_button,			20,  4,   20,   1

		vbox 			= Gtk::Box.new :vertical, 2
		vbox.pack_start 	grid \
						,:expand => false \
						,:fill => false \
						,:padding => 0 \

		add vbox
		show_all
	end
end

class Gtk_Edit < Gtk_Window
	def initialize db, store, tree
		super()
		set_title 			"Poor man's bookmark - Edit"

		name_entry 			= Gtk::Entry.new
		url_entry 			= Gtk::Entry.new
		comment_entry 			= Gtk::Entry.new
		tag_entry 			= Gtk::Entry.new
		edit_button 			= Gtk::Button.new :label => "_Edit"
		edit_button.use_underline 	= true
		cancel_button 			= Gtk::Button.new :label => "_Cancel"
		cancel_button.use_underline 	= true

		grid 			= Gtk::Grid.new
		grid.set_property 	"row-homogeneous", true
		grid.set_property 	"column-homogeneous", true

		rs 			= db.find (store.get_value \
						(tree.selection).selected, 0)

		if n = rs.next
			name_entry.set_text 	n[1]
			url_entry.set_text 	n[2]
			comment_entry.set_text 	n[3]
			tag_entry.set_text 	n[4]

			edit_button.signal_connect "clicked" do
				if n[1] != name_entry.text 
					db.edit n[0], "name", name_entry.text
				end

				if n[2] != url_entry.text 
					db.edit n[0], "url", url_entry.text
				end

				if n[3] != comment_entry.text 
					db.edit n[0], "comment", comment_entry.text
				end

				if n[4] != tag_entry.text
					db.edit n[0], "tag", tag_entry.text
				end

				Actions.store_feed 	db, store, tree
				destroy
			end
								#e   #t   #l    #a
			grid.attach (Gtk::Label.new "Name"),	0,   0,   20,   1
			grid.attach name_entry, 		20,  0,   20,   1
			grid.attach (Gtk::Label.new "Url"), 	0,   1,   20,   1
			grid.attach url_entry,			20,  1,   20,   1
			grid.attach (Gtk::Label.new "Comment"),	0,   2,   20,   1
			grid.attach comment_entry,		20,  2,   20,   1
			grid.attach (Gtk::Label.new "Tag"), 	0,   3,   20,   1
			grid.attach tag_entry,			20,  3,   20,   1
			grid.attach edit_button,		0,   4,   20,   1
			grid.attach cancel_button,		20,  4,   20,   1
		else
			set_title 	"Poor man's bookmark - Edit tag"
			old_tag 	= store.get_value \
						(tree.selection).selected, 3

			tag_entry.set_text old_tag
			
			edit_button.signal_connect "clicked" do
				db.edit_tag 		old_tag, tag_entry.text
				Actions.store_feed 	db, store, tree
				destroy
			end
								#e   #t   #l    #a
			grid.attach (Gtk::Label.new "Tag"),	0,   0,   20,   1
			grid.attach tag_entry, 			20,  0,   20,   1
			grid.attach edit_button,		0,   1,   20,   1
			grid.attach cancel_button,		20,  1,   20,   1
		end

		cancel_button.signal_connect "clicked" do
			destroy
		end

		vbox 			= Gtk::Box.new :vertical, 2
		vbox.pack_start 	grid \
						,:expand 	=> false \
						,:fill 		=> false \
						,:padding 	=> 0 \

		add vbox
		show_all
	end
end

class Gtk_Delete < Gtk_Window
	def initialize db, store, tree
		super()
		set_title 			"Poor man's bookmark - Delete?"

		id 		= store.get_value \
					(tree.selection).selected, 0

		name 		= store.get_value \
					(tree.selection).selected, 1

		url 		= store.get_value \
					(tree.selection).selected, 2

		comment 	= store.get_value \
					(tree.selection).selected, 4

		tag 		= store.get_value \
					(tree.selection).selected, 3

		name_entry 			= Gtk::Entry.new
		name_entry.set_text 		name
		name_entry.set_editable 	false

		url_entry 			= Gtk::Entry.new
		url_entry.set_text 		url
		url_entry.set_editable 		false

		comment_entry 			= Gtk::Entry.new
		comment_entry.set_text 		comment
		comment_entry.set_editable 	false

		tag_entry 			= Gtk::Entry.new
		tag_entry.set_text 		tag
		tag_entry.set_editable 		false

		delete_button 			= Gtk::Button.new :label => "_Delete"
		delete_button.use_underline 	= true

		delete_button.signal_connect "clicked" do

			if id.size < 2
				db.delete_tag 		tag
				Actions.store_feed	db, store, tree
			else
				db.delete 		id
				Actions.store_feed 	db, store, tree
			end

			destroy
		end

		cancel_button 			= Gtk::Button.new :label => "_Cancel"
		cancel_button.use_underline 	= true

		cancel_button.signal_connect "clicked" do
			destroy
		end

		grid 			= Gtk::Grid.new
		grid.set_property 	"row-homogeneous", true
		grid.set_property 	"column-homogeneous", true

								#e   #t   #l    #a
		grid.attach (Gtk::Label.new "Name"), 		0,   0,   20,   1
		grid.attach name_entry, 			20,  0,   20,   1
		grid.attach (Gtk::Label.new "Url"), 		0,   1,   20,   1
		grid.attach url_entry,				20,  1,   20,   1
		grid.attach (Gtk::Label.new "Comment"),		0,   2,   20,   1
		grid.attach comment_entry,			20,  2,   20,   1
		grid.attach (Gtk::Label.new "Tag"), 		0,   3,   20,   1
		grid.attach tag_entry,				20,  3,   20,   1
		grid.attach delete_button,			0,   4,   20,   1
		grid.attach cancel_button,			20,  4,   20,   1

		vbox 			= Gtk::Box.new :vertical, 2
		vbox.pack_start 	grid \
						,:expand => false \
						,:fill => false \
						,:padding => 0 \

		cancel_button.grab_focus
		add vbox
		show_all
	end
end

class Gtk_Ui < Gtk_Window
	def initialize 
		super
		set_default_size 	800, 600
		set_type_hint 		:normal

		signal_connect "destroy" do
			Gtk.main_quit
		end

		@db 			= Db.new ENV['HOME']+"/.bookmarks.db"
		@store 			= Store.new String, String \
						,String, String, String
		@tree 			= make_tree 

		Actions.store_feed 	@db, @store, @tree

		sw 			= Gtk::ScrolledWindow.new nil, nil
		sw.border_width 	= 2
		sw.add 			@tree

		vbox 			= Gtk::Box.new :vertical, 2

		vbox.pack_start 	make_action_box \
						,:expand 	=> false \
						,:fill 		=> false

		vbox.pack_start 	sw 
		add 			vbox

		@tree.set_cursor (@tree.model.iter_first).path, nil, false
		@tree.grab_focus

		signal_connect "key-press-event" do |w, e|
			key = "#{Gdk::Keyval.to_name e.keyval}"

			if e.state.control_mask?
				if key == "a" 
					@tree.expand_all
				end

				if key == "x"
					@tree.collapse_all
				end

				if key == "j"
					@tree.move_cursor 	:display_lines, 1 
					cursor, column 		= @tree.cursor
					@tree.set_cursor 	cursor, nil, false
				end

				if key == "k" 
					@tree.move_cursor 	:display_lines, -1 
					cursor, column 		= @tree.cursor
					@tree.set_cursor 	cursor, nil, false
				end

				if key == "g"
					@tree.move_cursor 	:buffer_ends, -1
					cursor, column 		= @tree.cursor
					@tree.set_cursor 	cursor, nil, false
				end

				if key == "G"
					@tree.move_cursor 	:buffer_ends, 1
					cursor, column 		= @tree.cursor
					@tree.set_cursor 	cursor, nil, false
				end

				if key == "b"
					@tree.move_cursor 	:pages, 1
					cursor, column 		= @tree.cursor
					@tree.set_cursor 	cursor, nil, false
				end

				if key == "B"
					@tree.move_cursor 	:pages, -1
					cursor, column 		= @tree.cursor
					@tree.set_cursor 	cursor, nil, false
				end

				if key == "h" or key == "a"
					if sel = @tree.selection.selected
						@tree.scroll_to_cell \
							sel.path \
							,(@tree.get_column 0) \
							,false \
							,0.0 \
							,0.0
					end
				end

				if key == "l" or key == "e"
					if sel = @tree.selection.selected
						@tree.scroll_to_cell \
							sel.path \
							,(@tree.get_column 4) \
							,false \
							,0.0 \
							,0.0
					end
				end
			end
		end

		show_all
	end

	def make_tree_menu 
		menu_item_copy 			= Gtk::MenuItem.new "Copy"
		menu_item_copy.signal_connect "activate" do
			Actions.clipboard 	@store, @tree
		end
		
		menu_item_insert 		= Gtk::MenuItem.new "Insert"
		menu_item_insert.signal_connect "activate" do
			Actions.insert 		@db, @store, @tree
		end

		menu_item_delete 		= Gtk::MenuItem.new "Delete"
		menu_item_delete.signal_connect "activate" do
			Actions.delete 		@db, @store, @tree
		end

		menu_item_edit 			= Gtk::MenuItem.new "Edit"
		menu_item_edit.signal_connect "activate" do
			Actions.edit 		@db, @store, @tree
		end

		menu_item_fetch 		= Gtk::MenuItem.new "Fetch"
		menu_item_fetch.signal_connect "activate" do
			Actions.store_feed 	@db, @store, @tree
		end

		menu 			= Gtk::Menu.new

		menu.append 		menu_item_copy
		menu.append 		menu_item_insert
		menu.append 		menu_item_edit
		menu.append 		menu_item_delete
		menu.show_all
		menu
	end

	def make_tree 
		tree			= Gtk::TreeView.new @store
		render 			= Gtk::CellRendererText.new

		tree.insert_column 	0, "tag",  	render, {:text => 3}
		tree.insert_column 	1, "id",   	render, {:text => 0}
		tree.insert_column 	2, "name", 	render, {:text => 1}
		tree.insert_column 	3, "url",  	render, {:text => 2}
		tree.insert_column 	4, "comment",  	render, {:text => 4}

		tree.search_column 	= 1
		tree.show_expanders 	= false

		tree.signal_connect "row-activated" do |tree, path, col|
			if path.depth <= 1
				if tree.row_expanded? path
					tree.collapse_row path
				else
					tree.expand_row path, nil
				end
			else
				Actions.clipboard @store, @tree
			end
		end

		menu			= make_tree_menu

		tree.signal_connect "button-press-event" do |tree, e|
			if e.kind_of? Gdk::EventButton and e.button == 3
				menu.popup nil, nil, e.button, e.time
			end
		end

		tree.signal_connect "popup_menu" do
			menu.popup nil, nil, 0, Gdk::Event::CURRENT_TIME
		end

		tree
	end

	def make_action_box 
		insert_button 			= Gtk::Button.new :label => "_Insert"
		insert_button.use_underline 	= true
		insert_button.signal_connect "clicked" do
			Actions.insert 		@db, @store, @tree
		end

		edit_button 			= Gtk::Button.new :label => "_Edit"
		edit_button.use_underline 	= true
		edit_button.signal_connect "clicked" do
			Actions.edit 		@db, @store, @tree
		end

		delete_button 			= Gtk::Button.new :label => "_Delete"
		delete_button.use_underline 	= true
		delete_button.signal_connect "clicked" do
			Actions.delete 		@db, @store, @tree
		end

		fetch_button 			= Gtk::Button.new :label => "_Fetch"
		fetch_button.use_underline 	= true
		fetch_button.signal_connect "clicked" do
			Actions.store_feed 	@db, @store, @tree
		end

		action_box 		= Gtk::Box.new :horizontal, 2
		action_box.pack_start 	insert_button
		action_box.pack_start 	edit_button 
		action_box.pack_start 	delete_button 
		action_box.pack_start 	fetch_button 
		action_box
	end
end

Gtk.init
Gtk_Ui.new
Gtk.main

