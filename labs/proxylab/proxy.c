#include <stdio.h>
#include "csapp.h"
#include <netdb.h>

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400
#define HOSTLEN 256 
#define SERVLEN 8

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

static sem_t mutex;

typedef struct {
    struct sockaddr_in addr;
    socklen_t addrlen;
    int connfd;
    char host[HOSTLEN];
    char serv[SERVLEN];
} client_info;

static void init_mutex(void)
{
    Sem_init(&mutex, 0, 1);
}

int open_clientfd_w(char *hostname, int port)
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

int Open_clientfd_w(char *hostname, int port)
{
	int rc;
	
	if ((rc = open_clientfd_w(hostname, port)) < 0) {
		if (rc == -1)
			printf("Open_clientfd Unix error\r\n");
		else 
			printf("Open_clienfd DNS error\r\n");
	}
	return rc;
} 

ssize_t Rio_readn_w(int fd, void *ptr, size_t nbytes) 
{
    ssize_t n;
  
    if ((n = rio_readn(fd, ptr, nbytes)) < 0) {
        printf("Rio_readn error\n");
        return 0;
    }
    return n;
}

ssize_t Rio_writen_w(int fd, void *usrbuf, size_t n) 
{
    if (rio_writen(fd, usrbuf, n) != n) {
	    printf("Rio_writen error\n");
        return 0;
    }
    return n;
}

void Rio_readinitb_w(rio_t *rp, int fd)
{
    rio_readinitb(rp, fd);
} 

ssize_t Rio_readnb_w(rio_t *rp, void *usrbuf, size_t n) 
{
    ssize_t rc;

    if ((rc = rio_readnb(rp, usrbuf, n)) < 0) {
        printf("Rio_readnb error\n");
        return 0;
    }
    return rc;
}

ssize_t Rio_readlineb_w(rio_t *rp, void *usrbuf, size_t maxlen) 
{
    ssize_t rc;

    if ((rc = rio_readlineb(rp, usrbuf, maxlen)) < 0) {
        printf("Rio_readlineb error\n");
        return 0;
    }
    return rc;
} 

/*
 * parse_uri - URI parser
 * 
 * Given a URI from an HTTP proxy GET request (i.e., a URL), extract
 * the host name, path name, and port.  The memory for hostname and
 * pathname must already be allocated and should be at least MAXLINE
 * bytes. Return -1 if there are any problems.
 */
int parse_uri(char *uri, char *hostname, char *pathname, int *port)
{
    char *hostbegin;
    char *hostend;
    char *pathbegin;
    int len;

    if (strncasecmp(uri, "http://", 7) != 0) {
        hostname[0] = '\0';
        return -1;
    }

    /* Extract the host name */
    hostbegin = uri + 7;
    hostend = strpbrk(hostbegin, " :/\r\n\0");
    if (!hostend) { /* Fix a bug when uri only contains hostname and port, without trailing '/' */
        strcat(uri, "/");
        hostend = strpbrk(hostbegin, " :/\r\n\0");
    }
    len = hostend - hostbegin;
    strncpy(hostname, hostbegin, len);
    hostname[len] = '\0';
    
    /* Extract the port number */
    *port = 80; /* default */
    if (*hostend == ':')
        *port = atoi(hostend + 1);
    
    /* Extract the path */
    pathbegin = strchr(hostbegin, '/');
    if (pathbegin == NULL) {
        pathname[0] = '\0';
    }
    else {
        pathbegin++;
        strcpy(pathname, pathbegin);
    }

    return 0;
}

/*
 * format_log_entry - Create a formatted log entry in logstring.
 * 
 * The inputs are the socket address of the requesting client
 * (sockaddr), the URI from the request (uri), and the size in bytes
 * of the response from the server (size).
 */
