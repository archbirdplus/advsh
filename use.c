#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>

#include "defs.h"
#include "types.h"
#include "jc/jobs.h"

extern PLAYER 	me;

douse(args)
	char 	*args[];
{
	ITEM 		*obj = 0, 
			*find_item(), *get_item();
	struct stat 	stbuf;
	char 		*item_name();

	if ( !strcmp(args[0], "quaff") ) {  /* use a potion */
		if ( args[1] ) {
			if ( (obj = get_item(args[1][0])) == NULL )
				return;
			if (obj->it_type != OBJ_POTION ) {
				puts("You can't drink that!");
				return;
			}
		}
		else
			if ( (obj = find_item(OBJ_POTION)) == NULL )
					return;
	}
	else if ( !strcmp(args[0], "read") ) {  /* use a scroll */
		if ( me.pl_blind ) {
			puts("It's hard to read in the dark.");
			return;
		}
		if ( args[1] ) {
			if ( (obj = get_item(args[1][0])) == NULL )
				return;
			if ( obj->it_type != OBJ_SCROLL  &&
					obj->it_type != OBJ_FILE ) {
				puts("You can't read that!");
				return;
			}
		}
		else
			if ( (obj = find_item(OBJ_SCROLL)) == NULL )
					return;
	}
	else if ( !strcmp(args[0], "zap") ) {  /* use a wand */
		if ( args[1] ) {
			if ( (obj = get_item(args[1][0])) == NULL )
				return;
			if (obj->it_type != OBJ_WAND ) {
				puts("You can't zap with that!");
				return;
			}
		}
		else
			if ( (obj = find_item(OBJ_WAND)) == NULL )
					return;
	}
	else {  /* use <magic object/file> */
		if ( args[1] == NULL ) {
			if  ( (obj = find_item(0)) == NULL )
				return;
		}
		else
			if ( (obj = get_item(args[1][0])) == NULL )
				return;
	
		if ( obj->it_type == OBJ_FILE ) {
			if ( stat(obj->it_path, &stbuf) ) {
				jcerror("douse: Can't stat %s", obj->it_path);
				return;
			}
			if ( stbuf.st_uid != getuid()  && !obj->it_magic ) {
				printf("%s resists you.\n", 
					item_name(obj, "The"));
				return;
			}
		}
	}

	if ( obj == NULL ) {
		log("douse: obj not assigned.");
		return;
	}

	if ( obj->it_type == OBJ_SCROLL && me.pl_blind ) {
		puts("It's hard to read in the dark.");
		return;
	}

	if ( obj->it_uses <= 0  &&  obj->it_magic > 0 ) {
		printf("%s is all used up.\n", 
			item_name(obj, "The"));
		return;
	}

	/* else, go ahead, all is ok */
	item_known(obj);
	use_obj(obj);

	/* now, do what with the object */
	switch ( obj->it_type ) {
	case OBJ_FILE: /* don't remove or decrement */
		break;
	case OBJ_WAND: /* decrement usage, but don't remove */
		if ( obj->it_uses > 0 )
			obj->it_uses--;
		if ( obj->it_uses <= 0 && obj->it_num > 1 ) {
			/* one is gone, set up so the other(s) is(are) usable */
			obj->it_num--;
			obj->it_uses = USE_WAND;
		}
		break;
	case OBJ_SCROLL: /* remove if used up */
		obj->it_uses--;  /* one use gone */
		if ( obj->it_uses <= 0 )  /* all used up? */
			if ( obj->it_num > 1 ) {  /* decrement number */
				printf("One of %s disappears\n",
					item_name(obj, "your"));
				obj->it_num--;
				obj->it_uses = USE_SCROLL;
			}
			else {  /* remove the item */
				printf("%s vanishes\n", item_name(obj, "Your"));
				rm_obj(obj);
			}
		break;
	case OBJ_POTION: /* always remove */
		obj->it_num--;  /* we used one up */
		if ( obj->it_num <= 0 )  /* none left? */
			rm_obj(obj);
		break;
	default:  /* what?? */
		printf("use: Object type %d not matched in switch\n",
			obj->it_type);
		break;
	}
}


/*
 *  Adds an object to the list of items in the inventory.  This
 *  routine keeps them alphabetically ordered by the it_letter
 *  field.
 */

ITEM *
add_obj(obj)
	ITEM	*obj;
{
	ITEM	*tmp, *last;

	if ( me.pl_curwht + obj->it_weight > me.pl_maxwht ) {  /* too heavy */
		puts("That would make your pack too heavy.");
		return(NULL);
	}
	me.pl_curwht += obj->it_weight;

	/* make sure we don't already have one */
	for ( tmp = me.pl_inven; tmp; tmp = tmp->it_next ) {
		if ( obj->it_type == OBJ_FILE ) {
			/* check if we're already carrying that file */
			if ( !strcmp(obj->it_path, tmp->it_path) ) {
				cfree(obj->it_name);
				cfree(obj->it_path);
				cfree(obj);
				return(tmp);   /* don't add new file */
			}
		} else  /* check if we have one of those already */
			if ( tmp->it_type == obj->it_type 
				&& tmp->it_magic == obj->it_magic ) {
				cfree(obj);
				tmp->it_num++;
				return(tmp);   /* all done */
			}
	}

	/* else it must be a new item */
	if ( getletter(obj) ) {
		puts("That would be too many things to carry.");
		/* subtract amount added to pack weight */
		me.pl_inven -= obj->it_weight;
		return(NULL);
	}

	/* stick the item in the proper place */
	last = NULL;
	for ( tmp = me.pl_inven; tmp; tmp = tmp->it_next ) {
		if ( tmp->it_letter > obj->it_letter )
			break;
		last = tmp;
	}

	obj->it_next = tmp;
	if ( last == NULL )
		me.pl_inven = obj;
	else
		last->it_next = obj;
	
	return(obj);
}


