#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#define N 7

void unix_error(char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(0);
}

pid_t Fork(void)
{
	pid_t pid;
	
	if ((pid = fork()) < 0)
	    unix_error("Fork error");
	return pid;	
}

void fork1()
{
	pid_t pid;
	int x = 1;
	
	pid = Fork();

	if (pid == 0) {
		printf("child : x=%d\n", ++x);
		exit(0);
	}
	
	printf("parent: x=%d\n", --x);
	exit(0);
}

void waitpid1()
{
	int status, i;
	pid_t pid;
	
	for (i = 0; i < N; i++)
		if ((pid = Fork()) == 0)
			exit(100 + i);
			
	while ((pid = waitpid(-1, &status, 0)) > 0) {
		if (WIFEXITED(status))
			printf("child %d terminated normally with exit status= %d \n", pid, WEXITSTATUS(status));
		else 
			printf("child %d terminated abnormally \n", pid);
	}
	
	if (errno != ECHILD)
		unix_error("waitpid error");
		
	exit(0);
}
 
void waitpid2()
{
	int status, i;
	pid_t pid[N], retpid;
	
	for (i = 0; i < N; i++)
		if ((pid[i] = Fork()) == 0)
			exit(100 + i);
	
	i = 0;
	while ((retpid = waitpid(pid[i++], &status, 0)) > 0) {
		if (WIFEXITED(status)) 
				printf("child %d terminated normally with exit status= %d \n", retpid, WEXITSTATUS(status));
		else
				printf("child %d terminated abnormally \n", retpid);
	}
	
	if (errno != ECHILD)
		unix_error("waitpid error");
		
	exit(0);	
}

int main()
{
	waitpid2();

}
