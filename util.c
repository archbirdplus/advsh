#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/dir.h>

#include "defs.h"
#include "types.h"

extern PLAYER	me;

/*
 *  Return a pointer to an allocated area or, if calloc()
 *  returns NULL, die horribly.
 */

char *
alloc(n)
{
	char	*p, *calloc();

	if ( (p = calloc(1, n)) == NULL )
		panic("calloc: No more free memory!");
	else
		return(p);

	/* NOTREACHED */
}


/*
 *  Read a yes of no answer form the terminal.
 *  Return, true if the first letter of response is 'y'
 *  or 'Y' and false otherwise.
 */

ask()
{
	char line[10];

	rewind(stdin);
	fgets(line, sizeof line, stdin);

	if ( isupper(line[0]) )
		line[0] = tolower(line[0]);

	rewind(stdin);
	return( line[0] == 'y' );
}


/*
 *  Looks to see if the file specified is in the current
 *  directory.
 */

indir(file)
	char *file;
{
	DIR *dirp;
	struct direct *ent;

	if ( (dirp = opendir(".")) == NULL )
		return(0);
	
	while ( ent = readdir(dirp) )
		if ( !strcmp(ent->d_name, file) ) {
			closedir(dirp);
			return(1);
		}
	
	closedir(dirp);
	return(0);
}


/*
 *  It the current player the author?
 *  We do this by name instead of UID to make it machine-independant.
 *  At least, I hope I have all those accounts.  :-)
 */

author()
{
	return( !strcmp(me.pl_who.pw_name, "john") );
}
