#define RAD2DEG 180.0/31.4
#define DEG2RAD 31.4/180.0
class charge {
	public:
	float pointx;
	float pointy;
	float theta;
	float magnitude;

	float unitx;
	float unity;
	
	float dx;
	float dy;
	float red;
	float green;
	
	float radius;
	int id;
	int type;//0 for positive

	charge(float x,float y,int getid,int t);
	
	void calcparams();	
	void display();

};
