#include <stdio.h>
//int loop_while(int a, int b)
//{
//	int result = 1;
//	while (a < b) {
//		result *= (a + b);
//		a++;
//	}
//	return result;
//}
//
//int main()
//{
//	loop_while(3, 4);
//}

long int simple_1(long int *xp, long int y)
{
	long int t = *xp + y;
	*xp = t;
	return t;
}

long int gval1 = 567;
long int gval2 = 763;

long int call_simple_1()
{
	long int z = simple_1(&gval1, 12L);
	return z + gval2;
}

