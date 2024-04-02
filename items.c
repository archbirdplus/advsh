#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>

#include "defs.h"
#include "types.h"

extern char 	*scroll_names[], *wand_names[], *potion_names[];
extern PLAYER 	me;


/*
 *  Set up the list of items for a directory, if dir is null,
 *  we use current directory.
 *  If it is one we have not yet visited, generate a list
 *  of magic items.  If we have already been here, use old
 *  list of magic items.
 */

setitems(dir)
	char	*dir;
{
	ITEM 		*tmp, *getitem();
	LIST 		*element, *last;
	char 		cur[BUFSIZ], wd[BUFSIZ], *alloc();
	int 		num;

	getwd(cur);  /* save our current directory */

	if ( dir )  /* handed a directory */
		if ( chdir(dir) )
			perror("setitems: Can't chdir to %s", dir);
	getwd(wd); /* get the true pathname */

	/* set magic items for directory */
	last = NULL;
	for ( element = me.pl_list; element; element = element->ls_next ) {
		if ( !strcmp(element->ls_dir, wd) )
			break;
		last = element;
	}
	if ( element == NULL ) {  /* no entry for current directory */
		element = (LIST *) alloc(sizeof(LIST));
		element->ls_dir = alloc(strlen(wd) + 1);
		strcpy(element->ls_dir, wd);
		element->ls_flags = 0;

		element->ls_list = NULL;
		num = random(0, MAX_ITEMS);
		while ( num-- > 0 ) {  /* generate a element of items */
			tmp = getitem();
			tmp->it_next = element->ls_list;
			element->ls_list = tmp;
		}

		element->ls_next = me.pl_list;
		me.pl_list = element;
	}
	else {  /* just bring that element to the head of the element */
		if ( last == NULL )  /* it's already the first element */
			return;
		last->ls_next = element->ls_next;
		element->ls_next = me.pl_list;
		me.pl_list = element;
	}

	if ( chdir(cur) )	/* go back to original directory */
		perror("setitems: Can't chdir back to %s", cur);

	/* 
	 *  Make sure the current directory is at head of list
	 */
	if ( !strcmp(cur, me.pl_list->ls_dir) )
		return;

	last = NULL;
	for ( element = me.pl_list; element; element = element->ls_next ) {
		if ( !strcmp(element->ls_dir, cur) )
			break;
		last = element;
	}
		
	if ( element == NULL ) {  /* should not hapen */
		error("setitems: could not find current directory");
		return;
	}

	last->ls_next = element->ls_next;  /* cut out the element */
	element->ls_next = me.pl_list;
	me.pl_list = element;   /* stick it at the head of the list */
}


/*
 *  Create a random item and return a pointer to it.
 */

ITEM *
getitem()
{
	ITEM 	*tmp;
	char	*alloc();

	tmp          	= (ITEM *) alloc(sizeof(ITEM));
	tmp->it_name 	= alloc(10);
	tmp->it_path 	= NULL;
	tmp->it_num  	= 1;
	tmp->it_letter	= '\0';
	switch ( random(0, 10) ) {
		case 0:
		case 1:
		case 2:
		case 3:  /* generate a scroll */
			strcpy(tmp->it_name, "scroll");
			tmp->it_weight = WHT_SCROLL;
			tmp->it_type = OBJ_SCROLL;
			tmp->it_uses  = USE_SCROLL;
			do  /* get a magic type */
				tmp->it_magic = random(0, (ITM_NUMBER - 1));
			while ( !scroll_names[tmp->it_magic] );
			break;
		case 4:
		case 5:
		case 6:
		case 7:
		case 8: /* generate a potion */
			strcpy(tmp->it_name, "potion");
			tmp->it_weight = WHT_POTION;
			tmp->it_type = OBJ_POTION;
			tmp->it_uses  = USE_POTION;
			do  /* get a magic type */
				tmp->it_magic = random(0, (ITM_NUMBER - 1));
			while ( !potion_names[tmp->it_magic] );
			break;
		case 9:
		case 10: /* generate a wand */
			strcpy(tmp->it_name, "wand");
			tmp->it_weight = WHT_WAND;
			tmp->it_type = OBJ_WAND;
			tmp->it_uses  = USE_WAND;
			do  /* get a magic type */
				tmp->it_magic = random(0, (ITM_NUMBER - 1));
			while ( !wand_names[tmp->it_magic] );
			break;
	}
	return(tmp);
}


/*
 *  Getfilename -- formats an ls -l like listing of a file
 *  from an obj pointer.
 */

