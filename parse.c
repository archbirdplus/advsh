#include <stdio.h>
#include <ctype.h>

#include "defs.h"
#include "types.h"

extern PLAYER 	me;


/*
 *  Parse the command line into an argv format.
 */

parse(cmd, args)
	char 	*cmd, *args[];
{
	struct passwd 	*pwptr;
	char 		token[BUFSIZ], word[BUFSIZ], str[100];
	char 		*c, *t, *w, *s, *get_var(), *alloc();
	int  		i, r, pid;

	for ( i = 0; (r = next_token(cmd, token)) > 0; i++ ) {
		t = token;
		w = word;
		while ( *t ) {
			switch ( *t ) {
				case '~': /* user's home */
					t++;
					s = str;
					while ( isalpha(*t) )
						*s++ = *t++;
					*s = '\0';

					if ( !str[0] )
						pwptr = &me.pl_who;
					else
						pwptr = getpwnam(str);
					if ( pwptr == NULL ) {
						puts(
					"I can't find that person's home");
						return(-1);
					}

					c = pwptr->pw_dir;
					while ( *c )
						*w++ = *c++;
					break;
				case '$':  /* variable expansion */
					t++;
					s = str;
					while ( isalpha(*t) )
						*s++ = *t++;
					*s = '\0';

					c = get_var(str);
					if ( c == NULL ) {
						puts(
						"I don't know that variable");
						return(-1);
					}

					while ( *c )
						*w++ = *c++;
					break;
				default: /* normal character */
					*w++ = *t++;
					break;
			}
		}

		/* null terminate word */
		*w = '\0';

		/* check if we are refering to a process */
		if ( word[0] == '%' ) {
			if ( (pid = findpid(word)) < 0 )
				return(-1);
			sprintf(word, "%d", pid);
		}

		args[i] = alloc(strlen(word) + 1);
		strcpy(args[i], word);
	}

	/* end argument array */
	args[i] = NULL;

	/* make sure last token did not contain error */
	if ( r )
		return(-1);
	else
		return(0);
}


/*
 *  Return the next token from the line.
 */

next_token(line, token)
	char *line, *token;
{
	ALIAS 		*tmp;
	static char 	*p = NULL;
	char 		*t;

	if ( p == NULL ) {  /* initially for each line */
		p = line;
		while ( *p == ' ' || *p == '\t' )
			p++;

		t = token;
		while ( *p && *p != ' ' && *p != '\t' )
			*t++ = *p++;
		*t = '\0';

		/* do alias expansion on first word */
		tmp = me.pl_aliases;
		while ( tmp ) {  /* look through all aliases */
			if ( !strcmp(tmp->al_name, token) ) {
				strcpy(token, tmp->al_str);
				break;
			}
			tmp = tmp->al_next;
		}
		if ( *p )
		strcat(token, p);
		strcpy(line, token);
		p = line;
	}
	else if ( *p == '\0' ) {  /* done a line */
		p = NULL;
		return(0);
	}
	
	t = token;
	while ( *p ) {
		switch ( *p ) {
			case '\\':  /* literal next */
				*t++ = *++p;
				p++;
				break;
			case '"':  /* quotes */
				p++; /* get past initial " */
				while ( *p && *p != '"' ) {
					if ( *p == '\\' )
						p++;
					*t++ = *p++;
				}
				if ( *p == '\0' && *(p-1) != '"' ) {
					puts("Unmatched \".");
					return(-1);
				}
				break;
			case ' ':
			case '\t':  /* white space */
			case '\n':
				*t = '\0';
				while ( *p && (*p == ' ' || *p == '\t') )
					p++;
				return(t - token);
			default: /* normal character */
				*t++ = *p++;
				break;
		}
	}
	*t = '\0';
	return(t - token);
}
