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
