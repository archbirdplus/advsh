#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>

#include "defs.h"
#include "types.h"

extern PLAYER 	me;
extern char	tmpdir[];

dopickup(args)
	char 	*args[];
{
	ITEM 		*tmp, *last, *obj, 
			*areitems(), *add_obj();
	struct stat 	stbuf;
	char 		tmpfile[BUFSIZ], *pathname(), 
			*alloc(), *item_name();
	int 		i;

	if ( args[1] == NULL ) {
		puts("Pick what up?");
		return;
	}

	if ( !strcmp(args[1], "up") )
		i = 2;
	else
		i = 1;
	
	for ( ; args[i]; i++ ) {
		switch ( args[i][0] ) {  /* check for abbreviations */
			case '?':  /* scroll */
				args[i] = "scroll";
				break;
			case '!':  /* potion */
				args[i] = "potion";
				break;
			case '/':  /* wand */
				args[i] = "wand";
				break;
		}

		/* check if magic item */
		last = NULL;
		for ( tmp = areitems(NULL); tmp; tmp = tmp->it_next ) {
			if ( !strcmp(tmp->it_name, args[i]) ) {
				/* remove that object from the list */
				if ( last )
					last->it_next = tmp->it_next;
				else
					me.pl_list->ls_list = tmp->it_next;
				/* add object to our pack */
				if ( (obj = add_obj(tmp)) == NULL ) {  
					/* put the item back in the list */
					tmp->it_next = me.pl_list->ls_list;
					me.pl_list->ls_list = tmp;
					return;
				}
				obj = tmp;
				goto cont;
			}
			last = tmp;
		}
		if ( indir(args[i]) ) {  /* a file in the current directory */
			if ( stat(args[i], &stbuf) || 
				(access(args[i], R_OK) && access(args[i], X_OK)) ) {
				puts("You can't grab on to the file.");
				continue;
			}
			if ( stbuf.st_mode & S_IFDIR ) {
				puts("That is a passageway");
				continue;
			}
			if ( me.pl_curwht + (stbuf.st_size / FILE_WHT) 
					> me.pl_maxwht ) {
				puts("That would make your pack too heavy.");
				continue;
			}

			obj = (ITEM *) alloc(sizeof(ITEM));
			obj->it_weight = stbuf.st_size / FILE_WHT;
			me.pl_curwht  += obj->it_weight;
			obj->it_num    = 1;

			sprintf(tmpfile, "%s/%s", tmpdir, args[i]);
			obj->it_path	= alloc(strlen(tmpfile) + 1);
			strcpy(obj->it_path, tmpfile);

			getfilename(obj, args[i]);
			obj->it_type   = OBJ_FILE;
			obj->it_magic  = stbuf.st_uid == getuid();
			obj->it_uses   = 1;
			if ( (obj = add_obj(obj)) == NULL )
				cfree(obj);
			
			if ( copyfile(args[i], tmpfile) || unlink(args[i]) )
				printf("An image of '%s' stays on the floor.\n",
					args[i]);
		}
		else  { /* not found */
			printf("I don't see a %s.\n", args[i]);
			continue;
		}

cont:		if ( obj->it_letter )
			printf("%c) %s.\n", (char) obj->it_letter, 
				item_name(obj, "a"));
	}
}


/*
 *  Generate the full pathname of a file in the current
 *  directory.
 */

char *
pathname(file)
	char 	*file;
{
	char 	line[1024], *p;

	getwd(line);

	p = alloc(strlen(line) + strlen(file) + 2);
	sprintf(p, "%s/%s", line, file);

	return(p);
}
