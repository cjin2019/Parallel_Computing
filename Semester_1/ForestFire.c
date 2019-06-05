#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#define r 30
#define c 40
double myrand()
{
   return ( rand() % 100 ) / 100.0 ;
}
double gettime()
{
	double t ;
	struct timeval* ptr = (struct timeval*)malloc( sizeof(struct timeval) ) ;
	gettimeofday( ptr , NULL ) ; // second argument is time zone... NULL
	t = ptr->tv_sec * 1000000.0 + ptr->tv_usec ;
	free( ptr ) ;
	return t / 1000000.0 ;
}
typedef struct Node
{
	int row;
	int col;
	int level;
	//
} TreeNode ;
typedef struct ListNode
{
    int row ;
    int col ;
    //
    struct ListNode* next ;
    //
} Node ;

int burn(char forest[r][c]){
	//put all the node into the left-column into 
	TreeNode* list[1000000];
	int step = 0;
	//left column
	for(int i = 0; i < r; i++){
		if(forest[i][0] == '+'){
			//add to the list
			TreeNode* t = (TreeNode*)malloc( sizeof(TreeNode) );
			t->row = i;
			t->col = 0;
			t->level = 0;

			forest[i][0]= ' ';

			list[step] = t;

			step++;
		}
	}

	//BFS
	int lastIndex = step; //empty spot available, so counts the number of steps
	step = 0;
	int numSteps = 0; //finds the actual number of steps
	while(step < lastIndex){
		TreeNode* curr = list[step];
		int currR = curr->row;
		int currC = curr->col;
		//up, down, left, right
		int moveC[4] = {0, 0, -1, 1};
		int moveR[4] = {1, -1, 0, 0};

		for(int i = 0; i < 4; i++){
			//out of bounds
			if(currR + moveR[i] < 0 || currR + moveR[i] >= r 
				|| currC + moveC[i] < 0 || currC + moveC[i] >=c){
				continue;
			}
			//burn and add to list
			if(forest[currR + moveR[i]][currC + moveC[i]]=='+'){
				forest[currR + moveR[i]][currC + moveC[i]] = ' ';

				TreeNode* t = (TreeNode*)malloc( sizeof(TreeNode) );
				t->row = currR + moveR[i];
				t->col = currC + moveC[i];
				t->level = (curr->level)+1;

				numSteps = t->level;
				list[lastIndex] = t;
				lastIndex++;
			}	
		}
		step++;
	}
	return numSteps;
}
int main(){
	//random seed
	int rseed = time(NULL);
	srand(rseed);
    printf("%d", rseed);
	printf( "%10s%10c\n" , "Prob", 'P') ;
	//probability
	for(double prob = 0.0; prob <= 1.01; prob+=.01){
	//each trial
		double sumP = 0.0;
		double sumT = 0.0;
		int numTrials = 1000;
		for(int trial = 1; trial <= numTrials; trial++){
			//initialize forest
			char forest[r][c];
			for(int i = 0; i < r; i++){
				for(int j = 0; j < c; j++){
					if(myrand() < prob)
						forest[i][j] = '+';
					else
						forest[i][j] = ' ';
					//printf("%c", forest[i][j]);
				}
				//printf("\n");
			}

			//simulation
			//start from left-hand and go up, down, left, right
			double start = gettime();
			int numSteps = burn(forest);
			double end = gettime();

			//output
			sumP+=numSteps/c * 1.0;
			//sumT+=(end-start);
		}
		printf("%f%10f\n", prob, sumP/1000);
	}
	return 0;
}
