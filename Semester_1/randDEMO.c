//
// Torbert, 24 Sept 2014
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//
double myrand()
{
   return ( rand() % 100 ) / 100.0 ;
}
//
int main( int argc , char* argv[] )
{
   int rseed;
   //
   rseed = time( NULL ) ;
   //
   srand( rseed ) ;
   //
   system( "clear" ) ;
   //
   printf( "%d\n" , rseed ) ;
   //
   printf( "%20.16f\n" , myrand() ) ;
   printf( "%20.16f\n" , myrand() ) ;
   printf( "%20.16f\n" , myrand() ) ;
   printf( "%20.16f\n" , myrand() ) ;
   printf( "%20.16f\n" , myrand() ) ;
   //
   return 0;
}
//
// end of file
//