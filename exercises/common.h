#include <stdio.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <setjmp.h>
#include <signal.h>

/* Í·ÎÄ¼þ */
#define MAXLINE 8192
#define MAXBUF 8192
#define RIO_BUFSIZE 8192

extern char **environ;
typedef void handler_t(int);

typedef struct {
	int rio_fd;
	int rio_cnt;
	char *rio_bufptr;
	char rio_buf[RIO_BUFSIZE];
} rio_t;
