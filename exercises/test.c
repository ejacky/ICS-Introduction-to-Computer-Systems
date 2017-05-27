#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{

	void *p;
	int cnt = 0;
	
	FILE *fp;
	fp = fopen("test.txt", "w+");
	fprintf(fp, "hello world!"); 
	fclose(fp);
	return 0;
}
