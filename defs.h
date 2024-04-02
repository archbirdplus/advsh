/*
**  Constants for ash.
*/

#define  MAXARGS		1000

#define  MAX_ITEMS	 4	/* from 1 to MAX_ITEMS magic items in a room */

#define  random(l, h)		(rand() % (h - l + 1) + l)


/*
 *  The magic properties of the items.
 */

#define  MG_NONE	0	/* no magic properties */
#define  MG_TELEP	1	/* teleport (cd) to a directory */
#define  MG_MANIP	2	/* manipulate (execute) command */
#define  MG_IDENT	3	/* identify a magic item */
#define  MG_INVIS	4	/* see invisible (`.' files) */
#define  MG_MEMLS	5	/* you lose memory */
#define  MG_BLIND	6	/* you get blind */
#define  MG_ADDST	7	/* add strength */
#define  MG_SUBST	8	/* decrease strength */
#define  MG_EMACS	9	/* stick you in emacs */
#define  MG_HIDEP	10	/* background processes */
#define  MG_ENCHA	11	/* use a file even if not owner */

#define  ITM_NUMBER	12	/* the number of magic types */


/*
 *  The number of uses of each class of object.
 */

#define  USE_WAND	3		 /* wands have three charges */
#define  USE_SCROLL	2		 /* two uses for scrolls */
#define  USE_POTION	1		 /* potions are one-shot */


/*
 *  Weights of magic items.
 */

#define  WHT_WAND		15
#define  WHT_POTION		10
#define  WHT_SCROLL		5


/*
 *  The types of objects.
 */

#define  OBJ_FILE		1	  /* item is a file */
#define  OBJ_SCROLL		2	  /* item is a scroll */
#define  OBJ_POTION		3	  /* item is a potion */
#define  OBJ_WAND		4	  /* item is a wand */


/*
 *  Player constants, and weight/strength converson.
 */

#define  STRENGTH	  15	/* starting strength */

#define  newstr()	  (me.pl_maxwht = me.pl_str * 10)
#define  FILE_WHT	  512  /* a block weights 2 lbs. */

/*
 *  if we are a login shell ...
 */

#define  isloginsh()	 (getppid() == 1)

/*
 *  An extra ctype function...
 */

#define todigit(c)	((c) + '0')
