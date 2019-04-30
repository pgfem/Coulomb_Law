#include <windows.h>
#include <GL/gl.h>
#include <math.h>
#include <stdio.h>
#include "coulomb.h"


charge::charge(float x,float y,int i, int t){
	
	pointx = x;
	pointy = y;
	radius=5.0;
	id =i;
	type=t;
				if (type==0){
					red = 1.0;
					green = 0.0;
				}
				else {
					red = 0.0;
					green = 1.0;
				}

}

void charge::calcparams(){
	magnitude = sqrt(pow(pointx,2) + pow(pointy,2));
	unitx = pointx/magnitude;
	unity = pointy/magnitude;
//	printf("%f %f %f",unitx,unity,pow(unitx,2)+pow(unity,2));
	theta = atan(pointy / pointx);
	printf("%f",theta*180/3.14);

}

void charge::display(){
	glColor3f(red,green,0);
	glBegin(GL_TRIANGLE_FAN);
	for (int x=0;x<360;x++){
	glVertex3f(pointx+(sin(x*DEG2RAD)*radius),pointy+(cos(x*DEG2RAD)*radius),0);
	}
	glEnd();
}
/*
int main(){

	charge obj(3,4);
	obj.calcparams();

return 0;
}
*/
