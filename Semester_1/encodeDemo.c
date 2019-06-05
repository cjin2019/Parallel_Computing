//
// Torbert, 16 Sept 2015
//
#include <stdio.h>
#include <stdlib.h>
//
typedef struct Node
{
	char symbol ;
	//
	int frequency ;
	//
	struct Node* left ;
	struct Node* right ;
	//
} TreeNode ;
//
int main( int argc , char* argv[] )
{
	TreeNode* t = NULL ;
	//
	t = (TreeNode*)malloc( sizeof(TreeNode) );
	//
	(*t).symbol = 'A' ;
	t -> frequency = 7 ;
	t -> left = NULL ;
	t -> right = NULL ;
	//
	printf( "%c\n" , t->symbol ) ;
	printf( "%d\n" , t->frequency ) ;
	//
	return 0 ;
}
//
// end of file
//