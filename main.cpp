/**************************
 * Includes
 *
 **************************/



#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <stdio.h>
#include "coulomb.h"

const int count = 100;
const int types = 2;
int num=4;

int positives;
int negatives;
int cx=0;
int cy=0;
RECT rect = {0,0,640,480};
char buffer[1000];
char motion[100]="Random";
HWND hwnd;
float res=1.0;

class charge *q[count];
float mag;
float distx;
float disty;
bool move=false;
bool LINESON=true;

void checkcollision(){
	for (int i=0;i<count;i++){
		if(q[i]->pointx <-100 || q[i]->pointx > 100 )
			q[i]->unitx*=-1;
		if(q[i]->pointy <-100 || q[i]->pointy > 100 )
			q[i]->unity*=-1;
	}
}

void movement(){
	//calculate unit vector
	for (int i=0;i<num;i++){
		q[i]->unitx=0.0;
		q[i]->unity=0.0;
		distx=0.0;
		disty=0.0;
		mag=0.0;
		for (int j=0;j<num;j++){
			if (i!=j){
				distx = q[i]->pointx - q[j]->pointx;
				disty = q[i]->pointy - q[j]->pointy;
				mag = sqrt(pow(distx,2)+pow(disty,2));
				if(mag >=10.0){
					
					if(q[i]->type==q[j]->type){				
						q[i]->unitx += 10*(distx/(mag*mag));
						q[i]->unity += 10*(disty/(mag*mag));
					}
					else{
						q[i]->unitx -= 10*(distx/(mag*mag));
						q[i]->unity -= 10*(disty/(mag*mag));
					}
				}
				
			}
			
		}
	}
	//adding unit vector
	for (int i=0;i<num;i++){
		//for (int j=0;j<count;j++){
			q[i]->pointx+=q[i]->unitx/res;
			q[i]->pointy+=q[i]->unity/res;
		//}
	}
	//checkcollision();
	
	
}

/**************************
 * Function Declarations
 *
 **************************/


void showlines(){
				glBegin(GL_LINES);
				glColor3f(.5,.5,.5);
				for (int k=0;k<num;k++){
					
					for (int j=0;j<num;j++){
						if(k!=j){
						
						glVertex3f(q[k]->pointx,q[k]->pointy,0.0);
						glVertex3f(q[j]->pointx,q[j]->pointy,0.0);
						}
					}
					
				}
				glEnd();
}

void displayprims(){
	
            glClearColor (1.0f, 1.0f, 1.0f, 1.0f);
            glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			glMatrixMode(GL_PROJECTION);

   			glLoadIdentity();

   			//glViewport(0,0,800,600);
			//gluPerspective(45.0,800.0/600.0, 1.0, 100.0);

   			//gluLookAt(0.0, 10.0, 50.0,0.0, 0.0, 0.0,0.0, 1.0, 0.0);
   			//glOrtho(0.0f, 800, 600, 0.0f, -0.1f, 1.0f);
   			//glOrtho(0,640,0,480,-1,1);
   			glOrtho(-133.0, 133.0, -100.0, 100.0, -1.5, 1.5);
   			

   			glMatrixMode(GL_MODELVIEW);
   			//glDisable(GL_BLEND);
   			glLoadIdentity();
   			glTranslatef(-133/2,-100/2,0);
   			//glTranslatef(-50,0,-50);
   			if(move)
			   movement();
			   if(LINESON)
			   showlines();
   			
   				for (int k=0;k<num;k++){
   					q[k]->display();
				}
			   
   			

            


            //Sleep (1);

}

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
WPARAM wParam, LPARAM lParam);
void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC);
void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC);


/**************************
 * WinMain
 *
 **************************/

