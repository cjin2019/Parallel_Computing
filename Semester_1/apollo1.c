#include <math.h> 
#include <stdio.h>

#define massMoon 7.34767309E22
#define massEarth 5.972E24
#define radius 6.371E6
#define G 6.67408E-11

int main(){
	int n = 10*60*60;
	double x[n];
	double y[n];
	double vx[n];
	double vy[n];

	double xA[n];
	double yA[n];
	double vxA[n];
	double vyA[n];

	double dt = 1.0;
	//use trig for initialization for x and you slightly closer to the moon

	//moon: radius + 3844000
	//vx = sqrt(G+mass/(y[0]+9E5))

	//moon
	y[0] = 0.0;
	//assume moon along x-axis and moving 
	//find angle for intialization
	//thrust--farthest point (acceleration opposite direction of velocity (resistance))
	//--appraching point change in velocity that bring y
	x[0] = radius+384400E3;
	vx[0] = 0.0; //esca[e velocity sqrt(2*G*mass/radius)
	vy[0] = -1*sqrt(G*massEarth/(x[0]));

	//Apollo
	double angle = 28;
	double distA = -1*x[0]/2;
	xA[0] = distA * cos(angle*M_PI/180);
	yA[0] = distA * sin(angle*M_PI/180);
	double velA = 12500;
	vxA[0] = velA * cos(angle*M_PI/180);
	vyA[0] = velA * sin(angle*M_PI/180);
	//angle = 24.8 for Apollo orbiting around the moon a the moon orbit
	//angle = 23 for Apollo deep space 
	//angle = 28 for Apollo free return
 

	//printf("%25.15s%25.15s%25.15s%25.15s%25.15s%25.15s%25.15s\n", "Time", "x", "y", "r", "vx", "vy", "|v|");
	for(int j = 1; j <n; j++){
		x[j] = x[j-1] + vx[j-1] * dt;
  		y[j] = y[j-1] + vy[j-1] * dt;
		double r = sqrt(pow(x[j-1], 2)+pow(y[j-1], 2));

		xA[j] = xA[j-1] + vxA[j-1] * dt;
		yA[j] = yA[j-1] + vyA[j-1] * dt;
		double rE = sqrt(pow(xA[j-1], 2)+pow(yA[j-1], 2));
		double rM = sqrt(pow(x[j-1]-xA[j-1], 2)+pow(y[j-1]-yA[j-1], 2));

  		double a = -1 * G * massEarth/pow(r, 2);
  		double ax = a * x[j]/r; //acceleration for Apollo now involves ac+am
  		double ay = a * y[j]/r; 

  		double aE = -1 * G * massEarth/pow(rE, 2);
  		double aM = -1 * G * massMoon/pow(rM, 2);
  		double aAx = aE * xA[j]/rE + aM * (xA[j]-x[j])/rM;
  		double aAy = aE * yA[j]/rE + aM * (yA[j]-y[j])/rM;

  		
  		vx[j] = vx[j-1] + ax * dt;
  		vy[j] = vy[j-1] + ay * dt;
  		double v = sqrt(pow(vx[j], 2)+pow(vy[j], 2)); 
  		
  		vxA[j] = vxA[j-1] + aAx * dt;
  		vyA[j] = vyA[j-1] + aAy * dt;
  		double vA = sqrt(pow(vxA[j], 2)+ pow(vyA[j], 2));
  		//printf("%s\n", );
  		printf("%25.5f%25.5f%25.5f\n", j* dt, x[j], y[j]);
  		//printf("%25.5f%25.5f%25.5f%25.5f%25.5f%25.5f%25.5f\n", j* dt, x[j], y[j], xA[j], yA[j], vA, rM);
  		//printf("%25.15f%25.15f%25.15f%25.15f%25.15f\n", ax, ay, vy[j], x[j], y[j]);
  	}
	return 0;
}