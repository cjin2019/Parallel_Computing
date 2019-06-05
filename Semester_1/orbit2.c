#include <stdio.h>
#include <stdlib.h>
#include <math.h> 

#define mass 5.972E24
#define radius 6.371E6
#define G 6.67408E-11
int main(){
	int n = 200*60*4;//ws2ws2ws2wsw2s
	double x[n];
	double y[n];
	double vx[n];
	double vy[n];
	double dt = .25;
	//use trig for initialization for x and y
	//assume moon along x-axis and moving 
	//space crafts do it twice once for the distance from the Earth and once from the moon
	
	//ellipse 
	//y[0] = radius+1400000;
	//vx[0] = sqrt(G*mass(7E6))
	double ratio = 1.0;
	x[0] = 0.0;
	y[0] = radius+400000;
	vx[0] = sqrt(G*mass/(radius*.90));
	//vx[0] = (vx[0] +  sqrt(2*G*mass/y[0]))/2;//escape velocity sqrt(2*G*mass/radius)
	vy[0] = 0.0;

	//printf("%25.5s%25.5s%25.5s%25.5s%25.5s%25.5s%25.5s\n", "Time", "x", "y", "r", "vx", "vy", "|v|");
	for(int j = 1; j <n; j++){
		x[j] = ratio *(x[j-1] + vx[j-1] * dt);
  		y[j] = y[j-1] + vy[j-1] * dt;
		double r = sqrt(pow(x[j], 2)+pow(y[j], 2));

  		double a = -1 * G * mass/pow(r, 2);
  		double ax = a * x[j]/r; //acceleration for Apollo now involves ac+am
  		double ay = a * y[j]/r; 
  		
  		vx[j] = vx[j-1] + ax * dt;
  		vy[j] = vy[j-1] + ay * dt;
  		double v = sqrt(pow(vx[j], 2)+pow(vy[j], 2)); 
  		
  		//printf("%s\n", );
  		printf("%25.15f%25.15f%25.15f%25.15f%25.15f%25.15f%25.15f\n", j* dt, x[j], y[j], r, vx[j], vy[j], v);
  		//printf("%25.15f%25.15f%25.15f%25.15f%25.15f\n", ax, ay, vy[j], x[j], y[j]);
  	}
	return 0;
}
