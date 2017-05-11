#include "../common.h"
void echo(int confd);

void sigchld_handler(int sig)
{
	while (waitpid(-1, 0, WNOHANG) > 0)
		;
	return;
}

int main(int argc, char **argv)
{
	int listenfd, connfd, port;
	socket_t clientlen = sizeof(struct sockadd_in);
	struct sockaddr_in clientaddr;
	
	if (argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(0);
	}
	port = atoi(argv[1]);
	
	Signal(SIGCHLD, sigchld_handler);
	listenfd = Open_listenfd(port);
	while (1) {
		connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);
		if (Fork() == 0) {
			Close(listenfd);
			echo(confd);
			Close(connfd);
			exit(0);
		}
		Close(connfd);
	}
}