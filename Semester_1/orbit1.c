#include <stdio.h>
#include <stdlib.h>
#include <math.h> 

#define mass 5.972E24
#define radius 6.371E6
#define G 6.67408E-11
int main(){
	int n = 15*60*60;
	double x[n];
	double y[n];
	double vx[n];
	double vy[n];
	double dt = 1.0;
	//use trig for initialization for x and y
	//assume moon along x-axis and moving up
	x[0] = 0.0;
	y[0] = radius+384400E3;
	vx[0] = sqrt(G*mass/x[0]); //esca[e velocity sqrt(2*G*mass/radius)
	vy[0] = 0.0;

	printf("%25.5s%25.5s%25.5s%25.5s%25.5s%25.5s%25.5s\n", "Time", "x", "y", "r", "vx", "vy", "|v|");
	for(int j = 1; j <n; j++){
		x[j] = x[j-1] + vx[j-1] * dt;
  		y[j] = y[j-1] + vy[j-1] * dt;
		double r = sqrt(pow(x[j-1], 2)+pow(y[j-1], 2));

  		double a = -1 * G * mass/pow(r, 2);
  		double ax = a * x[j]/r; //acceleration for Apollo now involves ac+am
  		double ay = a * y[j]/r; 
  		
  		vx[j] = vx[j-1] + ax * dt;
  		vy[j] = vy[j-1] + ay * dt;
  		double v = sqrt(pow(vx[j], 2)+pow(vy[j], 2)); 
  		
  		//printf("%s\n", );
  		printf("%25.5f%25.5f%25.5f%25.5f%25.5f%25.5f%25.5f\n", j* dt, x[j], y[j], r, vx[j], vy[j], v);
  		//printf("%25.15f%25.15f%25.15f%25.15f%25.15f\n", ax, ay, vy[j], x[j], y[j]);
  	}
	return 0;
}
