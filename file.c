#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>

#include "defs.h"
#include "types.h"

#define MODE		0644

extern PLAYER 	me;
extern char 	tmpdir[];
extern int	errno;

/*
 *  Remove a file from the current directory.
 *  We save a copy of the file for later restoration.
 */

dodest(args)
	char 	*args[];
{
	char 		name[50];
	struct stat 	stbuf;
	int		pid, sig = SIGQUIT, i;

	if ( args[1] == NULL ) {
		puts("What do you want to destroy??");
		return;
	}

	for ( i = 1; args[i]; i++ ) {
		if ( !indir(args[i]) ) {
			printf("I don't see %s.\n", args[i]);
			return;
		}

		if ( stat(args[i], &stbuf) ) {
			perror("dodest: Can't stat %s", args[i]);
			return;
		}

		if ( access(args[i], R_OK) ) {  /* no write permission */
			printf("You can't hurt %s.\n", args[i]);
			continue;
		}

		sprintf(name, "%s/%s", tmpdir, args[i]);
		if ( copyfile(args[i], name) ) {
			printf("I just could not destroy %s.\n", args[i]);
			continue;
		}

		if ( unlink(args[i]) < 0 )
			printf("%s does not die.\n", args[i]);
		else
			printf("%s dies with a single blow.\n", args[i]);
	}
}


/*
 *  Resurect a destroyed (removed) file;
 *  if we just happen to have a copy in /tmp...
 */

doresu(args)
	char	*args[];
{
	char	tmpfile[BUFSIZ], thisfile[BUFSIZ];
	int	i;

	if ( args[1] == NULL ) {
		puts("What do you want to Resurect?");
		return;
	}

	for ( i = 1; args[i]; i++ ) {
		sprintf(tmpfile, "%s/%s", tmpdir, args[i]);
		if ( access(tmpfile, F_OK) ) {	/* no saved file */
			printf("I don't remember destroying %s!\n", args[i]);
			continue;
		}

		sprintf(thisfile, "./%s", args[i]);
		if ( !access(thisfile, F_OK) ) { /* that file exists in "." */
			printf("%s is already in this room!\n", args[i]);
			continue;
		}

		if ( access(".", W_OK) ) {  /* cannot write in the directory */
			puts("You can't bring a file into this room!");
			continue;
		}

		if ( copyfile(tmpfile, thisfile) )  /* copy file into "." */
			printf("I tried, but %s is too far gone.", args[i]);
		else
			printf("%s appears in the room!\n", args[i]);
	}
}


/*
 *  Copy the file 'from' to the file 'to'.
 */

copyfile(from, to)
	char		*from, *to;
{
	char		buf[BUFSIZ];
	struct stat	stbuf;
	int		in, out, n;

	sighold(SIGINT);	/* we don't want to be interrupted here */
	sighold(SIGQUIT);

	if ( stat(from, &stbuf) < 0 ) {
		perror("copyfile: Can't stat %s", from);
		return(errno);
	}

	if ( (in = open(from, O_RDONLY)) < 0 ) {
		perror("copyfile: Can't open %s", from);
		return(errno);
	}
	if ( (out = open(to, O_CREAT | O_WRONLY, (int) stbuf.st_mode)) < 0 ) {
		perror("copyfile: Can't open %s", to);
		close(in);
		return(errno);
	}
	while ( (n = read(in, buf, BUFSIZ)) > 0 )
		if ( write(out, buf, n) != n ) {
			perror("copyfile: Write error to %s", to);
			return(errno);
		}

	close(in);
	close(out);

	sigrelse(SIGINT);		/* let the signals go, again */
	sigrelse(SIGQUIT);

	return(0);
}
