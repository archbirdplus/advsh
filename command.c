#include <stdio.h>
#include <ctype.h>
#include <sys/file.h>

#include "defs.h"
#include "types.h"
#include "functs.h"
#include "cmdlist.h"

extern PLAYER 	me;


/*
 *  Take the input line and execute the right
 *  command, after parsing.
 */

command(line)
	char 	*line;
{
	char 	*args[MAXARGS], *get_var();
	int  	i;

	/* if there is an error, parse will bitch */
	if ( parse(line, args) == -1 )
		return;

	if ( get_var("verbose") || get_var("echo") ) {  /* echo parsed line */
		for ( i = 0; args[i]; i++ ) {
			fputs(args[i], stdout);
			putchar(' ');
		}
		putchar('\n');
	}

	if ( docall(args) )  /* we know one by that name */
		puts("I don't understand that.");
	
	/* free up the space used for args by parse */
	for ( i = 0; args[i]; i++ )
		cfree(args[i]);
}


/*
 *  Call the function ascociated with the command given
 *  as the argument.  Return 0 if successfull or -1 if
 *  not.
 */

docall(args)
	char	*args[];
{
	int	i;
	
	for ( i = 0; findex[i].f_commd; i++ )
		if ( !strcmp(findex[i].f_commd, args[0]) )
			break;

	if ( findex[i].f_commd == NULL )
		return(-1);

	callfunct(findex[i].f_funct, args, findex[i].f_atype);

	return(0);
}


/*
 *  Call the given function with the correct number 
 *  and type of arguments.  If the function takes a
 *  string or integer as an argument, and the user has
 *  specified more than a single argument, we call the
 *  function repeatedly, with a representation of 
 *  each argument.
 */

callfunct(funct, args, atype)
	int	(*funct)();
	char	*args[];
{
	int	i;

	if ( atype == ARRAY ) {		/* argv (char **) array */
		funct(args);
	}
	else if ( atype == STRING ) {	/* string (char *) */
		if ( args[1] )
			for ( i = 1; args[i]; i++ )
				funct(args[i]);
		else
			funct(NULL);
	}
	else if ( atype == INT ) {	/* integer (status) */
		if ( args[1] )
			for ( i = 1; args[i]; i++ )
				if ( isdigit(*args[i]) )
					funct(atoi(args[i]));
				else
					funct(0);
		else
			funct(0);
	}
	else				/* no arguments at all */
		funct();
}
