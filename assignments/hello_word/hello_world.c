#include <stdio.h>

int main(void)
{
	char msg[] = "Hello MIT OCW";
	float x=3/2;

	puts(msg);
	printf("%f\n",x);
	printf("int : %lu\n",sizeof(int));
	printf("float : %lu\n",sizeof(float));
	printf("double : %lu\n",sizeof(double));
	printf("short : %lu\n",sizeof(short));
	printf("double: %lu\n",sizeof(double));
	printf("long: %lu\n",sizeof(long));
	printf("char: %lu\n",sizeof(char));
	printf("size_t: %lu\n",sizeof(size_t));
}
