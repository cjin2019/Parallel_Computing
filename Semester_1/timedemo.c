//
// Torbert, 15 October 2013
//
#include <stdio.h>
#include <stdlib.h>
//
#include <sys/time.h>
//
double gettime()
{
	double t ;
	//
	struct timeval* ptr = (struct timeval*)malloc( sizeof(struct timeval) ) ;
	//
	gettimeofday( ptr , NULL ) ; // second argument is time zone... NULL
	//
	t = ptr->tv_sec * 1000000.0 + ptr->tv_usec ;
	//
	free( ptr ) ;
	//
	return t / 1000000.0 ;
}
//
int main( int argc , char* argv[] )
{
	int j ;
	//
	double tic , toc ;
	//
	tic = gettime() ;
	//
	for( j = 0 ; j < 123456789 ; j++ ) ;
	//
	toc = gettime() ;
	//
	printf("Time: %0.16f seconds\n" , toc - tic ) ;
	//
	return 0;
}
//
// end of file
//
// time ./a.out
// Time: 0.3139879703521729 seconds
// 
// real    0m0.315s
// user    0m0.313s
// sys     0m0.001s