getfilename(obj, fname)
	ITEM 	*obj;
	char	*fname;
{
	struct passwd 	*pwptr;
	struct stat 	stbuf;
	char 		*name, mode[15], 
			line[BUFSIZ];

	if ( stat(fname, &stbuf) ) {
		perror("getfilename: Can't stat %s", fname);
		printf("Can't decipher the file %s\n", fname);
		return;
	}

	getmode(stbuf.st_mode, mode);

	pwptr = getpwuid(stbuf.st_uid);
	if ( pwptr == NULL )
		name = "???";
	else
		name = pwptr->pw_name;
	sprintf(line, "%s %s  %d  %s", mode, name, stbuf.st_size, fname);
	obj->it_name = alloc(strlen(line) + 1);
	strcpy(obj->it_name, line);
}


/*
 *  Getmode formats a visual representation of the stat
 *  struct mode bits.
 */

getmode(mode, buf)
	unsigned short 	mode;
	char		buf[];
{
	/* file 'type' letter */
	if ( mode & S_IFREG )		/* regular file */
		buf[0] = '-';
	else if ( mode & S_IFDIR )  	/* directory */
		buf[0]  = 'd';
	else if ( mode & S_IFCHR )	/* charcter special */
		buf[0]  = 'c';
	else if ( mode & S_IFBLK )	/* block special */
		buf[0]  = 'b';
	else if ( mode & S_IFLNK )	/* symbolic link */
		buf[0]  = 'l';
	else if ( mode & S_IFSOCK )	/* socket endpoint */
		buf[0]  = 's';
	else	/* should never happen */
		buf[0]  = '-';

	/* owner permissions */
	buf[1]  = mode & 0400  ? 'r' : '-';
	buf[2]  = mode & 0200  ? 'w' : '-';
	if ( mode & S_ISUID )  /* set user id on execution */
		buf[3]  = 's';
	else
		buf[3]  = mode & 0100  ? 'x' : '-';

	/* group permissions */
	buf[4]  = mode & 0040  ? 'r' : '-';
	buf[5]  = mode & 0020  ? 'w' : '-';
	if ( mode & S_ISGID )  /* set group id on execution */
		buf[6]  = 's';
	else
		buf[6]  = mode & 0010  ? 'x' : '-';

	/* other permissions */
	buf[7]  = mode & 0004  ? 'r' : '-';
	buf[8]  = mode & 0002  ? 'w' : '-';
	if ( mode & S_ISVTX )  /* save swapped text after use */
		buf[9]  = 't';
	else
		buf[9]  = mode & 0001  ? 'x' : '-';

	buf[10] = '\0';
}


/*
 *  Find_item -- returns an item of type `type' from in the
 *  player's pack.  If there are multiple items of that type,
 *  make the player pick one from a menu.
 */

ITEM *
find_item(type)
{
	ITEM 	*obj;
	int  	count;
	char 	line[BUFSIZ];

	count = 0;
	for ( obj = me.pl_inven; obj; obj = obj->it_next )
		if ( type == 0 )  /* anything */
			count++;
		else if ( obj->it_type == type )
			count++;

	if ( count < 1 ) {
		puts("You don't have anything appropriate.");
		return(NULL);
	}
	else if ( count == 1 ) {
		for ( obj = me.pl_inven; obj; obj = obj->it_next ) {
			if ( obj->it_type == type )
				return(obj);
		}
		error("find_item: item %s not found in inventory.",
				obj->it_name);
		return(NULL); /* just in case */
	}
	else {  /* choose one out of the bunch */
		obj = me.pl_inven;
		/* make selection */
		for (;;) {  /* get a choice */
			fputs("Which one (* for list)? ", stdout);
			if ( gets(line) == NULL ) {
				putchar('\n');  /* next line */
				continue;
			}

			if ( line[0] == '\033' || line[0] == '\0' )
				return(NULL);  /* changed their mind */

			/* list inventory */
			if ( line[0] == '*' ) { /* list possible items */
				inv_list(type);
				continue;
			}

			/* check if it's in our inventory */
			for ( obj = me.pl_inven; obj; obj = obj->it_next )
				if ( line[0] == obj->it_letter )
					return(obj);

			/* else it was not found */
			printf("Invalid item '%c'!\n", line[0]);
		}
	}
	/* NOTREACHED */
}


/*
 *  Get an object in our pack from the item letter.
 *  If there is no item with that letter, we bitch and return
 *  NULL.
 */

ITEM *
get_item(letter)
	char	letter;
{
	ITEM	*tmp;

	for ( tmp = me.pl_inven; tmp; tmp = tmp->it_next )
		if ( tmp->it_letter == letter )
			return(tmp);
	
	printf("You don't have an item labeled '%c'\n", letter);
	return(NULL);
}


/*
 *  Ifree -- recursively free a linked list of item pointers.
 */

ifree(ptr)
	ITEM 	*ptr;
{
	if ( ptr->it_next )
		ifree(ptr->it_next);
	
	if ( ptr->it_name )
		free(ptr->it_name);
	if ( ptr->it_path )
		free(ptr->it_path);
	free(ptr);
}
