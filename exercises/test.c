#include <stdio.h>
#include <stdlib.h>

int test()
{
	void *p;
	int cnt = 0;
	
	FILE *fp;
	fp = fopen("test.txt", "w+");
	fprintf(fp, "hello world!"); 
	fclose(fp);
}

void sum()
{
	int i, sum;
	
	for (i = 0; i < 100000; i++)
	{
		sum += i / 2;
	}
	printf("sum = %d\n", sum);
}

int main(int argc, char **argv)
{

//	char *i = "a";
//	char *j = "b";
	 int i = 123;
	 int j = 455;

	
	printf("i addr = %p\n", &i);
	printf("j addr = %p\n", &j);

	return 0;
}


