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
#include <math.h> 
#include <stdio.h>
#include <stdbool.h>

#define massMoon 7.34767309E22
#define massEarth 5.972E24
#define radius 6.371E6
#define G 6.67408E-11
#define radiusMoon 1737E3
double orbit(double angle, double velA){
   double fail;
   //declaring variables to be used 
   int n = 9E5;
   double x; double xPrev; double y; double yPrev;
   double vx; double vxPrev; double vy; double vyPrev;

   double xA; double xAPrev; double yA; double yAPrev;
   double vxA; double vxAPrev; double vyA; double vyAPrev;

   double dt = 1.0;
   //moon initialization
   double mDist = 384400E3;
   xPrev = radius + mDist;
   yPrev = 0.0;
   vxPrev = 0.0; 
   vyPrev = sqrt(G*massEarth/(xPrev));
   //apollo initialization
   double distA = radius+ mDist/2;
   xAPrev = distA * cos(angle*M_PI/180);
   yAPrev = distA * sin(angle*M_PI/180);
   
   vxAPrev = velA * cos(angle*M_PI/180);
   vyAPrev = velA * sin(angle*M_PI/180);

   //determine whether free return, 1 rE<r; 2 rE>r; 3 rE<r no crash to Earth
   int c = 1;
   
   for(int j = 1; j <n; j++){
      //position of the moon
      x = xPrev + vxPrev * dt;
      y = yPrev + vyPrev * dt;
      double r = sqrt(pow(xPrev, 2)+pow(yPrev, 2));

      //position of Apollo
      xA = xAPrev + vxAPrev * dt;
      yA = yAPrev + vyAPrev * dt;
      double rE = sqrt(pow(xAPrev, 2)+pow(yAPrev, 2));
      double rM = sqrt(pow(xPrev-xAPrev, 2)+pow(yPrev-yAPrev, 2));
      
      //acceleration of the moon
      double a = -1 * G * massEarth/pow(r, 2);
      double ax = a * x/r; //acceleration for Apollo now involves ac+am
      double ay = a * y/r; 

      //acceleration of Apollo to Earth
      //acceleration of Apollo to moon
      double aE = -1 * G * massEarth/pow(rE, 2);
      double aM = -1 * G * massMoon/pow(rM, 2);
      double aAx = aE * xA/rE + aM * (xA-x)/rM;
      double aAy = aE * yA/rE + aM * (yA-y)/rM;

      //velocity of moon
      vx = vxPrev + ax * dt;
      vy = vyPrev + ay * dt;
      double v = sqrt(pow(vx, 2)+pow(vy, 2)); 
      
      //velocity of Apollo
      vxA = vxAPrev + aAx * dt;
      vyA = vyAPrev + aAy * dt;
      double vA = sqrt(pow(vxA, 2)+ pow(vyA, 2));
      //free return to Earth w/o crashing
      if(rE > r && c==1)
         c++;
      if(rE<r && c==2){
         c++;
         fail=1.0;
         break;
      }
      if(j==n-1){
         fail=-1.0;
      }
      //printf("%25.5f%25.5f%25.5f%25.5f%25.5f%25.5f%25.5f\n", j* dt, x, y, xA, yA, vA, rM);
      
      //replace previous values with current
      xPrev = x; yPrev = y; xAPrev = xA; yAPrev = yA;
      vxPrev = vx; vyPrev = vy; vxAPrev = vxA; vyAPrev = vyA;
            
   }
   return fail;
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

   double angle = 5.0;
   double angleStart = angle;
   double angleIncr = 1.0;
   double angleEnd = 85.0;

   double v = 1400;
   double vIncr = -10;
   double vEnd = 1300;

   
   int numComplete = 0;
   //
   // manager has rank = 0
   //
   if( rank == 0 )
   {
      printf( "\n" ) ;

         //initially send one angle and same velocity to all the workers

         for(j=1; j < size; j++){
            double aV[2];
            aV[0] = angle;
            aV[1] = v;
            MPI_Send( &aV[0] , 2 , MPI_DOUBLE , j, tag, MPI_COMM_WORLD ) ;
            angle+=angleIncr;
         }
         while(true){
      
            double output[3];
            MPI_Recv( &output[0] , 3 , MPI_DOUBLE , MPI_ANY_SOURCE , tag, MPI_COMM_WORLD , &status ) ;

            j = status.MPI_SOURCE ;
            if(output[2]>0.0)
               printf( "%f %f %f\n" , output[0], output[1], output[2]) ;
            double aV[2];
            if(angle < angleEnd){
               aV[0] = angle;
               aV[1] = v;
               MPI_Send( &aV[0] , 2 , MPI_DOUBLE , j, tag, MPI_COMM_WORLD ) ;
            }
            else if(v > vEnd){
               angle = angleStart;
               v+=vIncr;
               aV[0] = angle;
               aV[1] = v;
               MPI_Send( &aV[0] , 2 , MPI_DOUBLE , j, tag, MPI_COMM_WORLD ) ;
            }
            else{
               numComplete++;
            }
            if(numComplete==size-1){
               for(j = 1; j < size; j++){
                  aV[0] = -1.0;
                  aV[1] = -1.0;
                  MPI_Send( &aV[0] , 2 , MPI_DOUBLE , j, tag, MPI_COMM_WORLD ) ;
                  
               }
               break;
            }
            angle+=angleIncr;

         }
         
      printf( "\n" );
   }
   //
   // workers have rank > 0
   //
   else
   {
      while(true){

         double aV[2];
         MPI_Recv( &aV[0] , 2 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD , &status ) ;
         if(aV[0]<0){
            //printf("Done \n");
            break;
         }
         double fail = orbit(aV[0], aV[1]);
         double output[3];
         output[0]=aV[0];
         output[1]=aV[1];
         output[2]=fail;
         
         MPI_Send( &output[0] , 3 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD ) ;
      }
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