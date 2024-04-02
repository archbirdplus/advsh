#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>

#include "defs.h"
#include "types.h"


/*
 *  Move (cd) to another directory.
 */

extern PLAYER	me;

domove(dir)
	char 	*dir;
{
	struct stat 	stbuf;
	char 		line[100], *alloc();

	if ( !dir || !*dir ) {
		puts("Move where?\n");
		return;
	}

	if ( !indir(dir) || stat(dir, &stbuf) ) {
		puts("I don't see a door that way!");
		return;
	}

	if ( !(stbuf.st_mode & S_IFDIR) ) {  /* not a directory */
		puts("That's not a passageway!");
		return;
	}
	
	if ( access(dir, X_OK) ) {  /* no permission */
		puts("There is a locked door in your way.");
		return;
	}

	if ( !changedir(dir) )
		printf("You are now in %s ...\n", me.pl_cwd);
}


/*
 *  Actually move to the other direcory and do the updating stuff.
 */

changedir(dir)
	char	*dir;
{
	char	line[BUFSIZ], *alloc();

	if ( chdir(dir) ) {
		puts("An invisible force blocks your way.");
		return(-1);
	}
	else {  /* everything worked */
		setitems(NULL);
		getwd(line);
		if ( me.pl_cwd )
			cfree(me.pl_cwd);
		me.pl_cwd = alloc(strlen(line) + 1);
		strcpy(me.pl_cwd, line);
		return(0);
	}
}


/*
 *  Move to the user's home direcotory.
 */

dohome()
{
	changedir(me.pl_who.pw_dir);
	puts("You find yourself back in your home.");
}
