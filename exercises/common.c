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

// process
pid_t Fork(void)
{
	pid_t pid;
	
	if ((pid = fork()) < 0)
	    unix_error("Fork error");
	return pid;	
}

void Execve(const char *filename, char *const argv[], char *const envp[])
{
	if (execve (filename, argv, envp) < 0)
		unix_error("Execve error");
}

pid_t Wait(int *status)
{
	pid_t pid;
	
	if ((pid = wait(status)) < 0)
		unix_error("Wait error");
	return pid;
}

int Dup2(int fd1, int fd2)
{
	int rc;
	
	if ((rc = dup2(fd1, fd2)) < 0)
		unix_error("Dup2 error");
	return rc;
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

// signal
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

// buffer
void rio_readinitb(rio_t *rp, int fd)
{
	rp->rio_fd = fd;
	rp->rio_cnt = 0;
	rp->rio_bufptr = rp->rio_buf;
}

static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
{
	int cnt;	
	
	while (rp->rio_cnt <= 0) {
		rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
		if (rp->rio_cnt < 0) {
			if  (errno != EINTR) return -1;
		} else if (rp->rio_cnt == 0) return 0;
		else rp->rio_bufptr = rp->rio_buf;
	}
	
	cnt = n;
	if (rp->rio_cnt < n) cnt = rp->rio_cnt;
	memcpy(usrbuf, rp->rio_bufptr, cnt);
	rp->rio_bufptr += cnt;
	rp->rio_cnt -= cnt;
	return cnt; 
}

ssize_t rio_written(int fd, void *usrbuf, size_t n)
{
	size_t nleft = n;
	ssize_t nwritten;
	char *bufp = usrbuf;
	
	while (nleft > 0) {
		if ((nwritten = write(fd, bufp, nleft)) <=0) {
			if (errno == EINTR) nwritten = 0;
			else return -1; 
		}
		nleft -= nwritten;
		bufp += nwritten;
	}
	return n;
}

ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
	int n, rc;
	char c, *bufp = usrbuf;
	
	for (n = 1; n < maxlen; n++) {
		if (rc = (rio_read(rp, &c, 1)) == 1) {
			*bufp++ = c;
			if (c == '\n') break;
		} else if (rc == 0) {
			if (n == 1) return 0;
			else break;
		} else return -1;
	}
} 

void Rio_written(int fd, void *usrbuf, size_t n)
{
	if (rio_written(fd, usrbuf, n) != n) unix_error("Rio_written error");
}

// memory
void *Mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
	void *ptr;
	
	if ((ptr = mmap(addr, len, prot, flags, fd, offset)) == ((void *) - 1 ))
		unix_error("mmap error");
	return (ptr);
}

void Munmap(void *start, size_t length)
{
	if (munmap(start, length) < 0)
		unix_error("munmap error");
}


// file
char *Fgets(char *ptr, int n, FILE *stream)
{
	char *rptr;
	
	if (((rptr = fgets(ptr, n, stream)) == NULL) && ferror(stream))
		app_error("Fgets error");
		
	return rptr;
}

int Open(const char *pathname, int flags, mode_t mode)
{
	int rc;
	
	if ((rc = open(pathname, flags, mode)) < 0)
		unix_error("Open error");
	return rc;
}

void Close(int fd)
{
	int rc;
	
	if ((rc = close(fd)) < 0)
		unix_error("close error");
}

