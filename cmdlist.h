/*
 *  List of functions and table for finding
 *  correct function for each command line argument.
 */

int	dosource(),	dosuspend(),		dofg(),	
	dohelp(), 	doset(),		dounset(),	
	doalias(),	dounalias(), 		dopickup(),	
	dodrop(),	doinv(),		dowield(),
	dohit(),	dokill(),		douse(),
	dolook(),	domove(),		doexit(),
	dojobs(),	dodebug(),		doresu(),
	dohome(),	dohist(),		dobg(),
	dodest(),	doputaway(),		dosay();

struct function_list findex[] = {
	{ "?",		dohelp,		NONE,
"This command is an alternate for the help command.  Type\n\
``help help'' for more help on the help command." },
	{ "alias",	doalias,	ARRAY,
"This command creates a simple command alias.  Usage is\n\
``alias <name> <command>'' where name is a single word\n\
and <command> is an advsh command line.  In the future, when\n\
<name> is typed, advsh will pretend the user typed <command>\n\
instead.  See ``unalias'' also." },
	{ "bg",		dobg,		NONE,
"This command puts a stopped UNIX process (a ``job'') into\n\
the background, that is, executing without user intervention.\n\
See ``jobs'' also." },
#ifdef DEBUG
	{ "debug",	dodebug,	INT,
"This command enters a special debugging command loop." },
#endif DEBUG
	{ "destroy",	dodest,		ARRAY,
"Remove a file that you see.  You may be able to get it back\n\
later with ``resurect''." },
	{ "drop",	dodrop,		ARRAY,
"Drop an object from your pack onto the floor of the current\n\
room.  This may be a magical object or a file.  You probably\n\
got this object with ``get'' and may be able to pick it up\n\
again this way." },
	{ "exit",	doexit,		NONE,
"Leave the adventure shell.  You probably don't want to do this.\n\
You'e having too much fun, right?" },
	{ "fg",		dofg,		STRING,
"Bring a stopped job into the foreground.  This means that the\n\
job can read from the terminal and write messages to you, and\n\
when you type interrupt, the process receives the interrupt." },
	{ "get",	dopickup,	ARRAY,
"Pick an object up from the floor and put it into your pack.\n\
You need to do this to examine it or use it in any way.  Until\n\
you pick something up, all you can see is what type of object it\n\
is, not its function.  See ``use'' and ``drop'' also." },
	{ "help",	dohelp,		STRING,
"This command prints out a general help message or shorter\n\
help messages on particular commands.  Typing just ``help''\n\
shows you a list of all the Adventure Shell commands, and\n\
typing ``help <command>'' prints out a message about that\n\
particular command." },
	{ "hit",	dohit,		ARRAY,
"When you are wielding an executable program (either that\n\
you own or has been enchanted), you can hit files, which\n\
causes the program you're wielding to be executed on the file\n\
you hit.  See ``wield'' also." },
	{ "history",	dohist,		STRING,
"Print out a list of all the commands you're done so far.\n\
Ash has a history mechanism just like the c-shell.  You can\n\
redo commands or substitute from previous commands just as\n\
you can in csh(1)." },
	{ "home",	dohome,		NONE,
"This command teleports you to your home directory from wherever\n\
you are.  This is ``xyzzy'' are synonymous." },
	{ "inventory",	doinv,		NONE,
"List all the items you currently have in your pack, each with\n\
a letter which you use to identify that object for other advsh\n\
commands." },
	{ "jobs",	dojobs,		NONE,
"Ash has a general job control facility for UNIX processes\n\
just like the c-shell.  When you start up a UNIX process, you\n\
can stop and restart it, move it to the fireground and background\n\
and kill it.  However, you can't start up a UNIX process as easily\n\
in advsh as you can in csh, you must have a ``Scroll of Manipulation''." },
	{ "kill",	dokill,		ARRAY,
"Kill a UNIX process that is currently running or stopped.  You\n\
must give a process id or a job name to kill." },
	{ "look",	dolook,		STRING,
"Look around you in the current room.  You can also give the\n\
name of an ajoining passageway (a UNIX directory) to look in." },
	{ "move",	domove,		STRING,
"Move to an ajoining room (a UNIX directory) if possible." },
	{ "put",	doputaway,	NONE,
"Stow away the weapon you're wielding.  It goes into your\n\
pack and you aren't wielding it any more." },
	{ "quaff",	douse,		ARRAY,
"Drink one of the potions you have.  Once you drink the\n\
potion, you'll be able to tell what it really is, and when\n\
pick up another of the same type, you'll see the true name,\n\
not the silly one.  Unless, you quaff a potion of ``Memory Loss''." },
	{ "read",	douse,		ARRAY,
"Read a scroll or a UNIX file you're carrying.  Reading a\n\
scroll activates the magic in it, and you'll find out what it\n\
does.  From then one, when you pick up a scroll of that type,\n\
you'll see the true name of the scroll instead of a silly name.\n\
Scrolls can be read twice." },
	{ "resurect",	doresu,		ARRAY,
"Try to restore a file you've ``destroy''ed from the dead.\n\
If you're prayers are answered, you'll get the file back." },
	{ "say",	dosay,		ARRAY,
"Print the words (arguments to this command) on the terminal.\n\
this parrot command just repeats the things you give it as\n\
arguments.  If there are multiple arguments to say, each is\n\
separated by a space in the output." },
	{ "set",	doset,		ARRAY,
"Set an advsh variable.  The syntax is ``set <name>'', which\n\
sets the given named variable to true, ``set <name> <value>'',\n\
which set the named variable to the given value or just ``set'',\n\
which prints out all the current variables.  You can also get\n\
the value of a variable in-line by prefixing the name as a word\n\
with a dollar sign (like csh(1))." },
	{ "source",	dosource,	STRING,
"Read in the text file named by the argument and execute the\n\
advsh commands it contains.  This is done automatically at\n\
startup time if you have a .advshrc file in your home directory." },
	{ "suspend",	dosuspend,	NONE,
"Stop the current advsh, returning you to the parent process,\n\
usually a shell.  The advsh is then restartable if invoked from\n\
the c-shell." },
	{ "unalias",	dounalias,	ARRAY,
"Remove the given alias.  An alias created with ``alias''\n\
is no longer known to advsh." },
	{ "unset",	dounset,	ARRAY,
"Remove a variable.  The variable sett with ``set'' is\n\
no longer known to advsh." },
	{ "use",	douse,		ARRAY,
"Utilize the powers of an object you're carrying.  These could\n\
be magical powers in magic items or powers inherent in files,\n\
such as programs or text files (which you can look at)." },
	{ "wield",	dowield,	STRING,
"Take the given item out of your pack and wield it.  From\n\
now on, you can hit things you see in the dungeon with it\n\
to execute the program you're wielding on those files." },
	{ "xyzzy",	dohome,		NONE,
"Teleport to you home directory.  This is exactly the same\n\
as the ``home'' command." },
	{ "zap",	douse,		ARRAY,
"Invoke the magical powers of a wand.  You must specify a\n\
wand item from you pack.  You'll also learn what the wand\n\
does, and will see it by its true name when you pick one\n\
up later.  Wands always have three charges." },
	{ NULL	}
};
