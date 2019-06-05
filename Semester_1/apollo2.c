#include <math.h> 
#include <stdio.h>
#include <stdbool.h>

#define massMoon 7.34767309E22
#define massEarth 5.972E24
#define radius 6.371E6
#define G 6.67408E-11
#define radiusMoon 1737E3

int main(){
	//int n = 27*24*60*60;

	int n = 9E5;
	double x; double xPrev; double y; double yPrev;
	double vx; double vxPrev; double vy; double vyPrev;

	double xA; double xAPrev; double yA; double yAPrev;
	double vxA; double vxAPrev; double vyA; double vyAPrev;

	double dt = 1.0;
	//keep track of distance from Earth every 100s
	//moon

	//Apollo
	//double angleV = 10;
	
	//curve inward: velA = 1531.62;

	//little dimp
	/*
		double angleV = 10;
		double angleR = 77.05;
		double distA = xPrev/2;
		xAPrev = distA * cos(angleR*M_PI/180);
		yAPrev = distA * sin(angleR*M_PI/180);
		double velA = 2031.62;
		vxAPrev = velA * cos(angleV*M_PI/180);
		vyAPrev = velA * sin(angleV*M_PI/180);
	*/
	double rMPrev = -1.0;
	
	//start with velA=1700 and angle=23.0
	double angle = 23.0;
	double aIncr = 0.1;
	//bool fail = false;
	int count = 0;
	for(double velA=1700; velA<1800; velA+=100){
		
		bool fail = false;
		while(count < 1200){
			//moon initialization
			double mDist = 384400E3;
			xPrev = radius + mDist;
			yPrev = 0.0;
			vxPrev = 0.0; 
			vyPrev = sqrt(G*massEarth/(xPrev));
			//apollo initialization
			double distA = radius+ mDist/2;
			xAPrev = distA * cos(angle*M_PI/180);
			yAPrev = distA * sin(angle*M_PI/180);
			
			vxAPrev = velA * cos(angle*M_PI/180);
			vyAPrev = velA * sin(angle*M_PI/180);

			//determine whether free return, 1 rE<r; 2 rE>r; 3 rE<r no crash to Earth
			int c = 1;

			for(int j = 1; j <n; j++){
				//position of the moon
				x = xPrev + vxPrev * dt;
				y = yPrev + vyPrev * dt;
				double r = sqrt(pow(xPrev, 2)+pow(yPrev, 2));

				//position of Apollo
				xA = xAPrev + vxAPrev * dt;
				yA = yAPrev + vyAPrev * dt;
				double rE = sqrt(pow(xAPrev, 2)+pow(yAPrev, 2));
				double rM = sqrt(pow(xPrev-xAPrev, 2)+pow(yPrev-yAPrev, 2));
				
				//acceleration of the moon
				double a = -1 * G * massEarth/pow(r, 2);
				double ax = a * x/r; //acceleration for Apollo now involves ac+am
				double ay = a * y/r; 

				//acceleration of Apollo to Earth
				//acceleration of Apollo to moon
				double aE = -1 * G * massEarth/pow(rE, 2);
				double aM = -1 * G * massMoon/pow(rM, 2);
				double aAx = aE * xA/rE + aM * (xA-x)/rM;
				double aAy = aE * yA/rE + aM * (yA-y)/rM;

				//velocity of moon
				vx = vxPrev + ax * dt;
				vy = vyPrev + ay * dt;
				double v = sqrt(pow(vx, 2)+pow(vy, 2)); 
				
				//velocity of Apollo
				vxA = vxAPrev + aAx * dt;
				vyA = vyAPrev + aAy * dt;
				double vA = sqrt(pow(vxA, 2)+ pow(vyA, 2));
				//free return to Earth w/o crashing
				if(rE > r && c==1)
					c++;
				if(rE<r && c==2){
					c++;
					printf("Success %f %f\n", angle, velA);
					if(fail){
						aIncr=aIncr*10.0;
					}
					angle+=aIncr;
					fail = false;
					break;
				}
				if(j==n-1){
					//printf("%f %f %f %f\n", angle, velA, rM, rE);
					if(!fail){
						aIncr=aIncr/10.0*-1.0;
						fail = true;
					}
					angle+=aIncr;
				}
				//printf("%25.5f%25.5f%25.5f%25.5f%25.5f%25.5f%25.5f\n", j* dt, x, y, xA, yA, vA, rM);
				
				//replace previous values with current
				xPrev = x; yPrev = y; xAPrev = xA; yAPrev = yA;
				vxPrev = vx; vyPrev = vy; vxAPrev = vxA; vyAPrev = vyA;
				
			}
			count++;
		}
	}
	return 0;
}
