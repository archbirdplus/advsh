#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>

#include "defs.h"
#include "types.h"

extern PLAYER 	me;

/*
 *  Drop an item from the user's pack.
 */

dodrop(args)
	char 	*args[];
{
	ITEM 	*obj, 
		*find_item(),
		*get_item();
	int 	i;

	if ( args[1] == NULL ) {  /* prompt for item */
		if ( (obj = find_item(0)) == NULL )
			return;
	}
	else {  /* look for item lettered the same as arguemt */
		for ( i = 1; args[i]; i++ ) {
			if ( (obj = get_item(args[i][0])) == NULL )
				continue;
			drop_item(obj);
		}
	}
}


/*
 *  Actually figure out the details of what (how many) the
 *  user wants to drop and doit.
 */

drop_item(obj)
	ITEM	*obj;
{
	int	n;
	char	*item_name();

	if ( obj->it_num > 1 ) {  /* how many? */
		printf("You have %d, how many? ", obj->it_num);
		scanf("%d", &n);
		if ( (obj->it_num - n) <= 0 )  /* all gone */
			rm_obj(obj);
		else
			obj->it_num -= n;
		/* put the item back into the directory */
		obj->it_next = me.pl_list->ls_list;
		me.pl_list->ls_list = obj;
	}
	else  /* single item, remove it */
		if ( obj->it_type == OBJ_FILE )
			drop_file(obj);
		else {  /* magic item */
			rm_obj(obj);
			obj->it_next = me.pl_list->ls_list;
			me.pl_list->ls_list = obj;
		}
	printf("You drop %s.\n", item_name(obj, "the"));
}


/*
 *  When we drop a file we try to make a copy of it in the
 *  curent directory.
 */

drop_file(obj)
	ITEM *obj;
{
	char 		buf[BUFSIZ], 
			*item_name(), *tail();
	struct 	stat 	stbuf;
	int 		fd, in, n;

	sighold(SIGINT);
	sighold(SIGQUIT);

	if ( access(".", W_OK) ) {  /* we can't write on this directory */
		printf("%s disapears as it touches the floor.\n",
			   item_name(obj, "The"));
		return;
	}

	if ( stat(obj->it_path, &stbuf) ) {
		jcerror("drop_file: Can't stat %s", obj->it_path);
		return;
	}

	fd = open(tail(obj->it_path), O_WRONLY | O_CREAT, (int) stbuf.st_mode);
	in = open(obj->it_path, O_RDONLY);

	if ( fd < 0 ) {
		printf("%s disapears as it touches the floor.\n",
			item_name(obj, "The"));
		jcerror("drop_file: Can't open %s", tail(obj->it_path));
		return;
	}

	if ( in < 0 ) {
		printf("%s disapears as it touches the floor.\n",
			 item_name(obj, "The"));
		jcerror("drop_file: Can't open %s", obj->it_path);
		close(fd);
		return;
	}

	while ( (n = read(in, buf, BUFSIZ)) > 0 )
		if ( write(fd, buf, n) != n ) {
			jcerror("drop_file: Write error to %s",
			     tail(obj->it_path));
			break;
		}
	
	close(in);
	close(fd);

	rm_obj(obj);			/* drop the object from the pack */

	if ( obj->it_path )		/* free the space */
		cfree(obj->it_path);
	cfree(obj);

	sigrelse(SIGINT);
	sigrelse(SIGQUIT);
}


/*
 *  Return the tail component of a full pathname.
 *  We hope the path stays around for a while, because
 *  we return a pointer to a point in the path array.
 */

char *
tail(path)
	char	*path;
{
	char	*p;

	if ( !path )			/* handed null pointer */
		return(NULL);		/* give them a null back */

	p = path;			/* pointing to the array */
	while ( *p )			/* move p to the end */
		p++;
	
	p--;				/* back up to before '\0' */
	while ( *p && *p != '/' )	/* go back until a '/' */
		p--;
	
	if ( *p == '/' )
		return(++p);		/* return substring (less '/') */
	else
		return(p);		/* reutrn the string (wierd) */
}
