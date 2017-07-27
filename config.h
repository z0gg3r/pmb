struct file_option {
	int 	size;
	int 	position;
	char* 	option;
};

typedef struct file_option file_option;

/* -- read option file */
/* char* filename */
char**
read_config(char*);