int WINAPI WinMain (HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine,
                    int iCmdShow)
{
    WNDCLASS wc;
    HWND hWnd;
    HDC hDC;
    HGLRC hRC;        
    MSG msg;
    BOOL bQuit = FALSE;
    
    
    
for (int x=0;x<count;x++)    {
	q[x] = (charge *)malloc(sizeof(charge));	
	//*q[x] = charge(rand()%100,rand()%100,x,x%2);
	*q[x] = charge(3,-5,x,x%2);
	//*q[x] = charge(50,rand()%100,x,x%2);
	//q[x] = new charge(rand()%100,rand()%100,x,x%2);
	
}
    
    

    /* register window class */
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "GLSample";
    RegisterClass (&wc);
    AdjustWindowRect(&rect,WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,false);

    /* create main window */
    hwnd = hWnd = CreateWindow (
      "GLSample", "OpenGL Sample", 
      WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
      0, 0, rect.right, rect.bottom,
      NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd,TRUE);

    /* enable OpenGL for the window */
    EnableOpenGL (hWnd, &hDC, &hRC);

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage (&msg);
                DispatchMessage (&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */
            displayprims();
            SwapBuffers (hDC);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL (hWnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow (hWnd);

    return msg.wParam;
}


/********************
 * Window Procedure
 *
 ********************/

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
                          WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_CREATE:
    		
			sprintf(buffer,"Coulomb %s: +ve=%d , -ve=%d , Total=%d , Move=%s",motion,positives,negatives,num,move?"true":"false");
			SetWindowText(hWnd,buffer);
        return 0;
    case WM_CLOSE:
        PostQuitMessage (0);
        return 0;

    case WM_DESTROY:
        return 0;
    
	case WM_MOUSEMOVE:
		//GetCursorPos(&mp);
		//newmx = LOWORD(lParam);
        //newmy = HIWORD(lParam);
        //ScreenToClient(hwnd,&mp);
         
    break;



    

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        case VK_HOME:
        	
        break;
        case VK_INSERT:

        	
        	
        break;
        case VK_DELETE:
        	LINESON=!LINESON;

        break;
        case VK_PRIOR:
        	

        break;
        case VK_NEXT:
        	
        	
        break;
        case VK_RETURN:
        	move=!move;
        	sprintf(buffer,"Coulomb %s: +ve=%d , -ve=%d , Total=%d , Move=%s",motion,positives,negatives,num,move?"true":"false");
        	SetWindowText(hWnd,buffer);
        break;
        case VK_END:

			for (int x=0;x<num;x++)    {
				//q[x] = (charge *)malloc(sizeof(charge));
				//*q[x] = charge(rand()%100,rand()%100,x,x%2);
				q[x]->pointx = rand()%100;
				q[x]->pointy = rand()%100;
				
				q[x]->type = x%types;				

				
					
					//q[x]->r = 1.0;
					//q[x]->g = 0.0;
				if (q[x]->type==0){
					q[x]->red = 1.0;
					q[x]->green = 0.0;
				}
				else {
					q[x]->red = 0.0;
					q[x]->green = 1.0;
				}

			}
			positives=0;negatives=0;
			for (int x=0;x<num;x++)
			if(q[x]->type==0)positives++ ;else negatives++;
			sprintf(motion,"%s","Alternate");
			sprintf(buffer,"Coulomb %s: +ve=%d , -ve=%d , Total=%d , Move=%s",motion,positives,negatives,num,move?"true":"false");
			SetWindowText(hWnd,buffer);

    	break;
    	case VK_SPACE:
    		for (int x=0;x<num;x++)    {
				//q[x] = (charge *)malloc(sizeof(charge));
				//*q[x] = charge(rand()%100,rand()%100,x,x%2);
				q[x]->pointx = rand()%100;
				q[x]->pointy = rand()%100;
				
				q[x]->type = rand()%types;				

				
					
					//q[x]->r = 1.0;
					//q[x]->g = 0.0;
				if (q[x]->type==0){
					q[x]->red = 1.0;
					q[x]->green = 0.0;
				}
				else {
					q[x]->red = 0.0;
					q[x]->green = 1.0;
				}
					
			}
			positives=0;negatives=0;
			for (int x=0;x<num;x++)
			if(q[x]->type==0)positives++ ;else negatives++;
			sprintf(motion,"%s","Random");
			sprintf(buffer,"Coulomb %s: +ve=%d , -ve=%d , Total=%d , Move=%s",motion,positives,negatives,num,move?"true":"false");
			
			SetWindowText(hWnd,buffer);
    	break;
        
        
        case VK_UP:
        	
        	
			num++;
			if (num >= 100)
			num = 99;
			if(strcmp(motion,"Random")==0)
			q[num]->type=rand()%types;
			else
			q[num]->type=num%types;
			
			positives=0;negatives=0;
			for (int x=0;x<num;x++)
			if(q[x]->type==0)positives++ ;else negatives++;
			sprintf(buffer,"Coulomb %s: +ve=%d , -ve=%d , Total=%d , Move=%s",motion,positives,negatives,num,move?"true":"false");
        	SetWindowText(hWnd,buffer);
        break;
        case VK_DOWN:
			num--;
			if (num <0)        	
			num =0;
			
			positives=0;negatives=0;
			for (int x=0;x<num;x++)
			if(q[x]->type==0)positives++ ;else negatives++;
			sprintf(buffer,"Coulomb %s: +ve=%d , -ve=%d , Total=%d , Move=%s",motion,positives,negatives,num,move?"true":"false");
			SetWindowText(hWnd,buffer);
			
        break;
		case VK_LEFT:
			
        break;
		case VK_RIGHT:
			
        break;		        
            return 0;
        }
        return 0;

    default:
        return DefWindowProc (hWnd, message, wParam, lParam);
    }
}


/*******************
 * Enable OpenGL
 *
 *******************/

void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC)
{
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC (hWnd);

    /* set the pixel format for the DC */
    ZeroMemory (&pfd, sizeof (pfd));
    pfd.nSize = sizeof (pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | 
      PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat (*hDC, &pfd);
    SetPixelFormat (*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext( *hDC );
    wglMakeCurrent( *hDC, *hRC );

}


/******************
 * Disable OpenGL
 *
 ******************/

void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent (NULL, NULL);
    wglDeleteContext (hRC);
    ReleaseDC (hWnd, hDC);
}
