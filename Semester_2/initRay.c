
#include <stdio.h>
#include <math.h>
#define M 640
#define N 480

typedef struct{
   int r;
   int g;
   int b;
} color;
typedef struct
{
   double x ;
   double y ;
   double z ;
   //
} triple ;

typedef struct{
   triple c;
   double r;
   color h; 
} sphere;
//
triple e = { 0.50 , 0.50 , -1.00 } ; // the eye
triple g = { 0.00 , 1.25 , -0.50 } ; // the light
//
double dotp( triple t , triple u )
{
   return t.x * u.x + t.y * u.y + t.z * u.z ;
}
//
void diff( triple* t , triple u , triple v ) // t = u - v
{
   t->x = u.x - v.x ;
   t->y = u.y - v.y ;
   t->z = u.z - v.z ;
}
//
int main(int argc, char const *argv[])
{
   /* code */
   sphere a[4];
   a[0].c.x =      0.50 ;
   a[0].c.y = -20000.00 ; // the floor
   a[0].c.z =      0.50 ;
   a[0].r   =  20000.25 ;
   a[0].h.r =    205    ; // color is Peru
   a[0].h.g =    133    ;
   a[0].h.b =     63    ;
   //
   a[1].c.x =      0.50 ;
   a[1].c.y =      0.50 ;
   a[1].c.z =      0.50 ;
   a[1].r   =      0.25 ;
   a[1].h.r =      0    ; // color is Blue
   a[1].h.g =      0    ;
   a[1].h.b =    255    ;
   //
   a[2].c.x =      1.00 ;
   a[2].c.y =      0.50 ;
   a[2].c.z =      1.00 ;
   a[2].r   =      0.25 ;
   a[2].h.r =      0    ; // color is Green
   a[2].h.g =    255    ;
   a[2].h.b =      0    ;
   //
   a[3].c.x =      0.00 ;
   a[3].c.y =      0.75 ;
   a[3].c.z =      1.25 ;
   a[3].r   =      0.50 ;
   a[3].h.r =    255    ; // color is Red
   a[3].h.g =      0    ;
   a[3].h.b =      0    ;

   //equation
   int rgb[N][M][3] ;

   int y , x ;
   for( y = 0 ; y < N ; y++ )
   {
      for( x = 0 ; x < M ; x++)
      {
         triple r = {x/640.0*4.0/3.0-e.x, y/480.0-e.y, 0.0-e.z};
         double normalization = sqrt(pow(r.x, 2.0)+pow(r.y, 2.0)+pow(r.z, 2.0)); 
         r.x/=normalization;
         r.y/=normalization;
         r.z/=normalization;
         //for each sphere
         double minT = 1E8;
         int sNum = -1;
         for(int i = 0; i < 4; i++){
            triple d = {e.x - a[i].c.x, e.y - a[i].c.y, e.z - a[i].c.z};
            double A = 1.0;
            double B = 2 * (r.x*d.x+r.y*d.y+r.z*d.z);
            double C = (pow(d.x, 2.0) + pow(d.y, 2.0) + pow(d.z, 2.0) - pow(a[i].r, 2));
            //printf("%f %f %f \n", A, B, C);
            double discrm = pow(B, 2.0)-4*A*C;
            //hit the spheres or the background
            if(discrm>=0){
               double T1 = -1*B+sqrt(discrm)/(2*A);
               double T2 = -1*B-sqrt(discrm)/(2*A);
               double minCT = fmin(T1, T2);
               if(minCT > 0 && minCT < minT){
                  minT = minCT;
                  sNum = i;
               }
            }

         }

         rgb[y][x][0] = a[sNum].h.r   ; // red
         rgb[y][x][1] = a[sNum].h.g; // green
         rgb[y][x][2] = a[sNum].h.b; // blue
      }
   }
   FILE* fout ;
   fout = fopen( "flat.ppm" , "w" ) ;
   fprintf( fout , "P3\n" ) ;
   fprintf( fout , "%d %d\n" , M , N ) ;
   fprintf( fout , "255\n" ) ;

   for( y = 0 ; y < N ; y++ )
   {
      for( x = 0 ; x < M ; x++)
      {
         fprintf( fout , "%d %d %d\n" ,
          rgb[N-1-y][x][0] , rgb[N-1-y][x][1] , rgb[N-1-y][x][2] ) ;
      }
   }
   return 0;
}   
//
// end of file
//