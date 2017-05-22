#include <stdio.h>
#include <sys/stat.h>
#include <netinet/in.h>

int main(int argc, char **argv)
{
	int port;
	//char method[1024] = "GET";
	port = atoi(argv[1]);
	printf("port is %s", argv[1]);
	printf("%d \n", htons(16));
	//printf("ÄãºÃ"); 
	//int rc;
	//rc = open("home.html", O_RDONLY, 0);
}
