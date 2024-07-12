#include <stdio.h>
#include "defs.h"
#include "types.h"

extern PLAYER 	me;

set_var(var, str)
	char 	*var, *str;
{
	VARS 	*tmp;

	tmp = me.pl_vars;
	while ( tmp ) {
		if ( !strcmp(tmp->vr_name, var) )
			break;
		tmp = tmp->vr_next;
	}

	if ( tmp == NULL ) {  /* a new variable */
		tmp = (VARS *) alloc(sizeof(VARS));
		tmp->vr_name = alloc(strlen(var) + 1);
		strcpy(tmp->vr_name, var);

		tmp->vr_next = me.pl_vars;
		me.pl_vars = tmp;
	}
	else {  /* the variable just needs to be changed */
		if ( tmp->vr_val )
			cfree(tmp->vr_val);
	}

	/* set the variable value field */
	if ( str == NULL )
		tmp->vr_val = NULL;
	else {
		tmp->vr_val = alloc(strlen(str) + 1);
		strcpy(tmp->vr_val, str);
	}
}


char *
get_var(var)
	char 	*var;
{
	VARS 	*tmp;

	if ( var == NULL ) {  /* list all variables */
		for ( tmp = me.pl_vars; tmp; tmp = tmp->vr_next ) {
			printf("%s\t", tmp->vr_name);
			if ( tmp->vr_val )  /* there is a value */
				printf("= \"%s\"\n", tmp->vr_val);
			else
				putchar('\n');
		}
		return(NULL);
	}

	/* look for the variable in our list */
	for ( tmp = me.pl_vars; tmp; tmp = tmp->vr_next )
		if ( !strcmp(var, tmp->vr_name) )
			break;
	
	if ( tmp )  /* valid varible */
		if ( tmp->vr_val )  /* a valued variable */
			return(tmp->vr_val);  /* the value of the variable */
		else  /* no value, return something */
			return(" ");  /* some place-holder */
	else
		return(NULL);
}


/*
 *  Set a variable to a string.
 */

doset(args)
	char 	*args[];
{
	char 	line[200];
	int  	i;

	if ( args[1] == NULL ) {
		get_var(NULL);
		return;
	}

	line[0] = '\0';
	for ( i = 2; args[i] != NULL; i++ ) {
		strcat(line, args[i]);
		strcat(line, " ");
	}
	if ( strlen(line) > 1 ) {  /* there is a line */
		line[strlen(line) - 1] = '\0';   /* get rid of trailing space */
		set_var(args[1], line);
	}
	else
		set_var(args[1], NULL);
}


/*
 *  Throw away a variable name.
 */

dounset(args)
	char 	*args[];
{
	VARS 	*tmp, *last;
	int  	i;

	if ( args[1] == NULL ) {
		puts("Unset what?");
		return;
	}

	for ( i = 1; args[i]; i++ ) {
		tmp = me.pl_vars;
		last = NULL;
		while ( tmp ) {
			if ( !strcmp(tmp->vr_name, args[i]) ) {
				if ( last == NULL )
					me.pl_vars = tmp->vr_next;
				else
					last->vr_next = tmp->vr_next;
				cfree(tmp);
				break;
			}
			last = tmp;
			tmp = tmp->vr_next;
		}
	}
}
