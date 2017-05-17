#include "../common.h"
#include "sbuf.h"
#define NTHREADS 4
#define SBUFSIZE 16

void echo_cnt(int connfd);
void *thread(void *vargp);

sbuf_t sbuf;

int main(int argc, char **argv)
{
	int i, listenfd, connfd, port;
	socklen_t clientlen = sizeof(struct sockaddr_in);
	struct sockaddr_in clientaddr;
	pthread_t tid;
	
	if (argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(0);
	}
	port = atoi(argv[1]);
	sbuf_init(&sbuf, SBUFSIZE);
	listenfd = Open_listenfd(port);
	
	for (i = 0; i < NTHREADS; i++)
		Pthread_create(&tid, NULL, thread, NULL);
	
	while (1) {
		connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
		sbuf_insert(&sbuf, connfd);
	}
}

void *thread(void *vargp)
{
	Pthread_detach(pthread_self());
	while (1) {
		int connfd = sbuf_remove(&sbuf);
		echo_cnt(connfd);
		Close(connfd);
	}
}
