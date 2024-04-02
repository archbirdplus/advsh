#include <stdio.h>
#include <signal.h>

int	cleanup(),	interrupt(),
	dodebug(),	child();

int (*SIG_FUNCTS[])() = {
	NULL,  /* placeholder, first signal == 1 */
	cleanup, 	interrupt, 	interrupt, 	SIG_IGN, 	
	SIG_IGN, 	SIG_IGN, 	SIG_IGN, 	SIG_DFL, 	
	SIG_DFL, 	SIG_DFL, 	SIG_DFL, 	SIG_DFL, 	
	SIG_IGN, 	SIG_IGN, 	SIG_IGN, 	SIG_IGN, 	
	SIG_DFL, 	SIG_IGN, 	SIG_IGN,	child, 		
	SIG_IGN, 	SIG_IGN, 	SIG_IGN, 	SIG_IGN, 	
	SIG_IGN, 	SIG_IGN, 	SIG_DFL, 	SIG_IGN, 	
	SIG_IGN, 	SIG_IGN, 	SIG_IGN
};

/*
 *  Set signals.  Most are ignored, a few are caught 
 *  for special stuffs.
 */

sigs()
{
	int  	i;

	for ( i = 1; i < NSIG; i++ )
		signal(i, SIG_FUNCTS[i]);
}


/*
 *  Set the signal to the 'normal' configuration for the
 *  shell (according to the above array).
 */

signorm(signo)
{
	signal(signo, SIG_FUNCTS[signo]);
}


/*
 *  Ignore the signal.  Set the signal to SIG_IGN.
 */

sigign(signo)
{
	signal(signo, SIG_IGN);
}


/*
 *  Reset the signal to SIG_DFL (default state).
 */

sigdfl(signo)
{
	signal(signo, SIG_DFL);
}


#define mask(sig)	(1 << ((sig) - 1))

/*
 *  Hold (queue) a signal for (presumably) later unblocking.
 */

sighold(signo)
{
	sigblock(mask(signo));
}


/*
 *  Release a signal.  All queued signals will now be delivered.
 */

sigrelse(signo)
{
	sigsetmask(sigblock(0) &~ mask(signo));
}
