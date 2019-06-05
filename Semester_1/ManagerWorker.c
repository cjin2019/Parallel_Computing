// 
// Torbert, 14 November 2016
// 
// MPI Demo
//    mpicc mpiDemo.c
//    time mpirun -np 4                                  a.out
//    time mpirun -np 4 --mca orte_base_help_aggregate 0 a.out
//    time mpirun -np 4 -mca btl ^openib                 a.out
// 
//    time mpirun -np 6 -machinefile hosts.txt a.out
// 
// Manager-Worker model for parallel processing.
// 
// 2 4   0.6283180000000000
// 3 4   0.9424770000000000
// 1 4   0.3141590000000000
// 
// real    0m1
// sys     0m13.29.140s
// user    1m3.061s51s
// 
// htop -u smtorbert
// 
#include <stdio.h>
// 
#include "mpi.h"
// 
#include <stdlib.h>
//
#include <time.h>
#include <sys/time.h>
//
#define r 120
#define c 160
#define trials 300
double myrand()
{
   return ( rand() % 100 ) / 100.0 ;
}
typedef struct Node
{
   int row;
   int col;
   int level;
   //
   struct Node* next ;
} TreeNode ;


int burn(char forest[r][c]){
   //put all the node into the left-column into 
   //TreeNode* list[1000000];

   TreeNode* beg = NULL;
   TreeNode* curr = NULL;
   int step = 0;
   //left column
   for(int i = 0; i < r; i++){
      if(forest[i][0] == '+'){
         //add to the list
         //printf("Line 41\n");
         TreeNode* t = (TreeNode*)malloc( sizeof(TreeNode) );
         t->row = i;
         t->col = 0;
         t->level = 0;
         

         forest[i][0]= ' ';

         if(step==0){
            beg = t;
            curr = beg;
         }
         else{
            curr->next = t;
            curr = curr -> next;
         }
         step++;
      }
   }
   //printf("Line 58\n");
   //BFS
   int numSteps = 0;
   while(beg!=curr){
      //printf("%d %d\n", beg -> row, beg ->col);
      int currR = beg->row;
      int currC = beg->col;
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
            t->level = (beg->level)+1;

            //printf("%d %d\n", t-> row, t->col);
            numSteps = t->level;
            curr->next = t;
            curr = curr->next;

         }  
      }
      beg = beg->next;
      
      
   }
   return numSteps;
}
int main( int argc , char* argv[] )
{
   //
   // MPI variables
   //
   int        rank    ;
   int        size    ;
   MPI_Status status  ;
   int        tag = 0 ;
   //
   // other variables
   //for the problem
   //
   int        k , j  ;
   double     prob , nbt ;
   //
   // boilerplate
   //
   MPI_Init(      &argc          , &argv ) ;
   MPI_Comm_size( MPI_COMM_WORLD , &size ) ; // same
   MPI_Comm_rank( MPI_COMM_WORLD , &rank ) ; // different

   int rseed = time(NULL);
   srand(rseed);
   //
   // manager has rank = 0
   //
   if( rank == 0 )
   {
      printf( "\n" ) ;
      //for(prob = 0.5; prob <= .61; prob+=.1){
         //
         //prob = 0.60 ; // everyone gets the same probability
         //
         for( j = 1 ; j < size ; j++ )
         {
            prob = (j-1)*(1.0/(size-1))+.01;
            MPI_Send( &prob , 1 , MPI_DOUBLE , j , tag, MPI_COMM_WORLD ) ;
         }
         //
         //
         for( k = 1 ; k < size ; k++ )
         {
            double arrProb[100/(size-1)];
            MPI_Recv( &arrProb[0] , 100/(size-1) , MPI_DOUBLE , MPI_ANY_SOURCE , tag, MPI_COMM_WORLD , &status ) ;
            //
            j = status.MPI_SOURCE ;
            //
            double lProb = (j-1)*(1.0/(size-1))+.01;
            for(int i = 0; i < 100/(size-1); i++){
               printf( "%d %d %20.16f %20.16f\n" , j , size , lProb, arrProb[i] ) ;
               lProb+=.01;
            }
         }
      //}
      //
      printf( "\n" );
   }
   //
   // workers have rank > 0
   //
   else
   {
      MPI_Recv( &prob , 1 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD , &status ) ;
      //int trials = 1000;
      double arrProb[100/(size-1)];

      for(int i = 0; i < 100/(size-1); i++){
         double probTot = 0.0;
         double lProb = prob + i*.01;

         for(k = 1; k < trials; k++){
            //initialize forest
            char forest[r][c];
            for(int i = 0; i < r; i++){
               for(int j = 0; j < c; j++){
                  if(myrand() < lProb)
                     forest[i][j] = '+';
                  else
                     forest[i][j] = ' ';   
               }
            }
            probTot += burn(forest)*1.0/c;
         }
         arrProb[i] = probTot/(trials);
         //
         //printf("Workers: %f %f\n", prob, nbt);
         
     }
     MPI_Send( &arrProb[0] , 100/(size-1) , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD ) ;
   }
   //
   // boilerplate
   //
   MPI_Finalize() ;
   //
   return 0;
}
// 
// end of file
// 