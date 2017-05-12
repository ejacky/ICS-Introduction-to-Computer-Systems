#include "../common.h"

int main(int argc, char **argv)
{
	char **pp;
	struct in_addr addr;
	struct hostent *hostp;
	
	if (argc != 2) {
		fprintf(stderr, "usage: %s <domin name>\n", argv[0]);
		exit(0);
	}
	
	if (inet_aton(argv[1], &addr) != 0) 
	    hostp = gethostbyaddr((const char *)&addr, sizeof(addr), AF_INET);
	else
	    hostp = gethostbyname(argv[1]);
	
	printf("official hostname : %s\n", hostp->h_name);
	
	for (pp = hostp->h_addr_list; *pp != NULL; pp++)
	    printf("alias: %s\n", *pp);

    for (pp = hostp->h_addr_list; *pp != NULL; pp++) {
        addr.s_addr = ((struct in_addr *)*pp)->s_addr;
		printf("address: %d\n", inet_ntoa(addr));
	}
	exit(0);
}
