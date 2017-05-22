#include "../common.h" 

void echo(int connfd);
void *thread(void *vargp);

int main(int argc, char **argv)
{
	int listenfd, *connfdp, port;
	socklen_t clientlen = sizeof(struct sockaddr_in);
	struct sockaddr_in clientaddr;
	pthread_t tid;
	
	if (argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(0);
	}
	port = atoi(argv[1]);
	listenfd = Open_listenfd(port);
	while (1) {
		connfdp = Malloc(sizeof(int));
		*connfdp = Accept(listenfd, (SA *)&clientaddr, &clientlen);
		Pthread_create(&tid, NULL, thread, connfdp);
	}
}

void *thread(void *vargp)
{
	int connfd = *((int *)vargp);
	pthread_id_np_t   tid;
	pthread_t         self;	
    self = pthread_self();
	pthread_getunique_np(&self, &tid);
	
	Pthread_detach(self);
	fprintf(stderr, "thread_id: %ld \n", tid);
	Free(vargp);
	echo(connfd);
	Close(connfd);
	return NULL;
}
