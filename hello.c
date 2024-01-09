#include <stdio.h>

int main(void) {
	FILE *fp;

	fp = fopen("hello.txt", "r");

	int c = fgetc(fp);
	printf("%c\n", c);
	
	fclose(fp);
}
