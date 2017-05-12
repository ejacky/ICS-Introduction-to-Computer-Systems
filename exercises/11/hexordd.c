#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void app_error(char *msg)
{
	fprintf(stderr, "%s\n", msg);
}

void hex2dd()
{
	
}

//void dd2hex(const char *cp, struct in_addr *inp)
//{
//	unsigned int addr;
//	if (inet_aton(&cp, &inp) == 0)
//		app_error("inet_aton error");
//	addr = ntohl(inp.s_addr); 
//	printf("0x%x\n", addr);
//}

int main(int argc, char **argv) 
{
	struct in_addr inaddr;
	unsigned int addr;
	
	if (argc != 2) {
		fprintf(stderr, "usaged: %s <ip or hex>", argv[0]);  // why printf?
		exit(0);
	}	
	
	// inet_aton
	//dd2hex(argv[1], &inaddr); something wrong


	// inet_htonl
	sscanf(argv[1], "%x", &addr);
	inaddr.s_addr = htonl(addr);
	printf("%s\n", inet_ntoa(inaddr));
	
	exit(0);
	
}
