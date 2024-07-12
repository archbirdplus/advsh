#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#include "defs.h"
#include "types.h"
#include "jc/jobs.h"

extern char 	*fake_wands[], *fake_scrolls[], *fake_potions[];
extern char 	*wand_names[], *scroll_names[], *potion_names[];
extern PLAYER 	me;
extern PROC	*proc_list;
extern int 	errno,
		debug;

/* shades of lisp ... */
#define comp(s)      (!strcmp(s, args[0]))

/*
 *  Pseudo shell for debugging.  Unless the shell is invoked with
 *  -d, this routine is not available to the normal user.
 */

dodebug(signo)
{
	char 	line[BUFSIZ], *args[MAXARGS];

	if ( !debug ) {
		if  ( !author() ) {	/* keep 'em away */
			puts("I don't understand that.");
			return;
		}
		else  			/* do it with warning */
			puts("(Ash not invoked with -d)");
	}

	sigign(SIGCHLD);	/* we will have spurrious SIGCHLD signals */
	sigign(SIGINT);		/* ignore interrupts so we can get to work */

	for (;;) {
		fputs("> Debug > ", stdout);
		if ( gets(line) == NULL ) {
			printf("^D\n");
			signorm(SIGCHLD);
			clearerr(stdin);
			return;
		}

		if ( parse(line, args) < 0 )
			continue;

		if ( comp("exit") )
			exit(0);
		else if ( comp("quit") )
			return;
		else if ( comp("core") )
			kill_shell(SIGQUIT);
		else if ( comp("real") )
			debug_print_real();
		else if ( comp("fake") )
			debug_print_fake();
		else if ( comp("magic") )
			debug_print_magic();
		else if ( comp("inven") )
			debug_print_inven();
		else if ( comp("me") )
			debug_print_me();
		else if ( comp("list") )
			debug_print_list();
		else if ( comp("procs") )
			debug_print_procs();
		else if ( comp("create") )
			debug_make_item();
		else if ( comp("cd") ) {
			if ( chdir(args[1]) )
				jcperror("Can't change to %s", args[1]);
		}
		else if ( line[0] == '?' )
			debug_help();
		else  /* execute shell command */
			system(line);
	}
	/* NOTREACHED */
}


/*
 *  Print out the real names of the magic items, showing
 *  their properties.
 */

debug_print_real()
{
	int i;

	puts("\npotion_names:             wand_names:               scroll_names:");
	for ( i = 0; i < ITM_NUMBER; i++ )
		printf("%d) %s %-20.20s  %-20.20s  %-.20s\n", i,
			i < 10 ? " " : "",
			potion_names[i] ? potion_names[i] : "(nil)",
			wand_names[i] ? wand_names[i] : "(nil)",
			scroll_names[i] ? scroll_names[i] : "(nil)");
}


/*
 *  Print out the "fake" names of the items.
 */

debug_print_fake()
{
	int	i;

	puts("\nfake_potions:             fake_wands:               fake_scrolls:");
	for ( i = 0; i < ITM_NUMBER; i++ )
		printf("%d) %s %-20.20s  %-20.20s  %-.20s\n", i,
			i < 10 ? " " : "",
			fake_potions[i], fake_wands[i], fake_scrolls[i]);
}


/*
 *  print out the magic items in the current direcotory,
 *  (the first on in the linked list of LIST structs.
 */

debug_print_magic()
{
	ITEM *tmp;

	for ( tmp = me.pl_list->ls_list; tmp; tmp = tmp->it_next ) {
		printf("ITEM struct, it_name = '%s'", tmp->it_name);
		printf("\tit_path = %s\n", 
			tmp->it_path ? tmp->it_path : "(nil)");
		printf("\tit_uses = %d, it_weight = %d, it_type = %d, it_magic = %d\n", tmp->it_uses, tmp->it_weight, tmp->it_type, tmp->it_magic);
		printf("\tit_next = %ld\n", tmp->it_next);
	}
}


/*
 *  Print out the ITEM structs of the items in our inventory.
 */

debug_print_inven()
{
	ITEM *tmp;

	for ( tmp = me.pl_inven; tmp; tmp = tmp->it_next ) {
		printf("ITEM struct, it_name = '%s', it_letter = '%c'\n", 
			tmp->it_name, tmp->it_letter);
		printf("\tit_path = %s\n", 	
			tmp->it_path ? tmp->it_path : "(nil)");
		printf("\tit_uses = %d, it_weight = %d, it_type = %d, it_magic = %d\n", 
			tmp->it_uses, tmp->it_weight, tmp->it_type, tmp->it_magic);
		printf("\tit_num = %d, it_next = %ld\n", 
			tmp->it_num, tmp->it_next);
	}
}


/*
 *  Print out the components of the PLAYER struct that holds
 *  all the info on the current user.
 */

