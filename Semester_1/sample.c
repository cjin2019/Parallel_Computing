#include<stdio.h>
#include<stdlib.h>
double *out(){
	double *ret = malloc(2);
	ret[0]=0.0;
	ret[1]=1.0;
	return ret;
}
int main(){
	double *p = out();
	free(p);
	printf("%f\n", p[0]);
	return 0;
}