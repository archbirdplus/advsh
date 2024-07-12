#include <stdio.h>
#include "defs.h"
#include "types.h"

extern PLAYER	me;

/*
 *  When the user types 'alias' we try to figure out what he wants
 *  to see.  This is handled like the csh.  'Alias' prints out a list
 *  of aliases, 'alias word' prints out alias for that word, and
 *  'alias word command' creates a new alias for the command.
 */

doalias(args)
	char 	*args[];
{
	ALIAS 	*tmp, 
		*last;
	char 	line[BUFSIZ];
	int  	i;

	if ( args[1] == NULL ) {	/* just print aliases */
		for ( tmp = me.pl_aliases; tmp; tmp = tmp->al_next )
			printf("%-10s \"%s\"\n", tmp->al_name, tmp->al_str);
		return;
	}
	if ( args[2] == NULL ) {	/* print out the single alias */
		for ( tmp = me.pl_aliases; tmp; tmp = tmp->al_next )
			if ( !strcmp(tmp->al_name, args[1]) ) {
				printf("%-10s \"%s\"\n", 
					tmp->al_name, tmp->al_str);
				return;
			}
		return;
	}

	/* concatenate the commands into one line for storage */
	line[0] = '\0';
	for ( i = 2; args[i]; i++ ) {
		strcat(line, args[i]);
		strcat(line, " ");
	}
	line[strlen(line) - 1] = '\0';  /* zap trailing space */

	/* look to see if the alias already exists */
	for ( tmp = me.pl_aliases; tmp; tmp = tmp->al_next )
		if ( !strcmp(tmp->al_name, args[1]) ) {
			cfree(tmp->al_str);
			tmp->al_str = alloc(strlen(line) + 1);
			strcpy(tmp->al_str, line);
			return;
		}

	last = (ALIAS *) alloc(sizeof(ALIAS));
	last->al_name = alloc(strlen(args[1]) + 1);
	last->al_str  = alloc(strlen(line) + 1);

	strcpy(last->al_name, args[1]);
	strcpy(last->al_str, line);
	last->al_next = me.pl_aliases;
	me.pl_aliases = last;
}

dounalias(args)
	char *args[];
{
	ALIAS *tmp, *last;

	if ( args[1] == NULL ) {
		puts("Unalias wnat?");
		return;
	}

	tmp = me.pl_aliases;
	last = NULL;
	while ( tmp ) {
		if ( !strcmp(tmp->al_name, args[1]) ) {
			if ( last == NULL )
				me.pl_aliases = tmp->al_next;
			else
				last->al_next = tmp->al_next;
			cfree(tmp->al_name);
			cfree(tmp->al_str);
			cfree(tmp);
			return;
		}
		last = tmp;
		tmp = tmp->al_next;
	}
	/* else it was not found */
	printf("You don't have %s aliased!\n", args[1]);
}
