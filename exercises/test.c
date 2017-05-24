#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{

	void *p;
	int cnt = 0;
	while ((p = malloc(100*1024*1024))) {
		cnt++;
	} 
	printf("%d 100 M ¿Õ¼ä", cnt);
	return 0;
}