void format_log_entry(char *logstring, struct sockaddr_in *sockaddr, char *uri, int size)
{
    time_t now;
    char time_str[MAXLINE];
    unsigned long host;
    unsigned char a, b, c, d;

    /* Get a formatted time string */
    now = time(NULL);
    strftime(time_str, MAXLINE, "%a %d %b %Y %H:%M:%S %Z", localtime(&now));

    /*
     * Convert the IP address in network byte order to dotted decimal
     * form. Note that we could have used inet_ntoa, but chose not to
     * because inet_ntoa is a Class 3 thread unsafe function that
     * returns a pointer to a static variable (Ch 13, CS:APP).
     */
    host = ntohl(sockaddr->sin_addr.s_addr);
    a = host >> 24;
    b = (host >> 16) & 0xff;
    c = (host >> 8) & 0xff;
    d = host & 0xff;


    /* Return the formatted log entry string */
    sprintf(logstring, "%s: %d.%d.%d.%d %s %d", time_str, a, b, c, d, uri, size);
}

void *handle(void *arg)
{
    int clientfd;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE], buf2[MAXLINE];
    char *cache_dir = ".cache/"; 
    char cache_path[MAXLINE];
    char hostname[MAXLINE];
    int  port, n;
    char filename[MAXLINE];
    rio_t rio, rio1;


    client_info *client = arg;

    Pthread_detach(pthread_self());

    Getnameinfo((SA *) &client->addr, client->addrlen,
     client->host, sizeof(client->host),
     client->serv, sizeof(client->serv),
     0);
    
    printf("Accepted connection from %s:%s\n", client->host, client->serv);
    
    Rio_readinitb_w(&rio, client->connfd);
    if (!Rio_readlineb_w(&rio, buf, MAXLINE))
        printf("cannot read\r\n");
    printf("%s", buf);
    sscanf(buf, "%s %s %s", method, uri, version); 
    parse_uri(uri, hostname, filename, &port); 
    printf("hostname: %s filename: %s port: %d\r\n", hostname, filename, port);
    
    FILE *fin, *fout;
    sprintf(cache_path, "%s%s", cache_dir, filename);
    printf("cache path: %s\r\n", cache_path);
    static pthread_once_t once = PTHREAD_ONCE_INIT;

    Pthread_once(&once, init_mutex);
    if( access( cache_path, F_OK ) != -1 ) {
    // file exists
        int c;
        printf("just haha: \r\n");
        P(&mutex);
        fin = fopen(cache_path, "r");
        printf("just new  haha: \r\n");

        while(1) {
            c = fgetc(fin);
            if( feof(fin) )
            { 
                break ;
            }
            printf("just haha: \r\n");

            Rio_writen(client->connfd, c, 1);
        }
        printf("you get cache\r\n");
        fclose(fin);
        V(&mutex);
    } else {
        // file doesn't exist
        clientfd = Open_clientfd_w(hostname, port);
        //Rio_readinitb_w(&rio, client->connfd);
        sprintf(buf, "%s /%s %s\r\n", method, filename, version);
        Rio_writen(clientfd, buf, strlen(buf));

        while (strcmp(buf, "\r\n")) {
            Rio_readlineb_w(&rio, buf, MAXLINE);
            printf("%s", buf);
            Rio_writen(clientfd, buf, strlen(buf));
        }
        Rio_readinitb_w(&rio1, clientfd);
            
        int flag = 0;
        P(&mutex);
        fout = fopen(cache_path, "w+");
        while ((n = Rio_readlineb_w(&rio1, buf2, MAXLINE)) != 0) {
            if (Rio_writen_w(client->connfd, buf2, n) != n ) // 没有这个， 二进制文件无法获取
                break;
            if (strcmp(buf2, "\r\n") == 0) {
                flag = 1;
            }
            if (flag) {
                fprintf(fout, buf2);
            }
        }
        fclose(fout);
        V(&mutex);

        Rio_writen(client->connfd, "\r\n", 2);

        printf("haha \r\n");
        Close(clientfd);
    }
    
    //char logstring[MAXLINE];
    //format_log_entry(char *logstring, struct sockaddr_in *sockaddr, char *uri, int size);
    Close(client->connfd);
    return NULL;
}

int main(int argc, char **argv)
{
    // as server
    int listenfd;
    char *port;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    port = argv[1];

    listenfd = Open_listenfd(port);

    while (1) {
        pthread_t thread;

        client_info *client = Malloc(sizeof(*client));

        client->addrlen = sizeof(client->addr);

        client->connfd = Accept(listenfd, (SA *)&client->addr, &client->addrlen);

        pthread_create(&thread, NULL, handle, client);

    }

    // as client
    printf("%s", user_agent_hdr);
    return 0;
}
