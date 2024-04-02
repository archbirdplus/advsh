#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/dir.h>
#include <sys/stat.h>

#include "defs.h"
#include "types.h"

extern PLAYER 	me;

dolook(dir)
	char 	*dir;
{
	struct stat	stbuf;
	char		*dirname;

	if ( amblind() ) {  /* suffering blindness */
		puts("There is a cloak of darkness upon you.");
		return;
	}

	dirname = dir ? dir : ".";
	
	if ( stat(dirname, &stbuf) < 0 ) {
		puts("I don't see that passage!");
		return;
	}
	if ( (stbuf.st_mode & S_IFMT) != S_IFDIR ) {
		puts("That's not a passageway!");
		return;
	}

	if ( access(dirname, R_OK) ) {   /* we can't read the directory */
		puts("The room is dark to you.");
		return;
	}

	list_dir(dirname);
}


/*
 *  Returns list of items if there are any items 'in' the specified
 *  directory, NULL otherwise.
 */

ITEM *
areitems(dir)
	char	*dir;
{
	LIST	*list;
	char	line[BUFSIZ];

	if ( dir == NULL ) {  /* use current directory */
		getwd(line);
		dir = line;
	}

	for ( list = me.pl_list; list; list = list->ls_next )
		if ( !strcmp(list->ls_dir, dir) )
			break;
	
	if ( list == NULL ) {  /* not found, add this directory */
		log("areitems: '%s' not in list, adding it.", dir);
		setitems(dir);
		list = me.pl_list;  /* setitems puts last one on top */
	}

	return( list->ls_list );  /* magic items? */
}


/*
 *  Print out list of magic items in the current directory, 
 *  using '?' for scrolls, '!' for potions, and '/' for wands.
 */

print_items(dir)
	char	*dir;
{
	ITEM	*tmp;

	for ( tmp = areitems(dir); tmp; tmp = tmp->it_next ) {
		switch ( tmp->it_type ) {
			case OBJ_POTION: /* potion */
				fputs("  !", stdout);
				break;
			case OBJ_SCROLL: /* scroll */
				fputs("  ?", stdout);
				break;
			case OBJ_WAND: /* wand */
				fputs("  /", stdout);
				break;
		}
	}

	/* down to next line */
	putchar('\n');
}


/*
 *  List the files, directories, and magic items
 *  in the directory 'dir'.
 */

/* the list of files and directories to print out */
#define		MAX_DIRS	100
#define  	MAX_FILES    	200
#define  	NAME_LEN     	15
char 		dnames[MAX_DIRS][NAME_LEN];
char 		fnames[MAX_FILES][NAME_LEN];

list_dir(dir)
	char 	*dir;
{
	DIR 		*dirp;
	struct direct 	*ent;
	struct stat 	stbuf;
	char 		name[BUFSIZ];
	int		d, f;

	if ( (dirp = opendir(dir)) == NULL ) {
		jcerror("list_dir: Can't opendir %s", dir);
		return;
	}

	strcpy(dnames, "..");
	for ( d = 1, f = 0; ent = readdir(dirp); ) {
		if ( ent->d_name[0] == '\0' || ent->d_name[0] == '.' )
			continue;

		sprintf(name, "%s/%s", dir, ent->d_name);
		if ( stat(name, &stbuf) < 0 ) {
			jcerror("list_dir: stat %s", name);
			continue;
		}

		/* add the name of the file to the right array */
		if ( stbuf.st_mode & S_IFDIR ) {  /* add to list of dirs */
			if ( d < MAX_DIRS )
				strncpy(dnames[d++], ent->d_name, NAME_LEN);
		}
		else {  /* add to list of files */
			if ( f < MAX_FILES )
				strncpy(fnames[f++], ent->d_name, NAME_LEN);
		}
	}
	closedir(dirp);

	/* create pathname of directory to look into */
	if ( !strcmp(dir, ".") )  /* current directory */
		strcpy(name, me.pl_cwd);
	else  /* another directory off the current one */
		sprintf(name, "%s/%s", me.pl_cwd, dir);

	if ( f > 0  ||  areitems(name) ) {  /* are files or magic items */
		fputs("You see: ", stdout);
		print_items(name);
		if ( f > 0 )
			print_list(fnames, f);
	}

	if ( d > 1 ) {  /* print names of other directories */
		puts("There are exits labeled:");
		print_list(dnames, d);
	}
	else
		puts("The only exit is '..'");
}


/*
 *  Print out the list of filenames in an columnated
 *  format, after sorting.
 */

print_list(names, n)
	char	names[][NAME_LEN];
{
	int 	i, strcmp();

	qsort(names, n, NAME_LEN, strcmp);

	for ( i = 0; i < n; i++ ) {
		if ( i % 5 == 0  &&  i > 0 )  /* time for a newline */
			putchar('\n');
		printf("%-*s ", NAME_LEN-1, names[i]);
	}
	putchar('\n');
}


/*
 *  Returns 1 if we are blinded at this time; 0 if not.
 */

long 	blind_time = 0;  /* the time we went blind */
#define	SECONDS    60

amblind()
{
	long now;

	if ( me.pl_blind == 0 )
		return(0);

	time(&now);
	if ( now - (me.pl_blind * SECONDS) >= blind_time ) {
		me.pl_blind = 0;
		return(0);
	}
	return(1);
}


/*
 *  Set the me.pl_blind variable for a random number of
 *  minutes to stay blind.
 */

blindme()
{
	time(&blind_time);

	puts("Everything goes dark.");
	me.pl_blind = random(1, 3);  /* minutes of blindness */
}
