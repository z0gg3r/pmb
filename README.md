# poor-man-s-bookmark

pmb is a bookmark management tool for those who want more power and flexibility
in bookmark management.

A bookmark inside pmb will have 5 fields:
	id		<- unique identifier
	name		<- name of the website
	url		<- url of the website
	comment		<- a comment about it [optional]
	tag		<- a tag [optional]

Tags are used to group bookmarks together, then you can operate on groups of
bookmarks easily.

Examples:
	Add a bookmark:
		pmb -a name=example,url="http://example.com",comment="my example",tag=ex

		Will add bookmark to the database with:
			name = example
			url = http://example.com
			comment = my example
			tag = ex

		comment and tag can be ommited, they be set to 'none' on database
		if you do not specify any value.

	Printing.
		pmb -p all

		Will print all bookmarks.

		pmb -p i=10

		Will print bookmark who have id 10;

		pmb -p=10,field=url

		Will print the field url of the bookmark
		who have id 10.

	Search by database by name, url, comment and tag.
		pmb -s name=linux 

		Will search all database for bookmarks who  have
		linux in name and print it, same for other field's.
		If you do not specify a field, it will search all
		fields:

		pmb -s linux	

	Exporting the search result.
		pmb -s linux,e=xyz.db

		Will export the result of the search to the database
		xyz.db (if xyz.db do not exist, it will be created).

	Deleting the search result.
		pmb -s linux,d
		pmb -s name=linux,d
		...

		Will delete the result of the search from database.

	Edit bookmarks by id or field.
		pmb -e id=10,tag="new value"

		Will set tag of the bookmark who have id number 10
		to "new value".

		pmb -e f=tag,value="old value",new-value="new value"

		Will set tag of the bookmarks who have tag with 
		value "old tag" to value "new value".
		
	Deleting.
		pmb -d id=10

		Wll delete bookmark who have id 10.

		pmb -d tag=value

		Will delete any bookmark tagged with "value"

		pmb -d tag=value,g

		Will delete any bookmark that contain the word
		"value" in the tag field.
			
	Colored output.
		pmb -c -p all
		pmb -s linux -c

		Adding -c will make search' output and print output
		colored.	

	And more =D

TODO:
	* Implement config file.
	* Gtk interface.
