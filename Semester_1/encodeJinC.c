/*
Caroline Jin
9/26/17
Period 1
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Node
{
	char symbol ;
	int frequency ;
	struct Node* left ;
	struct Node* right ;
	//
} TreeNode ;
static char enc[256][500]; 
void traverse(TreeNode* t, char path[]){
	if(t==NULL)
		return;
	if(t -> left == NULL && t-> right == NULL){
		strcpy(enc[(int)(t->symbol)], path);
	}
	
	char pathR[500];
	char pathL[500];
	strcpy(pathR, path);
	strcpy(pathL, path);
	strcat(pathL, "0");
	strcat(pathR, "1");
	traverse(t -> left, pathL);
	traverse(t -> right, pathR);
}
int main(){

	char mes[] = "Hello world from Caroline";
	//find the frequency of each character 
	int freq[256];
	memset(freq, 0, 256 * sizeof(int));
	for(int i = 0; i < 256; i++){
		memset(enc[i], 0, 500*sizeof(char));
	}
	int numChar = 0;
	for(int i = 0; i < (int)sizeof(mes)-1; i++){

		char input = mes[i];
		freq[(int) input]++;
		numChar++;

	}
	//make Treenode for each char

	TreeNode *nodes[256];
	int index = 0;

	
	for(int i = 0; i < 256; i++){

			if(freq[i]>0){
				TreeNode* t = NULL ;
				t = (TreeNode*)malloc( sizeof(TreeNode) );
				t -> symbol = (char)i;
				t -> frequency = freq[i];
				t -> left = NULL;
				t -> right = NULL; 
				nodes[index] = t;
				index++;

			}
	}

		//find the two minimum and shrink the array
	for(int size = index-1; size >=1; size--){
		int min = 1000000;
		int i1 = -1;

		int min2 = 1000000000;
		int i2 = -1;

		for(int i = 0; i <= size; i++){
			TreeNode* curr = nodes[i];
			int cFreq = curr -> frequency;

			if(cFreq < min){
				min2 = min;
				i2 = i1;

				min = cFreq;
				i1 = i;
			}
			
			else if(cFreq < min2){
				min2 = cFreq;
				i2 = i;
			}

		}
		
		//swap the two min into the end of the array
		TreeNode* temp = NULL;
		temp = nodes[size];
		nodes[size] = nodes[i1];
		nodes[i1] = temp;

		temp = nodes[size-1];
		nodes[size-1] = nodes[i2];
		nodes[i2] = temp;

		
		//minimize the array
		TreeNode* tNew = NULL ;
		tNew = (TreeNode*)malloc( sizeof(TreeNode) );
		tNew -> frequency = (nodes[size]-> frequency) + (nodes[size-1]-> frequency);
		tNew -> left = nodes[size-1];
		tNew -> right = nodes[size];
		nodes[size] = NULL;
		nodes[size-1] = tNew;

	}
	//traverse the tree to determine the scheme
	traverse(nodes[0], "");

	//encode the message
	printf("%d\n", index);
	for(int i = 0; i < 256; i++){
		if(strlen(enc[i])>0)
			printf("%c%s\n", (char)i, enc[i]);
	}

	for(int i = 0; i < numChar; i++){
		char inp = mes[i];
		printf("%s", enc[(int)inp]);
	}
	printf("\n");
	
	return 0;
}
/*
Hello world from Caroline

14
 001
C11101
H11100
a10010
d11001
e1000
f11000
i11010
l101
m1111
n11011
o01
r000
w10011
111001000101101010011001101000101110010011100000001111100111101100100000110111010110111000
*/