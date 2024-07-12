#include <stdio.h>
#include <setjmp.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "types.h"
#include "defs.h"
#include "hgets/hist.h"

#define  MAIL_CHECK    5

extern PLAYER	me;
jmp_buf		jumpenv;
extern HQUEUE	*histlist;
extern int	h_hist;


/*
 *  Loop here reading commands until end-of-file.
 */

shell()
{
	char 	*line;
	int  	i, n = 0;

	if ( !isloginsh() )  /* don't check mail if login already has */
		mailcheck();
	
	for (;;) {
		if ( ++n % MAIL_CHECK == 0 )  /* check mail file */
			mailcheck();

		jccheck();  /* look at background stopped/running processes */

		if ( setjmp(jumpenv) )
			putchar('\n');

		printprompt();
		if ( hgets(&line, histlist, stdin) != H_NOERROR ) {
			if ( h_errno == H_EMPTY )	/* no input line */
				continue;
			else if ( h_errno == H_EOF ) {	/* end of file */
				if ( get_var("ignoreeof") == NULL ) {
					putchar('\n');
					doexit(NULL);
				}
				puts("^D\nUse 'exit' to leave shell.");
				rewind(stdin);
			} else				/* bad error */
				herror(NULL);
			continue;
		}
		if ( h_hist ) {  /* echo history substituted line */
			for ( i = 0; i < histlist->hq_head->hi_argc; i++ )
				printf("%s ", histlist->hq_head->hi_argv[i]);
			putchar('\n');
			fflush(stdout);
		}

		if ( line[0] != '\0' )
			command(line);
	}
}


/*
 *  Print out the prompt string, substituting for the 
 *  ! the event number if appropriate.
 */

printprompt()
{
	char	*p;
	int	num;

	if ( (p = get_var("prompt")) == NULL )	/* no prompt */
		return;
	
	for ( ; *p; p++ )
		if ( *p == '\\'  &&  *(p+1) )	/* escaped character */
			putchar(*++p);
		else if ( *p == '!' )		/* exclam (event number) */
			printf("%d", histlist->hq_line+1);
		else				/* normal character */
			putchar(*p);
}


/*
 *  Interrupts are caught here so we can do a longjmp
 *  back to the shell input loop.
 */

/* ARGSUSED */
interrupt(signo)
{
	rewind(stdout);  /* throw away pending output */
	rewind(stdin);  /* throw away pending input */
	longjmp(jumpenv, 1);
}


/*
 *  Check to see if the size of the user's mailbox has
 *  changed.
 */

char *mail_names[] = {
	"ring",
	"chain",
	"splint",
	"banded",
	"plate",
	NULL
};

mailcheck()
{
	static long 	mailsize = -1;
	struct stat 	stbuf;
	static int  	lev;

	if ( stat(get_var("mail"), &stbuf) )
		return;

	if ( mailsize == -1 ) {
		mailsize = stbuf.st_size;
		lev = 0;
		return;
	}
	if ( mailsize > stbuf.st_size ) {  /* they read the mail */
		mailsize = stbuf.st_size;
		lev = 0;
		return;
	}
	if ( mailsize < stbuf.st_size ) {
		if ( mail_names[++lev] == NULL )
			lev = 0;
		printf("You have %s mail.\n", mail_names[lev]);
		mailsize = stbuf.st_size;
	}
}
