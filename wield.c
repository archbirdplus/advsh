#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>

#include "defs.h"
#include "types.h"

extern PLAYER 	me;

dowield(name)
	char 	*name;
{
	ITEM 		*tmp, *find_item(), *get_item(), *add_obj();
	struct stat 	stbuf;
	char 		*item_name();

	if ( name == NULL )
		tmp = find_item(OBJ_FILE);
	else
		tmp = get_item(name[0]);

	if ( tmp == NULL )  /* no object */
		return;

	if ( tmp->it_type != OBJ_FILE  || access(tmp->it_path, X_OK) ) {
		puts("That's not a useful weapon!");
		return;
	}

	if ( stat(tmp->it_path, &stbuf) ) {
		jcperror("dowield: Can't stat %s", tmp->it_path);
		return;
	}

	if ( stbuf.st_size <= 0 ) {
		puts("The file is empty.");
		return;
	}

	if ( stbuf.st_uid != getuid() && tmp->it_magic <= 0 ) {
		puts("You don't even own that file!");
		return;
	}

	/* 
	 *  if we got to here, everything is okay
	 */

	if ( me.pl_weapon != NULL ) {
		if ( (tmp = add_obj(me.pl_weapon)) == NULL ) {
			me.pl_weapon->it_next = me.pl_list->ls_list;
			me.pl_list->ls_list = me.pl_weapon;
			printf("There's no room in your pack...\n");
			printf("%s you were wielding falls to the floor.\n", 
				item_name(me.pl_weapon, "The"));
		}
		else
			printf("You put away %s.\n", 
				item_name(me.pl_weapon, "your"));
	}

	printf("You are wielding %s.\n", item_name(tmp, "a"));
	me.pl_weapon = tmp;
	rm_obj(tmp);
}


/*
 *  Move the item fromm being wielded to just being in the user's
 *  pack (put away).  If there is no room in the pack, we drop the
 *  item.
 */

doputaway()
{
	ITEM	*obj;

	if ( me.pl_weapon == NULL ) {
		puts("You aren't wielding anything!");
		return;
	}

	obj = me.pl_weapon;
	me.pl_weapon = NULL;
	if ( add_obj(obj) ) {
		printf("%s falls to the floor.\n", item_name(obj, "The"));
		drop_item(obj);
		return;
	}
	printf("You put %s away.\n", item_name(obj, "your"));
}