debug_print_me()
{
	printf("PLAYER struct: me.pl_name = %s, pl_str = %d, pl_inven = %ld\n",
		me.pl_name, me.pl_str, me.pl_inven);
	printf("\tpl_cwd = '%s', pl_wealth = %d,\n", me.pl_cwd, me.pl_wealth);
	printf("\tpl_who.pw_name = %s, pw_uid = %d, pw_dir = %s\n",
		me.pl_who.pw_name, me.pl_who.pw_uid, me.pl_who.pw_dir);
	printf("\tpl_weapon = %d, pl_maxwht = %d, lp_curwht = %d, pl_blind = %d\n",
		me.pl_weapon, me.pl_maxwht, me.pl_curwht, me.pl_blind);
	printf("\tpl_vars = %ld, pl_list = %ld\n", me.pl_vars, me.pl_list);
}


/*
 *  Print out list of directories that we have visited.
 */

debug_print_list()
{
	LIST *tmp;

	tmp = me.pl_list;
	puts("LIST struct: me.pl_list =");
	while ( tmp ) {
		printf("\tls_dir = %s, ls_list = %ld, ls_next = %ld\n",
			tmp->ls_dir, tmp->ls_list, tmp->ls_next);
		tmp = tmp->ls_next;
	}
}


/*
 *  prtint out the asctive child processes and their states.
 */

debug_print_procs()
{
	PROC 	*tmp;

	for ( tmp = proc_list; tmp; tmp = tmp->pr_next ) {
		printf("pr_name = `%s'\n", tmp->pr_name);
		printf("\tpr_pid = %d, pr_pgrp = %d, pr_exit = %d, pr_sig = %d\n",
			tmp->pr_pid, tmp->pr_pgrp, tmp->pr_exit, tmp->pr_sig);
		printf("\tpr_flags = %04o (%s%s%s%s%s).\n",
			tmp->pr_flags,
			tmp->pr_flags & JC_REPORT ? " REPORT " : "",
			tmp->pr_flags & JC_RUNNING ? " RUNNING " : "",
			tmp->pr_flags & JC_STOPPED ? " STOPPED " : "",
			tmp->pr_flags & JC_EXITED ? " EXITED " : "",
			tmp->pr_flags & JC_COREDUMP ? " COREDUMP " : "");
	}
}


/*
 *  Send our shell the specified signal.  First we have
 *  to reset the default action.
 */

kill_shell(signo)
{
	signal(signo, SIG_DFL);
	kill(proc_list->pr_pid, signo);
}


/*
 *  Create a magic item of the given type and
 *  magic class.
 */

debug_make_item()
{
	ITEM	*tmp, *add_obj();
	char	*alloc();

	tmp = (ITEM *) alloc(sizeof(ITEM));

	tmp->it_num = 1;

	fputs("Type (1=FILE, 2=SCROLL, 3=POTION, 4=WAND) ", stdout);
	scanf("%d", &tmp->it_type);

	switch ( tmp->it_type ) {
		case OBJ_FILE:
			puts("Can't create a file.");
			return;
		case OBJ_POTION:
			tmp->it_weight = WHT_POTION;
			tmp->it_uses = USE_POTION;
			break;
		case OBJ_SCROLL:
			tmp->it_weight = WHT_SCROLL;
			tmp->it_uses = USE_SCROLL;
			break;
		case OBJ_WAND:
			tmp->it_weight = WHT_WAND;
			tmp->it_uses = USE_WAND;
			break;
		default:
			puts("Illegal type.");
			return;
	}

	fputs("Magic Property (see 'real') ", stdout);
	scanf("%d", &tmp->it_magic);

	if ( (unsigned) tmp->it_magic >= ITM_NUMBER ) {
		puts("Illegal magic class.");
		return;
	}

	if ( (tmp = add_obj(tmp)) == NULL )
		puts("Can't add the object.");
}


/*
 *  Print out a (debug) help message.
 */

char 	*debug_help_lines[] = {
	"",
	"	--DEBUG Help Mode--",
	">  exit     - exit shell.",
	">  quit     - continue with ash.",
	">  core     - coredump current state.",
	">  real     - print the real names (properties) of the magic types.",
	">  fake     - print the 'fake' names of the magic item types.",
	">  procs    - print list of processes.",
	">  magic    - print list of magic items in current directory.",
	">  inven    - print list of inventory items.",
	">  list     - print list of directories we have been in.",
	">  me       - print out the me PLAYER struct.",
	">  cd       - chdir to another directory.",
	">  create   - create a magic item.",
	"any other command is executed as a sh command by system()\n",
	NULL
};

debug_help()
{
	int	i;

	for ( i = 0; debug_help_lines[i]; i++ )
		puts(debug_help_lines[i]);
}
