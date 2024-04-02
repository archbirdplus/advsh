#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>

#include "defs.h"
#include "types.h"
#include "jc/jobs.h"

extern PLAYER 	me;


/*
 *  Hit a file (presumably) with the "weapon" we are wielding.
 *  actually, the things 'hit' may be anything one would like
 *  as arguments to the command.
 */

dohit(args)
	char 	*args[];
{
	if ( me.pl_weapon == NULL ) {
		puts("You are not wielding anything!");
		return;
	}

	args[0] = me.pl_weapon->it_path;
	jcexec(args, JC_FOREGROUND, -1, -1, -1);
}
