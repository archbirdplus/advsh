#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/file.h>

#include "defs.h"
#include "types.h"
#include "jc/jobs.h"

char	*USAGE = "usage: %s [ -f ]";

PLAYER 		me;		/* all the info on the current player */
extern char 	tmpdir[];	/* /tmp directory where we save files */
extern int 	errno;
int		debug = 0;	/* debugging mode */

int	fflag = 0;	/* fast startup flag */

main(argc, argv)
	char	*argv[];
{
	int	i, j;

	/* standard argument processing */
	for ( i = 1; i < argc; i++ )
		if ( argv[i][0] == '-' )
			for ( j = 1; argv[i][j]; j++ )
				switch ( argv[i][j] ) {
					case 'f':  /* fast startup */
						fflag++;
						break;
					case 'd':  /* debugging */
						debug++;
						break;
					default:  /* unknown option */
						panic(USAGE, argv[0]);
				}
		else
			panic(USAGE, argv[0]);

	/* initialize the files and data structures */
	setup(fflag);

	/* set up signals */
	sigs();

	/* run the shell */
	shell();

	/* bye, bye */
	cleanup(0);
}


/*
 *  called when user types "exit".
 */

doexit()
{
	puts("Bye, bye.");

	cleanup(0);
}


/*
 *  Cleanup tries to get everything back into shape at exit time,
 *  clearing out the backup directory in /tmp.
 */

cleanup(signo)
{
	DIR     	*dirp;
	struct  direct	*ent;
	char      	name[50];

	jcend(JC_LEAVERUN, SIGHUP);	/* kill all stopped processes */

	/* clean up temp directory */
	dirp = opendir(tmpdir);
	if ( dirp == NULL ) {
		jcperror("cleanup: Can't opendir %s", tmpdir);
		if ( rmdir(tmpdir) )
			jcperror("cleanup: Can't rmdir %s", tmpdir);
		exit(errno);
	}

	while ( ent = readdir(dirp) ) {
		if ( !strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..") )
			continue;
		if ( ent->d_name[0] ) {
			sprintf(name, "%s/%s", tmpdir, ent->d_name);
			if ( unlink(name) )
				jcperror("cleanup: Can't unlink %s", name);
		}
	}
	closedir(dirp);
	if ( rmdir(tmpdir) )
		jcperror("cleanup: Can't rmdir %s", tmpdir);

	if ( signo == 0 )  /* we were not called by a signal */
		doscores();

	exit(0);
}


/*
 *  Dosuspend stops the shell, resetting the TSTP
 *  signal on the way in and out.
 */

dosuspend()
{
	if ( isloginsh() ) {
		puts("Suspend a login shell?!?");
		return;
	}

	sigdfl(SIGTSTP);		/* reset default action */
	sigrelse(SIGTSTP);		/* unblock signal */
	kill(getpid(), SIGTSTP);	/* kill ourselves */

	/* here we are stopped */

	signorm(SIGTSTP);		/* reset signal to normal state */

	puts("\nWell, glad to have you back!");
}
