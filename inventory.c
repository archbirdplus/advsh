#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>

#include "defs.h"
#include "types.h"

extern PLAYER 	me;

doinv()
{
	char 	*item_name();

	if ( me.pl_inven == NULL && me.pl_weapon == NULL ) {
		puts("You aren't carrying anything");
		return;
	}

	puts("You have:");

	if ( me.pl_weapon )
		printf("%s (weapon in hand)\n", 
			item_name(me.pl_weapon, "A"));

	inv_list(0);
}


/*
 *  List the items in the inventory corresponding to what.
 *  Or all items if what is zero.
 */

inv_list(what)
{
	ITEM 	*tmp;
	char 	*item_name();

	if ( what ) {
		if ( me.pl_inven == NULL ) {
			puts("Nothing appropriate.");
			return;
		}
		/* list items that match type */
		for ( tmp = me.pl_inven; tmp; tmp = tmp->it_next )
			if ( tmp->it_type == what )
				printf("%c) %s\n", tmp->it_letter, 
					item_name(tmp, "A"));
	}
	else {  /* list everything */
		for ( tmp = me.pl_inven; tmp; tmp = tmp->it_next )
			printf("%c) %s\n", tmp->it_letter, item_name(tmp, "A"));
	}
}
