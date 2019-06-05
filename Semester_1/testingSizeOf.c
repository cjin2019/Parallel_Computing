#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(){
	char message[] = "Hello!";
	printf("%s\n", message);
	printf("%d\n", (int)sizeof(message)-1);
	return 0;
}