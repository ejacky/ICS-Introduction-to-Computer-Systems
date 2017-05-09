#include "../common.h"

void sigint_handler(int sig)
{
	printf("想用 ctr +　c 关闭我？ ");
	sleep(2);
	fflush(stdout);
	sleep(1);
	printf("OK. :)\n");
	exit(0);
}

int main()
{
	if (signal(SIGINT, sigint_handler) == SIG_ERR)
		unix_error("signal_error");
		
	pause();
	return 0;
}
