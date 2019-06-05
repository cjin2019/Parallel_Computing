#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
int main(){
	FILE* fin;
	fin= fopen("decodeME.txt", "r");
	char input[300];

	fgets(input, 200, fin);

	int num;
	num = atoi(input);
	
	char dec[1900000];
	memset(dec, 0, 1900000);

	for(int i = 0; i < num; i++){

		fgets(input, 200, fin);
		//printf("%s\n", input);
		char ch = input[0];
		int index = 1;
		int j = 1;
		if(ch=='\n'){
			fgets(input, 200, fin);
			j = 0;
		}
		
		
		while(input[j]== '0' || input[j]=='1'){
			//printf("%c\n", ch);
			if(input[j]=='0'){
				index = 2 * index;
			}
			else{
				index = 2 * index + 1;
			}
			j++;
		}
		//printf("%d\n", index);
		dec[index] = ch;
	}
	
	int index = 1;
	char ch = fgetc(fin);

	int freq[256];
	memset(freq, 0, 256 * sizeof(int));
	int numBits = 1;
	int numLetters = 1;

	while(ch == '0' || ch == '1'){

		if(ch=='0'){
			index = 2 * index;
		}
		else{
			index = 2 * index+1;
		}
		numBits++;

		if(dec[index]!=0){

			printf("%c", dec[index]);
			freq[dec[index]]++;
			numLetters++;
			index = 1;

		}
		ch = fgetc(fin);
	}

	printf("\n");
	float sum = 0.0;
	for(int i = 0; i < 256; i++){
		if(freq[i]>0){
			float prob = freq[i] * 1.0 / numLetters;
			sum += (freq[i] * (-log(prob)/log(2.0)) );

		}
	}
	/*printf("\n\n");
	printf("Compression ratio: %f\n\n", (8.0 * numLetters - numBits)/(8.0 * numLetters));	
	printf("Shannon: %f\n", sum);*/
}
