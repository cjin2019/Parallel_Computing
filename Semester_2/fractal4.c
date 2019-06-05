// 
// to compile type... make 
// uses file named... Makefile
// 
// a.out: fireGL.c
// 	gcc fireGL.c -lGL -lGLU -lglut
// 
// tab character '\t' before gcc
// 

//pull points from the unit circle, boundary of the mandelbrot set 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
// 
#include <GL/glut.h>
// 
#define M     800//refers to the column
#define N     600//refers to the row
#define TREE    1 
#define FIRE    2 
#define SPARK   3 
#define BURNT   8 
#define EMPTY   0 
// 

//generate points in a sequence
int pause = 0;
int gStep = 0;
double grMax = 2.0;
double grMin = -2.0;
double giMax = 1.5;
double giMin = -1.5; 
int gSteps = 100; 
// 
typedef struct{
  double  r;
  double g ;
  double b; 
} color;

double pixel[N][M];
/*color setColor;
setColor.r = 0;
setColor.g = 0;
setColor.b = 0;

color gradColor; 
gradColor.r = 1.0;
gradColor.g = 0.0;
gradColor.b = 1.0; */
// 
void init( )
{
   int x , y ;
   //
   for( x = 0 ; x < M ; x++ )
   {
      for( y = 0 ; y < N ; y++ )
      { 
	     pixel[y][x]= -1.0; 
      }
   }
}

// 
void makeJulia(double cRe, double cIm,
        double reMin, double reMax, double imMin, double imMax, int limit)//re refers to the max real number and img refers to the max imaginary number
{
  // convert the pixel screen to re-im plane
  int step; 
  //printf("Made fractal\n");
  for(int x = 0; x <M; x++){
    for(int y = 0; y < N; y++){
      double reDist = reMax - reMin;
      double imDist = imMax - imMin; 

      double zRePrev = x * 1.0 / M * (reDist) + reMin;
      double zImPrev = y * 1.0 / N * (imDist) +imMin;

      double zRe = 0.0;
      double zIm = 0.0;

      for (step = 0; step < limit; step++){
        zRe = zRePrev*zRePrev - zImPrev*zImPrev + cRe;
        zIm = 2 * zRePrev * zImPrev + cIm;

        double zMag = sqrt(zRe*zRe + zIm * zIm);
        //printf("%f\n", zMag);
        if( zMag > 2){
           break;
        }
        
        zRePrev = zRe;
        zImPrev = zIm;
      }
      if (step==limit){
        pixel[y][x] =1000.0; 
      }
      else{
        double scale = 1.0 - pow(1.0-step*1.0/limit, 3.0);
       // printf("scale\n");
        //printf("%f\n", scale);
        pixel[y][x] = scale; 
      }
    }
  }
}
void makeMandelbrot(double reMin, double reMax, double imMin, double imMax, int limit )//re refers to the max real number and img refers to the max imaginary number
{
  // convert the pixel screen to re-im plane
  int step; 
  printf("Made fractal\n");
  for(int x = 0; x <M; x++){
    #pragma omp parallel for private(y, t)
    for(int y = 0; y < N; y++){
      t = y; 

      double reDist = reMax - reMin;
      double imDist = imMax - imMin; 

      double real = x * 1.0 / M * (reDist) + reMin;
      double img = y * 1.0 / N * (imDist) +imMin;

      double zRePrev = 0.0;
      double zImPrev = 0.0;

      double zRe = 0.0;
      double zIm = 0.0;

      for (step = 0; step < limit; step++){
        zRe = zRePrev*zRePrev - zImPrev*zImPrev + real;
        zIm = 2 * zRePrev * zImPrev + img;

        double zMag = sqrt(zRe*zRe + zIm * zIm);
        //printf("%f\n", zMag);
        if( zMag > 2){
           break;
        }
        
        zRePrev = zRe;
        zImPrev = zIm;
      }
      if (step==limit){
        pixel[y][x] =1000.0; 
      }
      else{
        double scale = 1.0 - pow(1.0-step*1.0/limit, 3.0);
       // printf("scale\n");
        //printf("%f\n", scale);
        pixel[y][x] = scale; 
      }
    }
  }
}
       
      
 
void idlefunc()
{
   int x , y    ;
   int fire = 1 ;
   //
   if( pause ) return ;

   if( fire )
   {
      ++gStep ;
      //
      glutPostRedisplay(); // calls displayfunc
   }
}

void displayfunc()
{
   int    x  , y  ;
   //
   glClear(GL_COLOR_BUFFER_BIT); // white
   //
   for( x = 0 ; x < M ; x++ )
   {
      for( y = 0 ; y < N ; y++ )
      { 
         if( pixel[y][x] >=1000.0 ) 
         {
            glColor3f( 0.0 , 0.0, 0.0) ; // white
            //printf("Hello!\n");
         }
         else  // t[y][x] == EMPTY
         {
            
            //printf("Bye %f\n", pixel[y][x]);
            glColor3f( 3.0*pixel[y][x] , 7.0*pixel[y][x] , 5.0*pixel[y][x] ) ; // brown
         }
         //
         glBegin(GL_POINTS);
         glVertex2f(x,y);
         glEnd();
      }
     // glFlush();
   }
   //
   //
   glutSwapBuffers(); // single buffering... call glFlush();
}
void reshapefunc(int wscr,int hscr)
{
   glViewport(0,0,(GLsizei)M,(GLsizei)N);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0,1.0*M,0.0,1.0*N); 
   glMatrixMode(GL_MODELVIEW);
}
void mousefunc(int button,int state,int xscr,int yscr)
{
   double real = xscr * 1.0 / M * (reDist) + grMin;
   double img = (N - yscr) * 1.0 / N * (imDist) + giMin;
   if(button==GLUT_LEFT_BUTTON)
   {
      if(state==GLUT_DOWN)
      {
        // init( prob ) ;
         //translate the point to re-im plane
         double reDist = grMax - grMin;
         double imDist = giMax - giMin; 

         

         grMax = real + reDist/4.0;
         grMin = real - reDist/4.0; 

         giMax = img + imDist/4.0;
         giMin = img - imDist/4.0; 

         printf("%f %f %f %f\n", grMin, grMax, giMin, giMax);
         makeMandelbrot(grMin, grMax, giMin, giMax, gSteps);

         glutPostRedisplay(); 
      }
   }
   else if(button==GLUT_RIGHT_BUTTON)
   {
      if(state==GLUT_DOWN)
      {
         makeJulia(real, img, grMin, grMax, giMin, giMax, gSteps);
      }
   }
}
/*void keyfunc(unsigned char key,int xscr,int yscr)
{
   if( key == ' ' )
   {
      pause = !pause ;
   }
   else if( key == 'q' )
   {
      exit( 0 ) ;
   }
}*/
int main(int argc,char* argv[])
{  
   glutInit(&argc,argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize(N, N);
   glutInitWindowPosition(100,50);
   glutCreateWindow("");
   glClearColor(1.0,1.0,1.0,0.0);
   glShadeModel(GL_SMOOTH);
   //
   srand( 12345 ) ; // arbitrary random seed
   //
   init(  ) ;
   makeMandelbrot(grMin, grMax, giMin, giMax, gSteps) ;
   //
   glutIdleFunc(idlefunc);
   glutDisplayFunc(displayfunc);
   glutReshapeFunc(reshapefunc);
   glutMouseFunc(mousefunc);
   //glutKeyboardFunc(keyfunc);
   //
   glutMainLoop();
   //
   return 0;
}
// 
// end of file
// 