/*
 *  Return the next free letter for a new item in the
 *  user's pack.
 */

getletter(obj)
	ITEM	*obj;
{
	ITEM	*tmp;
	char	l;

	for ( l = 'a'; l <= 'z'; l++ ) {
		for ( tmp = me.pl_inven; tmp; tmp = tmp->it_next )
			if ( tmp->it_letter == l )
				break;
		if ( tmp == NULL )  /* we went through without finding it */
			break;
	}
	if ( l > 'z' )  /* no letters left */
		return(1);

	obj->it_letter = l;  /* assign the letter */
	return(0);
}


/*
 *  Remove an object from the user's pack, and free
 *  the ascociated letter for a new item.
 */

rm_obj(obj)
	ITEM 	*obj;
{
	ITEM 	*tmp, *last;

	tmp = me.pl_inven;
	last = NULL;
	while ( tmp ) {
		if ( tmp->it_name == obj->it_name && 
				tmp->it_magic == obj->it_magic ) {

			/* we got the right item now */
			if ( last == NULL )
				me.pl_inven = tmp->it_next;
			else
				last->it_next = tmp->it_next;

			break;
		}

		last = tmp;
		tmp = tmp->it_next;
	}

	if ( tmp == NULL ) {  /* item not found */
		error("rm_obj: object not found in inventory!");
		return;
	}

	/* reduce weight */
	me.pl_curwht -= tmp->it_weight;
	if ( me.pl_curwht < 0 )
		me.pl_curwht = 0;
}


/*
 *  Make use of the object we were handed based on the type
 *  and properties if the object.
 */

use_obj(obj)
	ITEM 	*obj;
{
	char 	*newargs[4], *item_name();

	if ( obj->it_type == OBJ_FILE ) {  /* object is a file */
		if ( !access(obj->it_path, X_OK) ) {
			newargs[0] = obj->it_path;
			newargs[1] = NULL;
			jcexec(newargs, JC_FOREGROUND, -1, -1, -1);
		}
		else {
			if ( me.pl_blind ) {
				puts("It is too dark to read the file.");
				return;
			}
			printf("You read %s ...\n", item_name(obj, "the"));
			newargs[0] = "more";
			newargs[1] = obj->it_path;
			newargs[2] = NULL;
			jcexec(newargs, JC_FOREGROUND, -1, -1, -1);
		}
	}
	else {  /* use the magic object */
		switch ( obj->it_magic ) {
			case MG_NONE:  /* no magic */
				switch ( obj->it_type ) {
					case OBJ_SCROLL:
						puts("This is a blank scroll");
						break;
					case OBJ_POTION:
						puts("This is Fruit Juice");
						break;
					case OBJ_WAND:
						puts("You have a walking stick");
						break;
				}
				break;
			case MG_TELEP:  /* teleport (cd) */
				puts(item_name(obj, "A"));
				teleport();
				break;
			case MG_MANIP:  /* execute a command */
				puts(item_name(obj, "A"));
				manipulate();
				break;
			case MG_IDENT:  /* identify a command */
				printf("This is %s\n", item_name(obj, "a"));
				identify();
				break;
			case MG_INVIS:  /* see invisible */
				invisible();
				break;
			case MG_MEMLS:  /* lose your memory */
				memory();
				break;
			case MG_BLIND:  /* you go blind */
				blindme();
				break;
			case MG_ADDST:  /* increase strength */
				puts("You feel strong, boy what bulging muscles!");
				me.pl_str++;
				newstr();
				break;
			case MG_SUBST:  /* lose strength */
				puts("Oh, you feel very sick now.");
				me.pl_str--;
				newstr();
				break;
			case MG_EMACS:  /* go into emacs */
				printf("Oh, no! %s\n", item_name(obj, "a"));
				newargs[0] = "emacs";
				newargs[1] = NULL;
				jcexec(newargs, JC_FOREGROUND, -1, -1, -1);
				break;
			case MG_HIDEP:  /* stick a process in the background */
				printf("Wow, %s!\n", item_name(obj, "a"));
				hidden();
				break;
			case MG_ENCHA:  /* use a file even if not owner */
				printf("This is %s!\n", item_name(obj, "a"));
				enchant();
				break;
			default: /* unknown magic property */
				puts("You feel a strange sense of loss.");
				break;
		}
	}
}
