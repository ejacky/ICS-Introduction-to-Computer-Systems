#include "common.h"

// error handler
void unix_error(char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(0);
}

void app_error(char *msg)
{
	fprintf(stderr, "%s\n", msg);
}

void dns_error(char *msg)
{
	fprintf(stderr, "%s: DNS error %d\n", msg, h_errno);
	exit(0);
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


// Unix I/O
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

ssize_t Read(int fd, void *buf, size_t n)
{
	ssize_t rc;
	
	if ((rc = read(fd, buf, n)) < 0) 
		unix_error("Read error");
	return rc;
}

ssize_t Write(int fd, const void *buf, size_t n)
{
	ssize_t rc;
	
	if ((rc = write(fd, buf, n)) < 0)
		unix_error("Write error");
	return rc;
}

// Standard I/O
char *Fgets(char *ptr, int n, FILE *stream)
{
	char *rptr;
	
	if (((rptr = fgets(ptr, n, stream)) == NULL) && ferror(stream))
		app_error("Fgets error");
		
	return rptr;
}

void Fputs(const char *ptr, FILE *stream)
{
	if (fputs(ptr, stream) == EOF)
	    unix_error("Fputs error");
}

// Robustly I/O
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

ssize_t rio_writen(int fd, void *usrbuf, size_t n)
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

void Rio_readinitb(rio_t *rp, int fd)
{
	rio_readinitb(rp, fd);
}

//ssize_t Rio_readnb(rio_t *rp, void *usrbuf, size_t n)
//{
//	ssize_t rc;
//	
//	if ((rc = rio_readnb(rp, usrbuf, n)) < 0)
//		unix_error("Rio_readnb error");
//	return rc;
//}

ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
	ssize_t rc;
	
	if ((rc = rio_readlineb(rp, usrbuf, maxlen)) < 0)
		unix_error("Rio_readlineb error");
	return rc;
} 

void Rio_writen(int fd, void *usrbuf, size_t n)
{
	if (rio_writen(fd, usrbuf, n) != n) unix_error("Rio_written error");
}

// Socket interface
int Accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
	int rc;
	
	if ((rc = accept(s, addr, addrlen)) < 0)
        unix_error("Accept error");
    return rc;
}


// Client/Server function
int open_listenfd(int port)
{
	int listenfd;
	int optval = 1;
	struct sockaddr_in serveraddr;
	
	// socket
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
		return -1;
		
	// setsockopt
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int)) < 0)
		return -1; 
		
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short)port);
	
	// bind
	if (bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
		return -1;
	}
	
	// listen
	if (listen(listenfd, 1024) < 0)
		return -1;
	
	return listenfd;
}

int open_clientfd(char *hostname, int port)
{
	int clientfd;
	struct hostent *hp;
	struct sockaddr_in serveraddr;
	
	if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return -1;
	
	if ((hp = gethostbyname(hostname)) == NULL)
		return -2;
	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	bcopy((char *)hp->h_addr_list[0], (char *)&serveraddr.sin_addr.s_addr, hp->h_length);
	serveraddr.sin_port = htons(port);
	
	if (connect(clientfd, (SA *) &serveraddr, sizeof(serveraddr)) < 0)
		return -1;
	return clientfd;
}

int Open_listenfd(int port)
{
	int rc;
	
	if ((rc = open_listenfd(port)) < 0)
		unix_error("Open_listenfd error");
	return rc;
}

int Open_clientfd(char *hostname, int port)
{
	int rc;
	
	if ((rc = open_clientfd(hostname, port)) < 0) {
		if (rc == -1)
			unix_error("Open_clientfd Unix error");
		else 
			dns_error("Open_clienfd DNS error");
	}
	return rc;
}

// DNS interface
struct hostent *Gethostbyname(const char *name)
{
	struct hostent *p;
	
	if ((p = gethostbyname(name)) == NULL)
		dns_error("Gethostbyname error");
	
	return p;
}

struct hostent *Gethostbyaddr(const char *addr, int len, int type)
{
    struct hostent *p;

    if ((p = gethostbyaddr(addr, len, type)) == NULL)
        dns_error("Gethostbyaddr error");
    return p;
}



