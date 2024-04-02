#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>

#include "defs.h"
#include "types.h"
#include "names.h"

char  	wands_known[ITM_NUMBER], 
      	scrolls_known[ITM_NUMBER],  /* flags if real names are known */
      	potions_known[ITM_NUMBER];

char  	*fake_wands[ITM_NUMBER],
      	*fake_scrolls[ITM_NUMBER],  /* disguised names */
      	*fake_potions[ITM_NUMBER];


/*
 *  Item_name -- format the real or disguised name of a magic
 *  item or file.
 */

#define PLURAL(n)	((n) != 1 ? "s" : "")

char *
item_name(obj, article)
	ITEM 	*obj;
	char 	*article;
{
	static char 	line[100], numstr[5];
	char  		bonus[20];

	/* a string version of the item quantity */
	sprintf(numstr, "%d", obj->it_num);

	/* find out which type of item */
	switch ( obj->it_type ) {
	case OBJ_POTION: /* potion */
		if ( potions_known[obj->it_magic] )
			if ( obj->it_magic == MG_NONE )
				sprintf(line, "%s Fruit Juice",
					(obj->it_num == 1) ? article : numstr);
			else
				sprintf(line, "%s potion%s of %s",
					(obj->it_num == 1) ? article : numstr,
					PLURAL(obj->it_num),
					potion_names[obj->it_magic]);
		else
			sprintf(line, "%s %s potion%s", 
				(obj->it_num == 1) ? article : numstr,
				fake_potions[obj->it_magic],
				PLURAL(obj->it_num));
		break;
	case OBJ_SCROLL: /* scroll */
		if ( scrolls_known[obj->it_magic] )
			if ( obj->it_magic == MG_NONE )
				sprintf(line, "%s Blank scroll%s",
					(obj->it_num == 1) ? article : numstr, 
					PLURAL(obj->it_num));
			else
				sprintf(line, "%s %sscroll%s of %s",
					(obj->it_num == 1) ? article : numstr,
					(obj->it_uses==1 && obj->it_num==1) ? 
						"yelowed " : "",
					PLURAL(obj->it_num),
					scroll_names[obj->it_magic]);
		else
			sprintf(line, "%s scroll%s labeled '%s'", 
				(obj->it_num == 1) ? article : numstr,
				PLURAL(obj->it_num),
				fake_scrolls[obj->it_magic]);
		break;
	case OBJ_WAND: /* wand */
		if ( wands_known[obj->it_magic] )
			if ( obj->it_magic == MG_NONE )
				sprintf(line, "%s Walking stick%s",
					(obj->it_num == 1) ? article : numstr,
					PLURAL(obj->it_num));
			else if ( obj->it_num > 1 )
				sprintf(line, "%d wands of %s",
					todigit(obj->it_num),
					wand_names[obj->it_magic]);
			else if ( obj->it_uses < 1 )
				sprintf(line, "%s wand of %s",
					article, wand_names[obj->it_magic]);
			else
				sprintf(line, "%s wand of %s with %d charge%s",
					article, wand_names[obj->it_magic],
					obj->it_uses, PLURAL(obj->it_uses));
		else
			sprintf(line, "%s %s wand%s", 
				(obj->it_num == 1) ? article : numstr,
				fake_wands[obj->it_magic],
				PLURAL(obj->it_num));
		break;
	case OBJ_FILE: /* file */
		sprintf(bonus, "%c%d ", 
			obj->it_magic >= 0 ? '+' : '-', obj->it_magic);
		sprintf(line, "%s %sfile labeled '%s'", 
			(obj->it_num == 1) ? article : numstr,
			!access(obj->it_path, X_OK) ? bonus : "", 
			obj->it_name);
		break;
	}
	return(line);
}


/*
 *  Make the status of an item type known, that is succesive
 *  item_name calls will tell the true name of the item.
 */

item_known(obj)
	ITEM 	*obj;
{
	switch ( obj->it_type ) {
		case OBJ_FILE:
			break;
		case OBJ_POTION:
			potions_known[obj->it_magic] = 1;
			break;
		case OBJ_SCROLL:
			scrolls_known[obj->it_magic] = 1;
			break;
		case OBJ_WAND:
			wands_known[obj->it_magic] = 1;
			break;
	}
}


set_names()
{
	int  	i, s, w, p;

	s = random(0, 5);
	p = random(0, 5);  /* go into aray of each type */
	w = random(0, 5);
	for ( i = 0; i < ITM_NUMBER; i++ ) {
		/* next potion name */
		if ( potion_words[++p] == NULL )
			p = 0;
		fake_potions[i] = potion_words[p];

		/* next scroll name */
		if ( scroll_words[++s] == NULL )
			s = 0;
		fake_scrolls[i] = scroll_words[s];

		/* next wand name */
		if ( wand_words[++w] == NULL )
			w = 0;
		fake_wands[i] = wand_words[w];
	}
}
