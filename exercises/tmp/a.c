#include <stdio.h>
static int a = 1;
int b; 
int c;

int main() 
{
    if (b == 1) {
        c = foo();
	b = 115;
	printf("%d", c);
    }
    if (a == 1) {
	printf("%c", b);
	return 1;  
    } else if(c == 2) {
	printf("%d", a);
	return 2;
    }
    return 0;
}
