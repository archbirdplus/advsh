#include <stdio.h>
#include <pwd.h>
#include <sys/file.h>

/*
 *  Execute lines in the ~/.advshrc file like commands, except
 *  that we ignore lines that start with a '#' (comments) or
 *  are empty.
 */

dosource(file)
	char	*file;
{
	char 	line[BUFSIZ];
	FILE 	*fp;

	if ( !file || !*file ) {
		puts("What file do you want to source?");
		return;
	}

	if ( (fp = fopen(file, "r")) == NULL ) {
		jcperror("I can't open %s", file);
		return;
	}
	
	while ( fgets(line, BUFSIZ, fp) != NULL ) {
		if ( line[0] == '\n' || line[0] == '#' )   /* a comment */
			continue;
		line[strlen(line) - 1] = '\0';  /* zap newline */
		command(line);
	}
	
	fclose(fp);
	return (0);
}

char	INITFILE[] = ".advshrc";

setupfile()
{
	extern char	*getenv();
	char		*home;
	struct passwd	*pwptr;
	char		path[BUFSIZ];

	if ((home = getenv("HOME")) == NULL) {
		pwptr = getpwuid(getuid());
		if (pwptr == NULL)
			panic("Who are you; you're not in the passwd file!");
		home = pwptr->pw_dir;
	}
	sprintf(path, "%s/%s", home, INITFILE);
	if (access(path, F_OK) == 0)
		return dosource(path);
	else
		return (1);
}
