/*
 *  Types of arguments possible.  NONE means pass no argument to
 *  C function, ARRAY means pass a (char **) array to C builtin,
 *  STRING means a single (char *) argument, and INT means a
 *  single interger argument.
 */
#define NONE		0
#define	ARRAY		1
#define STRING		2
#define INT		3

struct function_list {
	char	*f_commd;	/* name of command */
	int	(*f_funct)();	/* C builtin function */
	short	f_atype;	/* type of argument */
	char	*f_help;	/* help string for function */
};

extern struct function_list	findex[];
