#include "common.h"

void unix_error(char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(0);
}

void app_error(char *msg)
{
	fprintf(stderr, "%s\n", msg);
}

pid_t Fork(void)
{
	pid_t pid;
	
	if ((pid = fork()) < 0)
	    unix_error("Fork error");
	return pid;	
}

char *Fgets(char *ptr, int n, FILE *stream)
{
	char *rptr;
	
	if (((rptr = fgets(ptr, n, stream)) == NULL) && ferror(stream))
		app_error("Fgets error");
		
	return rptr;
}

void Pause()
{
	(void)pause();
	return;
}

unsigned int Sleep(unsigned int secs)
{
	unsigned int rc;
	
	if ((rc = sleep(secs)) < 0)
		unix_error("Sleep error");
	return rc;
}

void Kill(pid_t pid, int signum)
{
	int rc;
	
	if ((rc = kill(pid, signum)) < 0) 
		unix_error("Kill error");
}

handler_t *Signal(int signum, handler_t *handler)
{
	struct sigaction action, old_action;
	
	action.sa_handler = handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = SA_RESTART;
	
	if (sigaction(signum, &action, &old_action) < 0)
		unix_error("Signal error");
	return (old_action.sa_handler); 
}

unsigned int Alarm(unsigned int seconds)
{
	return alarm(seconds);
}
