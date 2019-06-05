#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct Node
{
	int row;
	int col;
	int level;
	//
} TreeNode ;
int main(){
	//TRYING TO INPUT
	FILE* fin;
	fin= fopen("prefSum.txt", "r");

	char buff[255];
	int n; 

	fscanf(fin, "%s", buff);
	n = atoi(buff);
	int t;

	int rows = (int)(log(n)/log(2.0)) + 1;//fix this later

	int tree[rows][n];

	int l; 

	int c; 
	
	for(l = 1; l <=n; l++){
		fscanf(fin, "%s", buff);
		tree[0][l] = atoi(buff);
		//printf("%d %d\n", tree[0][l], l);
	}

	//MAKES THE TREE
	for(int r = 1; r <= rows; r++){//start on the second to last row
		double cMax = pow(2, r);
		int end = (int)(n/pow(2.0, r));
		 
		#pragma omp parallel for private(c, t)
		for(c = 1; c <= end; c++){
			t = c; 
			tree[r][c]=tree[r-1][2*c-1] + tree[r-1][2*c];
			//printf("%d\n", tree[r][c]);
		}
	}

	//TRIES TO SOLVE THE PREFIX
	int prefixSum[rows][n]; 

	for(int r = rows; r>= 0; r--){
		int end = (int)(n/pow(2.0, r));

		#pragma omp parallel for private(c, t)
		for(c = 2; c <=end; c+=2){
			t = c; 
			prefixSum[r][c] = prefixSum[r+1][c/2];
		}
		prefixSum[r][1] = tree[r][1];
		
		#pragma omp parallel for private(c, t)
		for(c = 3; c <= end; c+=2){
			t = c; 
			prefixSum[r][c] = prefixSum[r+1][(c-1)/2]+tree[r][c];
		}
	}


	//OUTPUT
	//#pragma omp parallel for private(c, t)
	for(int c = 1; c <=n; c++){
		

		//t = c; 
		printf("%d\n", prefixSum[0][c]);
	}
	//xmt pdf on website starting at page 20
	/*
	1. start with the operator (min)
	2. prefix sum (prefix min)
	3. pardo (parallel do)
		a. openMP - # program
		b. for loop for the levels --> don't do parallel
		c. cuDa - <<< >>>--> transfer data
	*/
	return 0; 
}