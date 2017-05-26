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

typedef struct {
    struct sockaddr_in addr;
    socklen_t addrlen;
    int connfd;
    char host[HOSTLEN];
    char serv[SERVLEN];
} client_info;

// int parse_uri(char *uri, char *port, char *cgiargs) 
// {
//     char *ptr;

//     if (!strstr(uri, "cgi-bin")) {  /* Static content */ //line:netp:parseuri:isstatic
// 	strcpy(cgiargs, "");                             //line:netp:parseuri:clearcgi
// 	strcpy(filename, ".");                           //line:netp:parseuri:beginconvert1
// 	strcat(filename, uri);                           //line:netp:parseuri:endconvert1
// 	if (uri[strlen(uri)-1] == '/')                   //line:netp:parseuri:slashcheck
// 	    strcat(filename, "home.html");               //line:netp:parseuri:appenddefault
// 	return 1;
//     }
//     else {  /* Dynamic content */                        //line:netp:parseuri:isdynamic
// 	ptr = index(uri, '?');                           //line:netp:parseuri:beginextract
// 	if (ptr) {
// 	    strcpy(cgiargs, ptr+1);
// 	    *ptr = '\0';
// 	}
// 	else 
// 	    strcpy(cgiargs, "");                         //line:netp:parseuri:endextract
// 	strcpy(filename, ".");                           //line:netp:parseuri:beginconvert2
// 	strcat(filename, uri);                           //line:netp:parseuri:endconvert2
// 	return 0;
//     }
// }

void *handle(void *arg)
{
    int is_static;
    int clientfd;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE], buf2[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio;


    client_info *client = arg;

    char *hostname = "localhost";
    char *port = "5002";
    char *method2 = "GET";
    char *uri2 = "/godzilla.jpg";
    char *version2 = "HTTP/1.0";
    
    Pthread_detach(pthread_self());

    Getnameinfo((SA *) &client->addr, client->addrlen,
     client->host, sizeof(client->host),
     client->serv, sizeof(client->serv),
     0);
    
    printf("Accepted connection from %s:%s\n", client->host, client->serv);

    Rio_readinitb(&rio, client->connfd);
    if (!Rio_readlineb(&rio, buf, MAXLINE))
        return;
    printf("%s", buf);
    sscanf(buf, "%s %s %s", method, uri, version); 
    //is_static = parse_uri(uri, filename, cgiargs); 

    clientfd = Open_clientfd(hostname, port);
    Rio_readinitb(&rio, clientfd);
    sprintf(buf2, "%s %s %s\r\n", method2, uri2, version2);
    Rio_writen(clientfd, buf2, strlen(buf2));

    while (fgets(buf2, MAXLINE, clientfd) != NULL) {

        Rio_readlineb(&rio, buf2, MAXLINE);
        printf("%s", buf);
        Rio_writen(client->connfd, buf2, strlen(buf2));

    }


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
