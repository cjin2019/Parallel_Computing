#include <math.h> 
#include <stdio.h>

#define massMoon 7.34767309E22
#define massEarth 5.972E24
#define radius 6.371E6
#define G 6.67408E-11

int main(){
	double dt = 1.0;
	int n = 3*24*60*60;
		//use trig for initialization for x and you slightly closer to the moon

	//moon: radius + 3844000
	//vx = sqrt(G+mass/(y[0]+9E5))

	//moon
	double pY = 0.0;
	//assume moon along x-axis and moving 
	//find angle for intialization
	//thrust--farthest point (acceleration opposite direction of velocity (resistance))
	//--appraching point change in velocity that bring y
	double pX = (radius+384400E3);
	double pVX = 0.0; //esca[e velocity sqrt(2*G*mass/radius)
	double pVY = -1*sqrt(G*massEarth/(pX));

	double cX = 0.0;
	double cY = 0.0;
	double cVX = 0.0;
	double cVY = 0.0;

	//Apollo
	double angle = 25.8;
	double distA = pX/2;
	double pXA= distA * cos(angle*M_PI/180);
	double pYA = distA * sin(angle*M_PI/180);
	double velA = 5000;
	double pVXA = velA * cos(angle*M_PI/180);
	double pVYA = velA * sin(angle*M_PI/180);
	//angle = 24.8 for Apollo orbiting around the moon a the moon orbit
	//angle = 23 for Apollo deep space 
	//angle = 28 for Apollo free return
 	double cXA = 0.0;
 	double cYA = 0.0;
 	double cVXA = 0.0;
 	double cVYA = 0.0;

	//printf("%25.15s%25.15s%25.15s%25.15s%25.15s%25.15s%25.15s\n", "Time", "x", "y", "r", "vx", "vy", "|v|");
	for(int j = 1; j <n; j++){
		cX = pX + pVX * dt;
  		cY = pY + pVY * dt;
		double r = sqrt(pow(pX, 2)+pow(pY, 2));

		cXA = pXA + cVXA * dt;
		cYA = pYA + cVYA * dt;
		double rE = sqrt(pow(pXA, 2)+pow(pYA, 2));
		double rM = sqrt(pow(pX-pXA, 2)+pow(pY-pYA, 2));

  		double a = -1 * G * massEarth/pow(r, 2);
  		double ax = a * cX/r; //acceleration for Apollo now involves ac+am
  		double ay = a * cY/r; 


  		double aE = -1 * G * massEarth/pow(rE, 2);
  		double aM = -1 * G * massMoon/pow(rM, 2);
  		double aAx = aE * cXA/rE + aM * (cXA-cX)/rM;
  		double aAy = aE * cYA/rE + aM * (cYA-cY)/rM;

  		
  		cVX = pVX + ax * dt;
  		cVY = pVY + ay * dt;
  		double v = sqrt(pow(cVX, 2)+pow(cVY, 2)); 
  		
  		cVXA = pVXA + aAx * dt;
  		cVYA = pVYA + aAy * dt;
  		double vA = sqrt(pow(cVXA, 2)+ pow(cVYA, 2));
  		//printf("%s\n", );
  		printf("%25.5f%25.5f%25.5f%25.5f%25.5f%25.5f%25.5f\n", j* dt, cX, cY, cXA, cYA, vA, rM);

  		//replace prev var with current
  		pX = cX;
  		pY = cY;
  		pVX = cVX;
  		pVY = cVY;

  		pXA = cXA;
  		pYA = cYA;
  		pVXA = cVXA;
  		pVYA = cVYA;
  		//printf("%25.15f%25.15f%25.15f%25.15f%25.15f\n", ax, ay, vy[j], x[j], y[j]);
  	}
	return 0;
}