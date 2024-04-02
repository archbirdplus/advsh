#include <stdio.h>
#include <ctype.h>

#include "defs.h"
#include "hgets/hist.h"

extern HQUEUE	*histlist;

dohist(num)
	char	*num;
{
	HIST	*tmp;
	int	i, n, line;
	char	argstr[BUFSIZ];

	n =  (num && isdigit(*num)) ? atoi(num) : atoi(get_var("history"));

	if ( n <= 0 )
		return;
	
	if ( n > histlist->hq_count )
		n = histlist->hq_count;
	
	line = histlist->hq_line + 1;	/* start at last process */
	for ( tmp = histlist->hq_head; tmp && n-- > 0; tmp = tmp->hi_next ) {
		line--;

		/* make a string out of the args */
		argstr[0] = '\0';
		for ( i = 0; i < tmp->hi_argc; i++ ) {
			strcat(argstr, tmp->hi_argv[i]);
			strcat(argstr, " ");
		}

		printf("%4d  %s\n", line, argstr);
	}
}
