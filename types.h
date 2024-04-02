#include <pwd.h>

/*
 *  Every item the character picks up (or can pick up) is saved in
 *  a struct like this.  The it_path element is used only for files,
 *  where it is the full pathname of the file.
 */

typedef struct _item {
	char		*it_name;
	char		it_letter;
	int	 	it_num;
	char		*it_path;
	int	 	it_weight;
	int	 	it_type;
	int	 	it_magic;
	int	 	it_uses;
	struct _item	*it_next;
} ITEM;


/*
 *  The list structure is used to store the list of directories
 *  that we have already visited.  Along with their contents, if
 *  any,
 */

typedef struct _list {
	char		*ls_dir;
	short	 	ls_flags;
	struct _item	*ls_list;
	struct _list	*ls_next;
} LIST;


/* 
 *  Aliases are saved in a linked list of ALIAS structs.
 */

typedef struct _alias {
	char		*al_name;
	char		*al_str;
	struct _alias 	*al_next;
} ALIAS;


/*
 *  The shell variables are saved in a linked list of
 *  VARS structs.
 */

typedef struct _var {
	char	 	*vr_name;
	char	 	*vr_val;
	struct _var	*vr_next;
} VARS;


/*
 *  The player struct is the record of the user.  Thus we have
 *  all the info in one place.
 */

typedef struct _player {
#define PL_NAMLEN	30
	char	 	pl_name[PL_NAMLEN];
	char		*pl_cwd;
	long		pl_wealth;
	struct passwd	pl_who;
	int		pl_str;
	int		pl_maxwht;
	int		pl_curwht;
	short		pl_hunger;
	struct _item 	*pl_weapon;
	short		pl_blind;
	struct _item 	*pl_inven;
	struct _alias	*pl_aliases;
	struct _var	*pl_vars;
	struct _list	*pl_list;
} PLAYER;
