#include <math.h> 
#include <stdio.h>

#define massMoon 7.34767309E22
#define massEarth 5.972E24
#define radius 6.371E6
#define G 6.67408E-11
#define radiusMoon 1737E3

int main(){
	int n = 9E5;
	double x; double xPrev; double y; double yPrev;
	double vx; double vxPrev; double vy; double vyPrev;

	double xA; double xAPrev; double yA; double yAPrev;
	double vxA; double vxAPrev; double vyA; double vyAPrev;

	double dt = 1.0;

	//use trig for initialization for x and y
	//assume moon along x-axis and moving 
	//find angle for intialization
	//thrust--farthest point (acceleration opposite direction of velocity (resistance))
	//--appraching point change in velocity that bring you slightly closer to the moon

	//moon: radius + 3844000
	//vx = sqrt(G+mass/(y[0]+9E5))

	//moon
	xPrev = radius + 384400E3;
	yPrev = 0.0;
	vxPrev = 0.0; //esca[e velocity sqrt(2*G*mass/radius)
	vyPrev = sqrt(G*massEarth/(xPrev));
	

	//Apollo
	
	//for(double angle = 30.0;angle>14.5; angle-=.05){
		double angle = 74.0;
		double distA = radius + 384400E3/2;
		xAPrev = distA * cos(angle*M_PI/180);
		yAPrev = distA * sin(angle*M_PI/180);
		//for(double velA = 500; velA < 10000; velA+=500){
		double velA = 1500;
		vxAPrev = velA * cos(angle*M_PI/180);
		vyAPrev = velA * sin(angle*M_PI/180);
		
		//double  minTime = 0.0;
		//double minDist = -1.0;

		//printf("%25.15s%25.15s%25.15s%25.15s%25.15s%25.15s%25.15s\n", "Time", "x", "y", "r", "vx", "vy", "|v|");
		
		for(int j = 1; j <n; j++){
			x = xPrev + vxPrev * dt;
  			y = yPrev + vyPrev * dt;
			double r = sqrt(pow(xPrev, 2)+pow(yPrev, 2));
	
			xA = xAPrev + vxAPrev * dt;
			yA = yAPrev + vyAPrev * dt;
			double rE = sqrt(pow(xAPrev, 2)+pow(yAPrev, 2));
			double rM = sqrt(pow(xPrev-xAPrev, 2)+pow(yPrev-yAPrev, 2));

  			double a = -1 * G * massEarth/pow(r, 2);
  			double ax = a * x/r; //acceleration for Apollo now involves ac+am
  			double ay = a * y/r; 

  			double aE = -1 * G * massEarth/pow(rE, 2);
  			double aM = -1 * G * massMoon/pow(rM, 2);
  			double aAx = aE * xA/rE + aM * (xA-x)/rM;
  			double aAy = aE * yA/rE + aM * (yA-y)/rM;

  		
  			vx = vxPrev + ax * dt;
  			vy = vyPrev + ay * dt;
  			double v = sqrt(pow(vx, 2)+pow(vy, 2)); 
  		
  			vxA = vxAPrev + aAx * dt;
  			vyA = vyAPrev + aAy * dt;
  			double vA = sqrt(pow(vxA, 2)+ pow(vyA, 2));
  			//printf("%s\n", );
  			//printf("%25.5f%25.5f%25.5f%25.5f%25.5f%25.5f%25.5f\n", j* dt, x, y, xA, yA, vA, rM);
  			xPrev = x; yPrev = y; xAPrev = xA; yAPrev = yA;
			vxPrev = vx; vyPrev = vy; vxAPrev = vxA; vyAPrev = vyA;
  			
  			printf("%25.5f%25.5f%25.5f%25.5f%25.5f%25.5f%25.5f\n", j* dt, x, y, xA, yA, vA, rM);
  			
		}
			//printf("%25.2f%25.2f\n", );
  		//}
	//}
	return 0;
}
