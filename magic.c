#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/dir.h>

#include "defs.h"
#include "types.h"
#include "jc/jobs.h"

extern char 	scrolls_known[], wands_known[], potions_known[];

/*
 *  Teleport (cd) to any directory.
 */

teleport()
{
	char 	line[BUFSIZ], 
		*newargs[MAXARGS];

	fputs("Teleport to: ", stdout);
	gets(line);
	if ( !line[0] ) {
		puts("You blew it!");
		return;
	}
	if ( parse(line, newargs) < 0 )
		return;
	if ( chdir(newargs[0]) )
		printf("Could not get to %s\n", newargs[0]);
	else
		setitems(NULL);
}


/*
 *  Execute an arbitrary command.
 */

manipulate()
{
	char 	line[BUFSIZ], 
		*newargs[MAXARGS];

	fputs("Execute command: ", stdout);
	gets(line);
	if ( !line[0] ) {
		puts("You blew it, dude!");
		return;
	}
	if ( parse(line, newargs) < 0 )
		return;
	jcexec(newargs, JC_FOREGROUND, -1, -1, -1);
}


/*
 *  Identify an item in the user's pack.  (make the name known).
 *  This action has a real effewct only on magic items.
 */

identify()
{
	ITEM 	*obj, 
		*find_item();
	char 	*item_name();

	if ( (obj = find_item(0)) == NULL )
		return;
	item_known(obj);
	printf("It is %s\n", item_name(obj, "a"));
}


/*
 *  The user has found a scroll that lets him see invisible.
 *  That is, all `.' files.
 */

invisible()
{
	DIR 		*dirp, *opendir();
	struct direct 	*ent, *readdir();
	int  		i;

	dirp = opendir(".");
	if ( dirp == NULL ) {
		puts("You feel a sense of loss.");
		return;
	}

	puts("You can see everything around you!");
	for ( i = 1; ent = readdir(dirp); ) {
		if ( ent->d_name[0] != '.' )
			continue;
		printf("%-14s ", ent->d_name);
		if ( i++ % 5 == 0 )
			putchar('\n');
	}

	closedir(dirp);
	putchar('\n');
}	


/*
 *  Lose your memory, that is, forget what everyting is
 *  really called.
 */

memory()
{
	int i;

	puts("You feel dizzy for a moment, Where am I??");
	for ( i = 0; i < ITM_NUMBER; i++ ) {
		scrolls_known[i] = 0;
		wands_known[i] = 0;
		potions_known[i] = 0;
	}
}


/*
 *  Execute a command in the background (hidden).
 */

hidden()
{
	char line[256], *newargs[MAXARGS];

	fputs("What command? ", stdout);
	gets(line);
	if ( !line[0] ) {
		puts("You blew it, man!");
		return;
	}

	if ( parse(line, newargs) < 0 )
		return;
	jcexec(newargs, JC_BACKGROUND, -1, -1, -1);
}


/*
 *  Enchant a file (make it executable even if the user is hot
 *  the owner).
 */

enchant()
{
	ITEM *obj;

	if ( (obj = find_item(OBJ_FILE)) == NULL )
		return;
	
	if ( obj->it_type != OBJ_FILE ) {
		jclog("enchant: object %s not a file.", obj->it_name);
		return;
	}

	obj->it_magic++;
}
