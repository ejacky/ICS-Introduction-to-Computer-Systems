#include <signal.h> 
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <setjmp.h>

#define MAXLINE 8192
#define MAXBUF  8192
extern char **environ;
typedef void handler_t(int);
