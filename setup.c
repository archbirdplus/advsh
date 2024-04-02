#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/file.h>

#include "defs.h"
#include "types.h"
#include "hgets/hist.h"

#define		MODE	0700

extern PLAYER 	me;		/* all info on player */
char   		tmpdir[50];	/* /tmp save directory */
HQUEUE		*histlist;	/* list of past history commands */
extern int	debug;
char		*LOGFILE = "#ash.log";

/*
 *  Set up assorted stuffs for the shell.
 */

extern FILE	*_logfp;

setup(fast)
{
	/* error package setup */
	if ( debug ) {
		if ( (_logfp = fopen(LOGFILE, "a")) == NULL )
			jcerror("setup: Can't fopen %s", LOGFILE);
	} else
		_logfp = NULL;
		
	/* startup message */
	log("startup: inializing for ash ...");

	/* ignore signals while we're working */
	sigign(SIGINT);
	sigign(SIGQUIT);

  	/* set random seed */
	srand( time(NULL)^getpid() );

	/* zero score keeper */
	me.pl_wealth = 0;

	/* initialize job control stuff */
	jcinit("ASH -- top level");

	/* initialize PLAYER struct */
	setstruct();

	if ( !fast ) {
		setupfile();
		welcome();
	}

	/* set up history stuff */
	sethist(fast);
}


/*
 *  Set up random parts of the PLAYER struct.
 */

setstruct()
{
	char		line[BUFSIZ], *alloc(), *get_var();
	struct passwd	*pwptr;

	/* setup current directory */
	getwd(line);
	me.pl_cwd = alloc(strlen(line) + 1);
	strcpy(me.pl_cwd, line);

	/* get the poop on the user */
	if ( (pwptr = getpwuid(getuid())) == NULL ) {
		puts("Can't find you in passwd file.\n");
		exit(-1);
	}
	bcopy(pwptr, &me.pl_who, sizeof me.pl_who);

	/* set up some basic aliases and variables */
	me.pl_list	= NULL;
	me.pl_aliases 	= NULL;
	me.pl_vars	= NULL;
	set_var("prompt", "? ");
	sprintf(line, "/usr/spool/mail/%s", me.pl_who.pw_name);
	set_var("mail", line);

	/* create a save diectorty in /tmp */
	sprintf(tmpdir, "/tmp/ash.%05.5d", getuid());
	if ( !access(tmpdir, F_OK) )
		if ( rmdir(tmpdir) )
			jcerror("setstruct: Can't rmdir %s", tmpdir);

	if ( mkdir(tmpdir, MODE) < 0 )
		error("setstruct: Can't mkdir %s", tmpdir);

	if ( get_var("name") )
		strncpy(me.pl_name, get_var("name"), PL_NAMLEN);
	else
		strcpy(me.pl_name, me.pl_who.pw_name);
	setenv("NAME", me.pl_name);

	/* set player stats */
	me.pl_str    = STRENGTH;
	newstr();
	me.pl_curwht = 0;
	me.pl_weapon = NULL;
	me.pl_inven  = NULL;
	me.pl_blind  = 0;

	/* create item names */
	set_names();

	/* set item list (for directory) */
	setitems(NULL);
}


/*
 *  Set up the history mechanisms (based on the history
 *  variable if it exists).
 */

#define DEF_HNUM	20

sethist(fast)
{
	int	hnum;
	char	*p, *get_var();

	if ( fast )
		hnum = 0;
	else {
		if ( (p = get_var("history")) == NULL )
			hnum = DEF_HNUM;
		else
			if ( sscanf(p, "%d", &hnum) < 0 )
				hnum = DEF_HNUM;
	}
	if ( hnum < 0 )
		hnum = DEF_HNUM;
	
	histlist = hqinit(hnum);
}


/*
 *  Welcome prints out a little welcome message.
 */

welcome()
{
	char	*getenv();
	char	line[BUFSIZ];
	char	*name;

	/* we are almost ready to start */
	if ( !getuid() ) {
		/* root running the program */
		puts("Welcome to the ADVenture SHell, O most exalted one!");
	} else {
		if ((name = get_var("name")) == NULL) {
			if ((name = getenv("NAME")) == NULL)
				name = me.pl_name;
			set_var("name", name);
		}
		printf("\nWelcome to the ADVenture SHell, %s!\n\n", name);
	}

	getwd(line);
	printf("You find yourself in %s ...\n", line);
}
