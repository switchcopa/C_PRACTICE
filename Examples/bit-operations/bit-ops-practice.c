#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

char* get_binary(int x);

int main(void) {
	int x;

	x = 14;
	char* b = get_binary(x);
	printf("%s\n", b);

	free(b);
	return 0;
}

char* get_binary(int x) {
	int i, size;
	char* b;
	
	size = 32, i = size - 1; 
	b = malloc(sizeof(char) * (size + 1)); // assume a 32-bit int
	if (!b) {
		printf("failed to allocate memory for buffer\n");
		exit(1);
	}	
	
	while (x > 0)	{
		b[i--] = (x % 2) + '0';
		x /= 2;
	}

	b[size] = '\0';

	char* result = strdup(&b[i + 1]);
	free(b);	

	return result;
}
