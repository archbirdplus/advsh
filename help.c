#include <stdio.h>
#include "defs.h"
#include "functs.h"

static char	*help_intro[] = {
	"",
	"   Oh, come on.  You know there isn't any help in UNIX",
	"for people who don't know what they're doing.  You're",
	"just expected to learn systems by osmosis...  All right,",
	"all right, I'll give you a little help.  Sigh.",
	"",
	"   You're in a shell reminiscent of your normal UNIX shell",
	"(either the c-shell, or for Real Men, the Bourne shell).",
	"Whenever you see the question mark prompt, you may type an",
	"ash (ADVenture SHell) command, not a UNIX command.  To use",
	"a normal UNIX command, you have to have a magic items whose",
	"property that is, or to be wielding an executable program.",
	NULL
};

static char	*help_general[] = {
	"",
	"Here is the complete list of advsh commands.  You can type",
	"``help <command>'' for more information on a particular",
	"command.  Note that ``?'' is a synonym for ``help''.",
	"",
	NULL
};

/*
 *  Print out the short help message.
 */

dohelp(subject)
	char	*subject;
{
	static int	introduced = 0;
	register int	i;

	if ( !introduced ) {
		for ( i = 0; help_intro[i]; i++ )
			puts(help_intro[i]);
		introduced = 1;
	}

	if ( subject == NULL || *subject == '\0' ) {
		for ( i = 0; help_general[i]; i++ )
			puts(help_general[i]);
		for ( i = 0; findex[i].f_commd; i++) {
			if ( i > 0 && i % 6 == 0 )
				putchar('\n');
			printf("%-11s", findex[i].f_commd);
		}
		putchar('\n');
		putchar('\n');
	} else {
		for ( i = 0; findex[i].f_commd; i++)
			if ( !strcmp(findex[i].f_commd, subject) )
				break;
		if ( findex[i].f_commd == NULL ) {
			error(
		    "No command called \"%s\", try ``help'' for a listing.",
			    subject);
			return;
		}
		switch ( findex[i].f_atype ) {
		case NONE:
			printf("Command \"%s\" takes no arguments.\n",
			     findex[i].f_commd);
			break;
		case ARRAY:
			printf("Command \"%s\" takes several arguments.\n",
			     findex[i].f_commd);
			break;
		case STRING:
			printf("Command \"%s\" takes a single argument.\n",
			     findex[i].f_commd);
			break;
		case INT:
			printf("Command \"%s\" takes an integer argument.\n",
			     findex[i].f_commd);
			break;
		}
		printf("%s\n\n", findex[i].f_help);
	}

	fflush(stdout);
}
