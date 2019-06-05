#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int binarySearch(int array[], int num, int start, int end){
	if(array[start] > num)
		return start;
	if(array[end-1] < num)
		return end;
	int mid = (start + end)/2; 
	if(array[mid] == num)
		return mid; 
	else{
		if(array[mid] < num)
			return binarySearch(array, num, start, mid);
		if(array[mid]>num)
			return binarySearch(array, num, mid, end); 
	}

}
void mergeSort(int A[], int B[], int len){
	if(len==1)
		return 
}

void merge(int a[], int newA[], int a1, int a2, int b1, int b2){
	int currA = a1;
	int currB = b1;

	for(int i = min(a1, b1); i < max())
	{
		if(currA >)
	
	}
}
int main(){
	/*FILE* fin;
	fin= fopen("prefSum.txt", "r");

	char buff[255];
	int n; 

	fscanf(fin, "%s", buff);
	n = atoi(buff);*/

	int num = 9;
	int array[5] = {2, 4, 6, 8, 10};
	printf("%d\n", binarySearch(array, num));
	return 0; 
}