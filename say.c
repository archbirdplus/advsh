#include <stdio.h>

/*
 *  Echo arguments to standard output.
 */

dosay(args)
	char	*args[];
{
	int	i;

	for ( i = 1; args[i]; i++ )
		printf("%s ", args[i]);
	putchar('\n');
}
