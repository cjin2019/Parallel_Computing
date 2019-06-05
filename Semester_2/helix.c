
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
   FILE* fin ;
   fin = fopen( "helix.txt" , "r" ) ;
   int numSphere;

   fscanf(fin, "%d", &numSphere);
   sphere a[1+numSphere];
   a[0].c.x =      0.50 ;
   a[0].c.y = -20000.00 ; // the floor
   a[0].c.z =      0.50 ;
   a[0].r   =  20000.25 ;
   a[0].h.r =    205    ; // color is Peru
   a[0].h.g =    133    ;
   a[0].h.b =     63    ;
   //
   for(int i = 1; i <= numSphere; i++){
      fscanf(fin, "%lf %lf %lf %lf", &a[i].c.x, &a[i].c.y, &a[i].c.z, &a[i].r);
      a[i].h.r =    0    ; // color is Peru
      a[i].h.g =    0    ;
      a[i].h.b =  255    ;
      //printf("%lf %lf %lf %lf\n", a[i].c.x, a[i].c.y, a[i].c.z, a[i].r);
   }

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
         for(int i = 0; i < numSphere+1; i++){
            triple d = {e.x - a[i].c.x, e.y - a[i].c.y, e.z - a[i].c.z};
            double A = 1.0;
            double B = 2 * (r.x*d.x+r.y*d.y+r.z*d.z);
            double C = (pow(d.x, 2.0) + pow(d.y, 2.0) + pow(d.z, 2.0) - pow(a[i].r, 2));
            //printf("%f %f %f \n", A, B, C);
            double discrm = pow(B, 2.0)-4*A*C;
            //hit the spheres or the background
            if(discrm>=0){
               double T1 = (-1*B+sqrt(discrm))/(2*A);
               double T2 = (-1*B-sqrt(discrm))/(2*A);
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

         triple ptSphere={e.x+minT*r.x, e.y+minT*r.y, e.z+minT*r.z};
         //normal
         triple n = {(ptSphere.x-a[sNum].c.x)/a[sNum].r, 
                     (ptSphere.y-a[sNum].c.y)/a[sNum].r, 
                     (ptSphere.z-a[sNum].c.z)/a[sNum].r};
         ptSphere.x += (n.x*.001);
         ptSphere.y += (n.y*.001);
         ptSphere.z += (n.z*.001);

         triple l = {g.x-ptSphere.x, g.y-ptSphere.y, g.z-ptSphere.z};
         normalization = sqrt(pow(l.x, 2)+pow(l.y, 2)+pow(l.z, 2));

         l.x/=normalization;
         l.y/=normalization;
         l.z/=normalization;

         

         rgb[y][x][0] = rgb[y][x][0]/2+rgb[y][x][0]/2*dotp(n, l);
         rgb[y][x][1] = rgb[y][x][1]/2+rgb[y][x][1]/2*dotp(n, l);
         rgb[y][x][2] = rgb[y][x][2]/2+rgb[y][x][2]/2*dotp(n, l);
         for(int i = 0; i < numSphere+1; i++){
            
            triple d = {ptSphere.x - a[i].c.x, ptSphere.y - a[i].c.y, ptSphere.z - a[i].c.z};
            double A = 1.0;
            double B = 2 * (l.x*d.x+l.y*d.y+l.z*d.z);
            double C = (pow(d.x, 2.0) + pow(d.y, 2.0) + pow(d.z, 2.0) - pow(a[i].r, 2));

            double discrm = pow(B, 2.0)-4*A*C;
            //hit the spheres or the background
            if(discrm>0){

               double T1 = (-1*B+sqrt(discrm))/(2*A);
               double T2 = (-1*B-sqrt(discrm))/(2*A);
               double minCT = fmin(T1, T2);
               if(minCT > 0){
                  rgb[y][x][0] -= rgb[y][x][0]/2*dotp(n, l) ;
                  rgb[y][x][1] -= rgb[y][x][1]/2*dotp(n, l) ;
                  rgb[y][x][2] -= rgb[y][x][2]/2*dotp(n, l) ;
                  

                  break; 
               }
               
            }
         }
         
         
      }
   }
   FILE* fout ;
   fout = fopen( "shadow.ppm" , "w" ) ;
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