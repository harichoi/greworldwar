#include <windows.h>
#include <Stdio.h>
#include <math.h>
#include <string.h>
#include <memory.h>
#include "resource.h"
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library

#define PI 3.1415926535
#define Ax (30)//*PI/180
#define Ay (30)//*PI/180
#define Az (30)//*PI/180
#define torad(a) ((a)/180.0*3.141592)
#define GRAPH(a,b) (sin(double((a)/50.0)+sin(double((b)/50.0)))*30)//+(sin(sqrt((a)*(a)+(b)*(b)))*5))/**///(sin(a/2+sin(b/2))*30.0)
#define absol(a) (((a)<0)?(-(a)):(a))
#define DIRECT 0
#define SPLIT 1.0
#define HH 15.0
#define DET 20

struct point
{
	double x;
	double y;
	double z;
};
struct Layer
{
	int np;
	point p[10];
	point nor;
	double dist;
	COLORREF rgb;
};
struct Shape
{
	char name[100];
	int Fe;
	int nlayer;
	Layer layer[2000];
	int energy;
	int v;
	double makeftime;
	
	char explain[7][100];
	char but[7][20];
	
	void option(char _name[100],double _makeftime, int _energy, int _v, int _Fe)
	{
		int i;
		
		for(i=0 ; i<100 ; i++)
		{
			name[i]=_name[i];
		}
		makeftime=_makeftime;
		energy=_energy;
		v=_v;
		Fe=_Fe;
	}
	void makebut(char but1[20],char but2[20],char but3[20],char but4[20],char but5[20],char but6[20])
	{
		int i;
		for(i=0 ; i<20 ; i++)
		{
			but[1][i]=but1[i];
			but[2][i]=but2[i];
			but[3][i]=but3[i];
			but[4][i]=but4[i];
			but[5][i]=but5[i];
			but[6][i]=but6[i];
		}
	}
	void makeex(char ex[100],char ex2[100],char ex3[100],char ex4[100],char ex5[100],char ex6[100])
	{
		int i;
		for(i=0 ; i<100 ; i++)
		{
			explain[1][i]=ex[i];
			explain[2][i]=ex2[i];
			explain[3][i]=ex3[i];
			explain[4][i]=ex4[i];
			explain[5][i]=ex5[i];
			explain[6][i]=ex6[i];
		}
	}
};
struct Unit
{
	int hard;
	int going;
	int delay;
	int maxdelay;
	int what; //0번 폭탄 1번 육군 2번 공군 3번 해군 4번 건물 5번 자원 6번 건물 짓는중 7번 자원 짓는중
	int team;
	int kind;
	double energy;
	point pt;
	point ft;
	point center[2000];
//	point cen;
//	point cen2;
//	point calpt;
	double ax;
	double ay;
	double az;
	int order[2000];
	double v;
	double viewx;
	double viewy;
	//double ft.x;
	//double ft.z;
	double making;
	int temp;
	int temp2[100];
	int temp3;
	int target;
	void createunit(int _what,int _team, int _kind, double x, double y, double z);
	void createbomb(double _fx, double _fy)
	{
		ax=90;
		ft.x=_fx;
		ft.z=_fy;
		v=20;
	}
};
class NameControl
{
public:
	NameControl(){
		n=1000;
	}
	void init()
	{
		n=1000;
	}
	point where[10000];
	int n;
	void put(double x,double z)
	{
		where[++n].x=x;
		where[n].z=z;
	}
};

int fx, fy, sx, sy;
int Fe[100]={200,20000,200,200,200,200,};

HINSTANCE hInstance;
double x; double y;
double av;
double rotate;
double v;
double nowx, nowy, nowz;
double aax, aay, aaz;
GLint xsize, ysize;
GLfloat ratio;
		point bb;

NameControl mapctrl;
Shape shape[101];
int keybd[256];
int aaa;
double defx, defy;
double map[101][101];
point mapp[101][101];
Unit unit[2000];
int sunit[1001];
int alltime;
int sbuild;
int ssunit;
int smapx, smapy;
HBITMAP downbit;
HDC hDC=0;
HGLRC hRC=0;

int list[101][7]=
{
	{0, 0,0,0,0,0,0},//아래.

	{0, 3,4,8,0,0,0},//Solget
	{0, 0,0,0,0,0,0},//폭탄
	{0, 1,0,0,0,0,0},//센터
	{0, 0,0,0,0,0,0},//자원 Fe
	{0, 1,2,3,4,5,6},//Twobox
	{0, 0,0,0,0,0,0},//diatank
	{0, 0,0,0,0,0,0},//forcetank
	{0, 5,0,0,0,0,0},//trabuild
};

LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
void init();
void draw(HWND hWnd, GLenum mode);
void stage(int level);
void rot(double ax, double ay, double az, double &x, double &y, double &z);
void autoattack();
void computer();
void fileopen(char *filename,int number,double div);
double minus(double a);
point makepoint(double xx, double yy, double zz);
// Open GL
void ReSizeGLScene(GLsizei width, GLsizei height);
int InitGL(HWND hWnd);
void KillGLWindow(HWND hWnd);
void lbut(HWND hWnd,GLint xx,GLint yy);
void slbut(HWND hWnd,GLint xx,GLint yy);
void rbut(HWND hWnd,GLint xx,GLint yy);


int WINAPI WinMain(HINSTANCE hInst,
				   HINSTANCE hPrev,
				   LPSTR lpCmd,
				   int nShow)
{
	downbit=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_DOWN));
	
	srand(GetTickCount());
	HWND hWnd;
	WNDCLASS wnd;
	MSG msg;
	hInstance=hInst;

	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = 0;
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);//LoadCursor(hInst,MAKEINTRESOURCE(IDC_CURSOR1));
	wnd.hIcon = LoadIcon(NULL, IDI_WINLOGO);//LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON1));
	wnd.hInstance = hInst;
	wnd.lpfnWndProc = WndProc;
	wnd.lpszClassName = "greworldwar";
	wnd.lpszMenuName =0;// MAKEINTRESOURCE(IDR_MENU1);
	wnd.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	RegisterClass(&wnd);

	//if(DIRECT)
	//hWnd=CreateWindow("greworldwar","세계대전",WS_POPUP,0,0,0,0,0,0,hInst,0);
	//else
	
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)800;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)600;		// Set Bottom Value To Requested Height
	AdjustWindowRectEx(&WindowRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);		// Adjust Window To True Requested Size

	hWnd=CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,"greworldwar","세계대전",WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN | WS_CLIPSIBLINGS,CW_USEDEFAULT,CW_USEDEFAULT,WindowRect.right-WindowRect.left,WindowRect.bottom-WindowRect.top,0,0,hInst,0);
	ShowWindow(hWnd,SW_SHOW);

//	selectx++;selecty++;
	/*while(GetMessage(&msg,0,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		
	//	draw(hWnd,GL_RENDER);
			SwapBuffers(hDC);
	}*/
	while(1)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				break;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
				SwapBuffers(hDC);					// Swap Buffers (Double Buffering)
			
		}
	}
	return 0;
}
int visit[100];
int check;
int mx, my,mmx,mmy;
LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
		int chk=0;
	int i, j, k, temp;
	double ax, ay;
	switch(msg)
	{
	case WM_SIZE:
		ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
		return 0;
		break;
	case WM_CLOSE:
		KillGLWindow(hWnd);
		PostQuitMessage(1);
		break;
	case WM_DESTROY:
	//	CloseDDraw();
		KillGLWindow(hWnd);
		PostQuitMessage(1);
		break;
	case WM_CREATE:
		init();
		InitGL(hWnd);
		return 0;
	//	if(DIRECT){
	//	Start(hWnd,"?");
	//	}
		break;
	case WM_RBUTTONDOWN:
		rbut(hWnd,LOWORD(lParam),HIWORD(lParam));
		break;
		mx=ax=LOWORD(lParam)+100-defx;
		my=ay=HIWORD(lParam)+100-defy;
		temp=0;
		for(k=1 ; k<=1000 ; k++){
			if(sunit[k] && unit[k].energy>0){
				for(i=1 ; i<=1000 ; i++)
				{
					if(unit[i].energy<=0 || unit[i].team==unit[k].team)
						continue;
					unit[i].temp=0;
					if(unit[i].pt.x>ax-25 && unit[i].pt.x<ax+25)
						if(unit[i].pt.z>ay-25 && unit[i].pt.z<ay+25)
						{
							for(j=1 ; j<=1000 ; j++)
							{
								if(unit[j].energy<=0)
								{
									unit[j].createunit(0,unit[k].team,2,unit[k].pt.x,unit[k].pt.y,unit[k].pt.z);
									unit[j].createbomb(unit[i].pt.x,unit[i].pt.z);
									temp=1;
									break;
								}
							}
							break;
						}
				}
			}
		}
		
		if(!temp)
		for(i=1 ; i<=1000 ; i++)
		{
			if(unit[i].energy>0)
			if(sunit[i])
			{
				unit[i].ft.x=mx;
				unit[i].ft.z=my;
				unit[i].v=5;
				unit[i].going=1;
				unit[i].temp=0;
			}
		}
		return 0;
		break;
		/*
		mx=ax=LOWORD(lParam)+100-deft.x;
		my=ay=HIWORD(lParam)+100-deft.z;

		for(i=1 ; i<=1000 ; i++)
		{
			if(unit[i].energy>0)
			if(sunit[i])
			{
				unit[i].ft.x=mx;
				unit[i].ft.z=my;
				unit[i].v=5;
			}
		}
		for(k=1 ; k<=1000 ; k++){
			if(sunit[k] && unit[k].energy>0)
		for(i=1 ; i<=1000 ; i++)
		{
			if(unit[i].energy<=0)
				continue;
			if(unit[i].pt.x>ax-25 && unit[i].pt.x<ax+25)
				if(unit[i].pt.z>ay-25 && unit[i].pt.z<ay+25)
				{
					for(j=1 ; j<=1000 ; j++)
					{
						if(unit[j].energy<=0)
						{
							unit[j].createunit(0,1,2,unit[k].pt.x,unit[k].pt.y,unit[k].pt.z,30);
							unit[j].createbomb(unit[k].ft.x,unit[k].ft.z);
							break;
						}
					}
					break;
				}
		}
		}
		break;*/
	case WM_LBUTTONUP:
		slbut(hWnd,LOWORD(lParam),HIWORD(lParam));
		return 0;
		break;
	case WM_LBUTTONDOWN:
		lbut(hWnd,LOWORD(lParam),HIWORD(lParam));
		return 0;
		break;
		/*mx=ax=LOWORD(lParam)+100-defx;
		my=ay=HIWORD(lParam)+100-defy;
		if(my+defy<550){
		if(!keybd[VK_SHIFT]){
		for(i=1 ; i<=1000 ; i++)
		{
			sunit[i]=0;
		}
		}
		sbuild=0;
		ssunit=0;
		
		for(i=1 ; i<=1000 ; i++)
		{
			if(unit[i].what==4 && unit[i].team==1 && unit[i].energy>0)	
			if(unit[i].calpt.x>ax-150 && unit[i].calpt.x<ax+150)
				if(unit[i].calpt.y>ay-75 && unit[i].calpt.y<ay+75)
					sbuild=i;
			if(sunit[i]!=1 && unit[i].what==1 && unit[i].team==1 && unit[i].energy>0)
			if(unit[i].pt.x>ax-25 && unit[i].calpt.x<ax+25)
				if(unit[i].pt.z>ay-25 && unit[i].calpt.y<ay+25)
				{
					sunit[i]=1;
					ssunit=i;
					sbuild=0;
					break;
				}
			}	
		}
		else
		{
			if(sbuild>0)
			{
				if(unit[sbuild].temp3>0)
				{
					if(Fe[1]>=shape[list[unit[sbuild].kind][unit[sbuild].temp3]].Fe)
					{
						Fe[1]-=shape[list[unit[sbuild].kind][unit[sbuild].temp3]].Fe;
					for(i=1 ; i<=100 ; i++){
						if(unit[sbuild].temp2[i]<=0){
							unit[sbuild].temp2[i]=unit[sbuild].temp3;
							break;
						}
					}
					}
				}
			}
			if(ssunit>0)
			{
				if(unit[ssunit].temp3>0)
				{
					if(Fe[1]>=shape[list[unit[ssunit].kind][unit[ssunit].temp3]].Fe)
					{
						double dist, min=999999;
						for(i=1 ; i<=1000 ; i++){
							if(unit[i].energy>0 && (unit[i].what==4 || unit[i].what==5 || unit[i].what==6 || unit[i].what==7)){
								dist=hypot(unit[ssunit].pt.x-unit[i].pt.x,unit[ssunit].pt.z-unit[i].pt.z);
								if(min>dist)
									min=dist;
							}
						}
						if(min>=300)
						{
							for(i=1 ; i<=1000 ; i++)
							{
								if(unit[i].energy<=0){
									if(list[unit[ssunit].kind][unit[ssunit].temp3]==4)
										unit[i].createunit(7,1,list[unit[ssunit].kind][unit[ssunit].temp3],unit[ssunit].pt.x,0,unit[ssunit].pt.z);
									else
										unit[i].createunit(6,1,list[unit[ssunit].kind][unit[ssunit].temp3],unit[ssunit].pt.x,0,unit[ssunit].pt.z);
									break;
								}
							}
							Fe[1]-=shape[list[unit[ssunit].kind][unit[ssunit].temp3]].Fe;
							if(list[unit[ssunit].kind][unit[ssunit].temp3]>0)
								unit[ssunit].energy=0;
						}
					}
					if(unit[ssunit].temp3==6 && unit[ssunit].kind==1)
					{
						unit[ssunit].temp=1;
					}
				}
			}
		}*/
		break;
	case WM_MOUSEMOVE:
		return 0;
		break;/*
		mmx=ax=LOWORD(lParam)+100;
		mmy=ay=HIWORD(lParam)+100;
		mmx-=100;
		mmy-=100;
		


		if(ax>700 && ax<785)
		{
			if(ay>540 && ay<570)
			{
				unit[sbuild].temp3=1;
				unit[ssunit].temp3=1;
			}
			if(ay>580 && ay<610)
			{
				unit[sbuild].temp3=3;
				unit[ssunit].temp3=3;
			}
			if(ay>620 && ay<650)
			{
				unit[sbuild].temp3=5;
				unit[ssunit].temp3=5;
			}
		}
		else if(ax>800 && ax<885)
		{
			if(ay>540 && ay<570)
			{
				unit[sbuild].temp3=2;
				unit[ssunit].temp3=2;
			}
			if(ay>580 && ay<610)
			{
				unit[sbuild].temp3=4;
				unit[ssunit].temp3=4;
			}
			if(ay>620 && ay<650)
			{
				unit[sbuild].temp3=6;
				unit[ssunit].temp3=6;
			}
		}
		else{
			unit[sbuild].temp3=0;
			unit[ssunit].temp3=0;
		}*/
		break;
	case WM_TIMER:
		
		alltime++;
	//	if(alltime%20==0)
			autoattack();
			computer();
			/*
		if(alltime%50==0)
		{
			for(i=1 ; i<=1000 ; i++){
				if(unit[i].energy<=0){
					unit[i].createunit(1,1,1,1200+rand()%100,200,1200+rand()%100);
					break;
				}
			}
		}*/
		if(alltime%40==0){
			for(i=1 ; i<=100 ; i++){
				if(unit[i].energy<=0){
					unit[i].createunit(1,1,5,300+rand()%100,1000,300+rand()%100);
					break;
				}
			}
			for(i=100 ; i<=200 ; i++){
				if(unit[i].energy<=0){
					unit[i].createunit(1,3,5,1000+rand()%100,1000,1000+rand()%100);
					break;
				}
			}
		}
		//밀쳐내기, 밀어내기
		
		for(i=1 ; i<=1000 ; i++){
			unit[i].delay++;
			if(unit[i].what==4 && unit[i].energy>0)
			{
				if(unit[i].temp2[1]>0)
					unit[i].temp++;
				if(unit[i].temp>shape[list[unit[i].kind][unit[i].temp2[1]]].makeftime)
				{
					unit[i].temp=0;
					for(j=1 ; j<=1000 ; j++)
					{
						if(unit[j].energy<=0){
							unit[j].createunit(1,1,list[unit[i].kind][unit[i].temp2[1]],unit[i].pt.x,unit[i].pt.y,unit[i].pt.z+100);
							break;
						}
					}
					for(j=1 ; j<=90 ; j++)
						unit[i].temp2[j]=unit[i].temp2[j+1];
				}
			}
			if(sunit[i])
				if(unit[i].energy<=0)
					sunit[i]=0;
				if(unit[i].temp==0 || unit[i].kind!=1)
			for(j=1 ; j<=1000 ; j++)
			{
				if(unit[i].energy>0 && unit[j].energy>0 && unit[i].what==unit[j].what && (unit[j].temp==0 || unit[j].kind!=1))
				{
					if(absol(unit[i].pt.x-unit[j].pt.x)<60)
					{
						if(absol(unit[i].pt.z-unit[j].pt.z)<60)
						{
							if(unit[i].pt.x<unit[j].pt.x)
							{
								unit[i].pt.x-=2;
								unit[i].ft.x-=4;
								unit[j].pt.x+=2;
								unit[j].ft.x+=4;
							}
							if(unit[i].pt.z<unit[j].pt.z)
							{
								unit[i].pt.z-=2;
								unit[i].ft.z-=4;
								unit[j].pt.z+=2;
								unit[j].ft.z+=4;
		
							}
						}
					}
				}
			}
		}
		
		for(k=1 ; k<=1000 ; k++)
		{//what 계산부분
			if(unit[k].energy<=0)
				continue;
			
			if(unit[k].what==5){
				if(unit[k].kind==4)//Fe
				{
					unit[k].temp=0;
					for(i=1 ; i<=1000 ; i++)
					{
						if(unit[i].energy>0 && unit[i].kind==1)
						{
							if(absol(unit[k].pt.x-unit[i].pt.x)+absol(unit[k].pt.y-unit[i].pt.y)<50)
							{
								unit[i].temp3++;
								if(unit[i].temp3>50)
									unit[i].temp3=50;
								unit[k].temp=i;
								break;
							}
						}
					}
				}
			}
			
			if(unit[k].what==6 || unit[k].what==7){
			
				unit[k].ax=unit[k].az=0;
							unit[k].ay+=0.01*180/PI;
							unit[k].making++;
					if(unit[k].making>shape[unit[k].kind].makeftime)
					{
						if(unit[k].what==6)
							unit[k].what=4;
						else if(unit[k].what==7)
							unit[k].what=5;
					}
					
			}
			if(unit[k].what==4){
				
				if(unit[k].kind==3)//Center
				{
					for(i=1 ; i<=1000 ; i++)
					{
						if(unit[i].energy>0 && unit[i].kind==1)
						{
							if(absol(unit[k].pt.x-unit[i].pt.x)+absol(unit[k].pt.y-unit[i].pt.y)<150)
							{
								Fe[unit[i].team]+=unit[i].temp3/3;
								unit[i].temp3=0;
							}
						}
					}
				}
			}
			
			if(unit[k].what==1){
				if(unit[k].kind==1)
				{
					if(unit[k].temp==1)
					{
						if(unit[k].temp3>=50)
						{
							//본진으로
							double min;
							int mini;
							min=99999;
							for(i=1 ; i<=1000 ; i++)
							{
								if(unit[i].team==unit[k].team){
									if(unit[i].kind==3)
									{
										double a=absol(unit[i].pt.x-unit[k].pt.x)+absol(unit[i].pt.y-unit[k].pt.y);
										if(min>a)
											min=a, mini=i;
									}
								}
							}
							unit[k].ft=unit[unit[k].target].pt;
							
						}
						else
						{
							//자원캐러
							double min;
							int mini;
							min=99999;
							
							if(unit[k].target>0){
							unit[k].ft=unit[unit[k].target].pt;
							//	if(absol(unit[k].ft.x-unit[unit[k].target].pt.x)+absol(unit[k].ft.z-unit[unit[k].target].pt.z)<=10 && unit[unit[k].target].temp!=k)
							//		unit[k].target=0;
							}
							else{
								mini=0;
								for(i=1 ; i<=1000 ; i++)
								{
									if(unit[i].kind==4 && unit[i].energy>0 && unit[i].team==unit[k].team && unit[i].temp==0)
									{
										double a=absol(unit[i].pt.x-unit[k].pt.x)+absol(unit[i].pt.z-unit[k].pt.z);
										if(min>a)
											min=a, mini=i;
									}
								}
								unit[k].target=mini;
							}
							
						}
					}
					
				}
				//모든 유닛 공통
				if(abs(int(unit[k].ft.z-unit[k].pt.z))>10 || abs(int(unit[k].ft.x-unit[k].pt.x))>10)
				{
					double aa, ab;
					aa=(unit[k].ft.x-unit[k].pt.x);
					ab=-(unit[k].ft.z-unit[k].pt.z);
					if(aa!=0)
					unit[k].ay=atan(ab/aa)*180/PI+45;
					else
						unit[k].ay=180;
					if(aa<0)
					unit[k].ay+=45;
					if(aa>0)
					unit[k].ay+=225;
					unit[k].v=shape[unit[k].kind].v;
				}
				else
				{
					unit[k].v=0;
					unit[k].going=0;
					unit[k].ft=unit[k].pt;
				}
				
				unit[k].ay=minus(unit[k].ay);
				if(unit[k].ay>180*2)
					unit[k].ay-=180*2;
				double k1,k2;
				k1=(unit[k].pt.x);
				k2=(unit[k].pt.z);
				unit[k].pt.y=GRAPH(k1,k2);
				if(unit[k].ft.x>-900){
				unit[k].pt.x-=sin(torad(unit[k].ay))*unit[k].v;
				unit[k].pt.z-=cos(torad(unit[k].ay))*unit[k].v;
				}
				double hx;
				if(unit[k].ay<=90 || unit[k].ay>=270 && unit[k].ay<=360)
					hx=((GRAPH(k1,k2+1)-GRAPH(k1,k2)));
				else if(unit[k].ay>=90 && unit[k].ay<=180 || unit[k].ay>=180 && unit[k].ay<=270)
					hx=(GRAPH(k1,k2)-GRAPH(k1,k2+1));

				double hz;
				if(unit[k].ay<=90 || unit[k].ay>=270 && unit[k].ay<=360)
					hz=(GRAPH(k1,k2)-GRAPH(k1+1,k2));
				else if(unit[k].ay>=90 && unit[k].ay<=180 || unit[k].ay>=180 && unit[k].ay<=270)
					hz=(GRAPH(k1+1,k2)-GRAPH(k1,k2));
				unit[k].ax=atan(hx)*180/PI;
				unit[k].az=atan(hz)*180/PI;
				
				
			}
			
			if(unit[k].what==0){
				if(unit[k].pt.y<=GRAPH(unit[k].pt.x,unit[k].pt.z) || unit[k].ax>=0)
				{
						
					if(abs(int(unit[k].ft.z-unit[k].pt.z))>10 || abs(int(unit[k].ft.x-unit[k].pt.x))>10)
					{
						if(unit[k].ax>=0.5){
						double aa, ab;
						aa=unit[k].ft.x-unit[k].pt.x;
						ab=-(unit[k].ft.z-unit[k].pt.z);
						if(aa!=0)
						unit[k].ay=atan(ab/aa)*180/PI+180/4;
						else
							unit[k].ay=180;
						
					if(aa<0)
					unit[k].ay+=45;
					if(aa>0)
					unit[k].ay+=225;
						}
					}
					else
					{
			//			unit[k].ay=-1;
			//			unit[k].pt.x=unit[k].ft.x;
			//			unit[k].pt.z=unit[k].ft.z;
					}
					
				} 
				else
				{
					unit[k].v=0;
					for(i=1 ; i<=1000 ; i++)
					{
						if(unit[k].team==unit[i].team || unit[i].energy<=0)
							continue;
						
						if(absol(unit[k].pt.x-unit[i].pt.x)<40)
						{
							if(absol(unit[k].pt.z-unit[i].pt.z)<40)
							{
								unit[i].energy-=unit[k].energy;
						//		break;
							}
						}
					
					}
					unit[k].energy=0;
				}
				if(unit[k].ay>180*2)
					unit[k].ay-=180*2;
					
				double k1,k2;
				k1=(unit[k].pt.x)/SPLIT;
				k2=(unit[k].pt.z)/SPLIT;
				unit[k].pt.y-=(sin(torad(unit[k].ax))+sin(torad(unit[k].az)))*unit[k].v;
				if(unit[k].ay!=-1){
				unit[k].pt.x-=sin(torad(unit[k].ay))*unit[k].v;
				unit[k].pt.z-=cos(torad(unit[k].ay))*unit[k].v;
				}
				unit[k].ax+=(-90-unit[k].ax)*0.1;
				unit[k].az=0;
				
			}
		}
	//	aay+=0.1;
/*
		for(k=1 ; k<=1000 ; k++){
			if(unit[k].energy<=0)
				continue;
		//	unit[k].calpt=unit[k].pt;
		//	rot(Ax,Ay,Az,unit[k].calpt.x,unit[k].calpt.y,unit[k].calpt.z);
			
			//cen 계산
		unit[k].pt.x=0;
		unit[k].pt.z=0;
		unit[k].cen2.x=0;
		unit[k].cen2.y=0;
		for(i=1 ; i<=shape[unit[k].kind].nlayer ; i++)
		{
			unit[k].order[i]=i;
			point aa;
			point bb;
			aa.x=aa.y=aa.z=0;
			for(j=1 ; j<=shape[unit[k].kind].layer[i].np ; j++)
			{
				aa.x+=shape[unit[k].kind].layer[i].p[j].x;
				aa.y+=shape[unit[k].kind].layer[i].p[j].y;
				aa.z+=shape[unit[k].kind].layer[i].p[j].z;
			}
			if(shape[unit[k].kind].layer[i].np>=1){
			aa.x/=shape[unit[k].kind].layer[i].np;
			aa.y/=shape[unit[k].kind].layer[i].np;
			aa.z/=shape[unit[k].kind].layer[i].np;
			}
			rot(unit[k].ax,unit[k].ay,unit[k].az,aa.x,aa.y,aa.z);
			aa.x+=unit[k].pt.x;
			if(unit[k].what==1)
			aa.y+=unit[k].pt.y;
			else
				aa.y=0;
			aa.z+=unit[k].pt.z;
			bb=aa;

			bb.y=0;
			rot(Ax,Ay,Az,aa.x,aa.y,aa.z);
			unit[k].center[i]=aa;
		//	unit[k].pt.x+=aa.x;
		//	unit[k].pt.z+=aa.y;
			
		}
		//unit[k].pt.x=unit[k].pt.x/shape[unit[k].kind].nlayer;
		//unit[k].pt.z=unit[k].pt.z/shape[unit[k].kind].nlayer;
	//	unit[k].cen=unit[k].calpt;
		if(unit[k].ft.x<-900)
				unit[k].ft.x=unit[k].pt.x;
		if(unit[k].ft.z<-900)
				unit[k].ft.z=unit[k].pt.z;
	//	unit[k].cen2.x=unit[k].cen2.x/shape[unit[k].kind].nlayer;
	//	unit[k].cen2.y=unit[k].cen2.y/shape[unit[k].kind].nlayer;
		for(i=1 ; i<=shape[unit[k].kind].nlayer ; i++)
			for(j=1 ; j<=shape[unit[k].kind].nlayer ; j++)
			{
				double ppi, ppj;
				ppi=unit[k].center[unit[k].order[i]].z;
				ppj=unit[k].center[unit[k].order[j]].z;
				if(ppi>ppj && unit[k].what<=5)
				{
					int a;
					a=unit[k].order[i];
					unit[k].order[i]=unit[k].order[j];
					unit[k].order[j]=a;
				}
			}
		}
		*/
		if(keybd[VK_LEFT])
			defx+=1;
		if(keybd[VK_RIGHT])
			defx-=1;
		if(keybd[VK_UP])
			defy-=1;
		if(keybd[VK_DOWN])
			defy+=1;
	/*
		if(mmx<=2)
			deft.x+=50;
		if(mmx>=798)
			deft.x-=50;
		if(mmy<=2)
			deft.z+=50;
		if(mmy>=598)
			deft.z-=50;*/
		draw(hWnd,GL_RENDER);
		return 0;
		break;
	case WM_KEYDOWN:
		keybd[wParam]=1;
		return 0;
		break;
	case WM_KEYUP:
		keybd[wParam]=0;
		return 0;
		break;
	case WM_COMMAND:
		return 0;
		break;
	case WM_PAINT:
		SetTimer(hWnd,0,50,0);
		SetFocus(hWnd);
		draw(hWnd,GL_RENDER);
		break;
	};
	return DefWindowProc(hWnd,msg,wParam,lParam);
}

double ee;
/*
void draw(HWND hWnd)
{
	int i, j, k;
//	char str[256];
	HBITMAP bit, *old, *old2;
//	HBRUSH *old3, brush;
	HDC hDC;
	HDC MemDC;
	HDC bitDC;

	//hDC=GetDC(hWnd);
//	DDBLTft.x ddbltft.x;
//	ddbltft.x.dwSize=sizeof(ddbltft.x);
//	ddbltft.x.dwFillColor=255;
//	lpDDS1->Blt(0,0,0,DDBLT_COLORFILL|DDBLT_WAIT,&ddbltft.x);
//	if(DIRECT)
//	lpDDS1->GetDC(&hDC);
//	else
		hDC=GetDC(hWnd);

	bit=CreateCompatibleBitmap(hDC,1200,1200);
	MemDC=CreateCompatibleDC(hDC);
	bitDC=CreateCompatibleDC(hDC);
	//hDC=BeginPaint(hWnd,&ps);
	old = (HBITMAP *)SelectObject(MemDC,bit);

	Rectangle(MemDC,0,0,1024,768);

	SetBkMode(MemDC,0);

	//배경  출력
	//	old2=(HBITMAP *)SelectObject(bitDC,bgbit);
	//	BitBlt(MemDC,0,0,1024,768,bitDC,0,0,SRCCOPY);
	//	SelectObject(bitDC,old2);

	double xx, yy, zz;

	for(i=1 ; i<=99 ; i++)
	{
		for(j=1 ; j<=99 ; j++){
			if(mapp[i][j].x+deft.x>0 && mapp[i][j].y+deft.z>0 && mapp[i][j].x+deft.x<900 && mapp[i][j].y+deft.z<700){
	HPEN pen, *op;
	HBRUSH brs, *ob;
	pen=CreatePen(0,1,RGB(0,0,0));//RGB((map[i][j]+map[i][j+1]+map[i+1][j+1])*2+50,255,(map[i][j]+map[i][j+1]+map[i+1][j+1])*2+50));//(l-mmin)/(mmax-mmin)*255,(m-mmmin)/(mmmax-mmmin)*255,(l-mmin)/(mmax-mmin)*127+(m-mmmin)/(mmmax-mmmin)*127));//secondc);
	op=(HPEN *)SelectObject(MemDC,pen);
	brs=CreateSolidBrush(RGB(150,150,(map[i][j]+map[i][j+1]+map[i+1][j+1])+60));
	ob=(HBRUSH *)SelectObject(MemDC,brs);
			POINT a[5];
			a[0].x=mapp[i][j].x+deft.x;
			a[0].y=mapp[i][j].y+deft.z;
			a[1].x=mapp[i][j+1].x+deft.x;
			a[1].y=mapp[i][j+1].y+deft.z;
			a[2].x=mapp[i+1][j+1].x+deft.x;
			a[2].y=mapp[i+1][j+1].y+deft.z;
			a[3].x=mapp[i+1][j].x+deft.x;
			a[3].y=mapp[i+1][j].y+deft.z;
			Polygon(MemDC,a,4);
	SelectObject(MemDC,ob);
	SelectObject(MemDC,op);
	DeleteObject(op);
	DeleteObject(ob);
	DeleteObject(pen);
	DeleteObject(brs);
			}
		}
	}
	for(k=1 ; k<=1000 ; k++){
		if(unit[k].energy<=0)
			continue;
		point rr;
		rr=unit[k].calpt;
		point temp;
		temp=rr;
		if(sbuild==k)
			if(rr.x-30+deft.x>0 && rr.x-30+deft.x<900 && rr.y-30+deft.z>0 && rr.y-30+deft.z<700){
				HBRUSH brs, *ob;
				brs=(HBRUSH)GetStockObject(NULL_BRUSH);
				ob=(HBRUSH *)SelectObject(MemDC,brs);
		Ellipse(MemDC,rr.x-150+deft.x,rr.y-75+deft.z,rr.x+150+deft.x,rr.y+75+deft.z);
		SelectObject(MemDC,ob);
		DeleteObject(brs);
		DeleteObject(ob);
		

			}
		if(sunit[k])
			if(rr.x-30+deft.x>0 && rr.x-30+deft.x<900 && rr.y-30+deft.z>0 && rr.y-30+deft.z<700){
				HBRUSH brs, *ob;
				brs=(HBRUSH)GetStockObject(NULL_BRUSH);
				ob=(HBRUSH *)SelectObject(MemDC,brs);
		Ellipse(MemDC,rr.x-40+deft.x,rr.y-20+deft.z,rr.x+40+deft.x,rr.y+20+deft.z);
		SelectObject(MemDC,ob);
		DeleteObject(brs);
		DeleteObject(ob);
		

			}
		if(unit[k].what==1 || unit[k].what==3)
			Rectangle(MemDC,rr.x-unit[k].energy/10+deft.x,rr.y+15+deft.z,rr.x+unit[k].energy/10+deft.x,rr.y+20+deft.z);
		else if(unit[k].what!=0)
			Rectangle(MemDC,rr.x-unit[k].energy/10+deft.x,rr.y+50+deft.z,rr.x+unit[k].energy/10+deft.x,rr.y+55+deft.z);
		rr=unit[k].pt;
		rr.y=GRAPH(rr.x/SPLIT,rr.z/SPLIT);
		rot(Ax,Ay,Az,rr.x,rr.y,rr.z);
		HBRUSH brs, *ob;
		brs=(HBRUSH)GetStockObject(BLACK_BRUSH);
		ob=(HBRUSH *)SelectObject(MemDC,brs);
		SelectObject(MemDC,brs);
		Ellipse(MemDC,rr.x-6+deft.x,rr.y-4+deft.z,rr.x+6+deft.x,rr.y+4+deft.z);
		SelectObject(MemDC,ob);
		DeleteObject(brs);
		DeleteObject(ob);
		
	}
	int u;
	//0번폭탄 1번육군 2번공군 3번해군 4번건물 5번자원 6번건물짓는중 7번자원짓는중
	int iii[8]={0,4,6,1,5,7,0,2};
	for(u=1 ; u<=7 ; u++)
	for(k=1 ; k<=1000 ; k++){

		if(unit[k].energy<=0 || unit[k].what!=iii[u])
			continue;
		int uuu;
		uuu=shape[unit[k].kind].nlayer;
		if(unit[k].what==6 || unit[k].what==7)
		{
			uuu=double(uuu)*(double(unit[k].making)/double(shape[unit[k].kind].makeftime));
		}
		for(i=1 ; i<=uuu ; i++)
		{
			Layer o;
			o=shape[unit[k].kind].layer[unit[k].order[i]];
			POINT a[1000];
			for(j=1 ; j<=o.np ; j++)
			{
				xx=o.p[j].x;
				yy=o.p[j].y;
				zz=o.p[j].z;
				//rot(unit[k].ax-Ax,unit[k].ay-Ay,unit[k].az-Az,xx,yy,zz);
				if(unit[k].what>=1){
				rot(0,unit[k].ay,0,xx,yy,zz);
				rot(unit[k].ax,0,unit[k].az,xx,yy,zz);
				}
				if(unit[k].what==0){
				rot(unit[k].ax,0,unit[k].az,xx,yy,zz);
				rot(0,unit[k].ay,0,xx,yy,zz);
				}
				xx+=unit[k].pt.x;
				yy+=unit[k].pt.y;
				zz+=unit[k].pt.z;
				rot(Ax,Ay,Az,xx,yy,zz);
				a[j-1].x=xx+deft.x;
				a[j-1].y=yy+deft.z;
			}
			if(o.np>=3 && a[1].x>0 && a[1].y>0 && a[1].x<900 && a[1].y<700)
			{
			HBRUSH brs, *ob;
	
			COLORREF f;
			f=shape[unit[k].kind].layer[unit[k].order[i]].rgb;
			if(GetRValue(f)*GetGValue(f)/255/255*GetBValue(f)/255==1){
				brs=CreateSolidBrush(RGB(unit[k].team*100,unit[k].team*150,unit[k].team*200));
			}
			else
			brs=CreateSolidBrush(shape[unit[k].kind].layer[unit[k].order[i]].rgb);
	ob=(HBRUSH *)SelectObject(MemDC,brs);
	
				Polygon(MemDC,a,o.np);
				SelectObject(MemDC,ob);
	DeleteObject(ob);
	DeleteObject(brs);
			//	Sleep(1000);
			}

		}
	}

	//rot(Ax,Ay,Az,nowx,nowy,nowz);
	SetTextColor(MemDC,RGB(0,0,0));
	
	char str[256];
	sprintf(str,"Fe : %d",Fe[1]);
	TextOut(MemDC,700,110,str,strlen(str));
	SetTextColor(MemDC,RGB(0,0,0));
	HBITMAP *obit;
	obit=(HBITMAP *)SelectObject(bitDC,downbit);
	BitBlt(MemDC,100,520,800,150,bitDC,0,0,SRCCOPY);
	SelectObject(bitDC,obit);
	DeleteObject(obit);
	if(sbuild>0)
	{
		TextOut(MemDC,150,555,shape[unit[sbuild].kind].explain[unit[sbuild].temp3],strlen(shape[unit[sbuild].kind].explain[unit[sbuild].temp3]));
		sprintf(str,"%s 제작 진행중 : %d/%.0lf",shape[list[unit[sbuild].kind][unit[sbuild].temp2[1]]].name,unit[sbuild].temp,shape[list[unit[sbuild].kind][unit[sbuild].temp2[1]]].makeftime);
		TextOut(MemDC,150,580,str,strlen(str));
		//if(unit[sbuild].kind==3){
			TextOut(MemDC,715,547,shape[unit[sbuild].kind].but[1],strlen(shape[unit[sbuild].kind].but[1]));
			TextOut(MemDC,715,587,shape[unit[sbuild].kind].but[3],strlen(shape[unit[sbuild].kind].but[3]));
			TextOut(MemDC,715,627,shape[unit[sbuild].kind].but[5],strlen(shape[unit[sbuild].kind].but[5]));
			TextOut(MemDC,815,547,shape[unit[sbuild].kind].but[2],strlen(shape[unit[sbuild].kind].but[2]));
			TextOut(MemDC,815,587,shape[unit[sbuild].kind].but[4],strlen(shape[unit[sbuild].kind].but[4]));
			TextOut(MemDC,815,627,shape[unit[sbuild].kind].but[6],strlen(shape[unit[sbuild].kind].but[6]));
		
	}
	else if(ssunit>0)
	{
		TextOut(MemDC,150,555,shape[unit[ssunit].kind].explain[unit[ssunit].temp3],strlen(shape[unit[ssunit].kind].explain[unit[ssunit].temp3]));
		
			TextOut(MemDC,715,547,shape[unit[ssunit].kind].but[1],strlen(shape[unit[ssunit].kind].but[1]));
			TextOut(MemDC,715,587,shape[unit[ssunit].kind].but[3],strlen(shape[unit[ssunit].kind].but[3]));
			TextOut(MemDC,715,627,shape[unit[ssunit].kind].but[5],strlen(shape[unit[ssunit].kind].but[5]));
			TextOut(MemDC,815,547,shape[unit[ssunit].kind].but[2],strlen(shape[unit[ssunit].kind].but[2]));
			TextOut(MemDC,815,587,shape[unit[ssunit].kind].but[4],strlen(shape[unit[ssunit].kind].but[4]));
			TextOut(MemDC,815,627,shape[unit[ssunit].kind].but[6],strlen(shape[unit[ssunit].kind].but[6]));
		TextOut(MemDC,150,580,str,strlen(str));
		if(unit[ssunit].kind==3){
			TextOut(MemDC,715,547,"Basic",5);
		}
	}
BitBlt(hDC,0,0,800,600,MemDC,100,100,SRCCOPY);
	SelectObject(MemDC,old);
	ReleaseDC(hWnd,MemDC);
	ReleaseDC(hWnd,bitDC);
	ReleaseDC(hWnd,hDC);
	DeleteObject(bit);
	DeleteObject(old);
	DeleteObject(old2);
//	DeleteObject(old3);
	DeleteObject(MemDC);
	DeleteObject(bitDC);
//	if(DIRECT)
//		lpDDS1->ReleaseDC(hDC);
	DeleteObject(hDC);


}*/
GLvoid glPrint(const char *text)
{
	GLuint base;
	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	base=glGenLists(96);
	glListBase(base - 32);								// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}
void draw(HWND hWnd, GLenum mode)
{

	int i, j, k;
	ee+=1;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();
	if(mode==GL_SELECT)
	glInitNames();
	//glPushName(0);
    //glNormal3f(0,0,1);
	/*HDC eDC=GetDC(0);
	char str[256];
	sprintf(str,"%lf",ee);
	TextOut(eDC,0,0,str,strlen(str));
	ReleaseDC(0,eDC);
	DeleteDC(eDC);
*/
	glEnable(GL_LIGHTING);

	glRotatef(45,1,0,0);
glTranslatef(defx,-30,-defy);//0.0, 0.0, -5.0);
	
	mapctrl.init();
	//for(i=-1650 ; i<=1650 ; i+=10)
	//{
	//	for(j=-1650 ; j<=1650 ; j+=10){
	for(i=-defx*15-650 ; i<=-defx*15+650 ; i+=DET)
	{
		for(j=defy*15-1200 ; j<=defy*15-150 ; j+=DET){
		mapctrl.put(i,j);
		if(mode==GL_SELECT)
		glPushName(mapctrl.n);
			glBegin(GL_POLYGON);
			glPushMatrix();
			
	//	glLoadName(0);
			glColor3f(0.4,0.6,0.6);//rand()%10,rand()%10,rand()%10)
			glNormal3f((atan(GRAPH(i,j)-GRAPH(i-1,j))/PI),1,(atan(GRAPH(i,j)-GRAPH(i,j-1))/PI));
    glVertex3f(i/HH,-GRAPH(i,j)/HH,j/HH);
    glVertex3f(i/HH,-GRAPH(i,j+DET)/HH,(j+DET)/HH);
    glVertex3f((i+DET)/HH,-GRAPH(i+DET,j+DET)/HH,(j+DET)/HH);
    glVertex3f((i+DET)/HH,-GRAPH(i+DET,j)/HH,j/HH);
	 glPopMatrix();
			glEnd();
			if(mode==GL_SELECT)
			glPopName();
		}
	}
	
	int u;
	//0번폭탄 1번육군 2번공군 3번해군 4번건물 5번자원 6번건물짓는중 7번자원짓는중
	int iii[8]={0,4,6,1,5,7,0,2};
	//glNormal3f(0,1,0);
	glDisable(GL_LIGHTING);

	for(u=1 ; u<=7; u++)
	for(k=1 ; k<=1000 ; k++){

		if(unit[k].energy<=0 || unit[k].what!=iii[u])
			continue;
		int uuu;
		uuu=shape[unit[k].kind].nlayer;
		if(unit[k].what==6 || unit[k].what==7)
		{
			uuu=double(uuu)*(double(unit[k].making)/double(shape[unit[k].kind].makeftime));
		}
		if(mode==GL_SELECT)
		glPushName(k);
		glPushMatrix();
	//	if(mode==GL_SELECT)
	//	glLoadName(k);
		for(i=1 ; i<=uuu ; i++)
		{
			glPushMatrix();
	//	if(mode==GL_SELECT)
	//	glLoadName(0);
		
			//glTranslatef(1+deft.x,1,1+deft.z);	
			glTranslatef(unit[k].pt.x/HH,-unit[k].pt.y/HH,unit[k].pt.z/HH);//0.0, 0.0, -5.0);
			
			glRotatef(unit[k].ay,0,1,0);
			glRotatef(unit[k].ax,1,0,0);
			glRotatef(unit[k].az,0,0,1);
			/* 선택시 */
			if(sunit[k]){
				glBegin(GL_TRIANGLES);
				glColor3f((rand()%10)/10.0,(rand()%10)/10.0,(rand()%10)/10.0);
				glVertex3f(-2.0f, 5.0f, -2.0f);
				glColor3f((rand()%10)/10.0,(rand()%10)/10.0,(rand()%10)/10.0);
				glVertex3f(2.0f, 5.0f, -2.0f);
				glColor3f((rand()%10)/10.0,(rand()%10)/10.0,(rand()%10)/10.0);
				glVertex3f(0.0f, 5.0f, 2.0f);
				glEnd();
			}
			
			glBegin(GL_POLYGON);
	
			Layer o;
			o=shape[unit[k].kind].layer[i];
			for(j=1 ; j<=o.np ; j++)
			{
		glPushMatrix();
				double xx,yy,zz;
				xx=o.p[j].x;//+unit[k].pt.x;
				yy=o.p[j].y;//+unit[k].pt.y;
				zz=o.p[j].z;//+unit[k].pt.z;
				COLORREF col;
				col=shape[unit[k].kind].layer[i].rgb;
				if(unit[k].team==1 && RGB(255,255,255)==col)
				{
					col=RGB(0,0,255);
				}
				else if(unit[k].team==3 && RGB(255,255,255)==col)
				{
					col=RGB(0,255,0);
				}
				glColor3f(GetRValue(col)/256.0,GetGValue(col)/256.0,GetBValue(col)/256.0); //(rand()%10)/10.0,(rand()%10)/10.0,(rand()%10)/10.0);//rand()%10,rand()%10,rand()%10);
				
			glNormal3f(o.nor.x,o.nor.y,o.nor.z);
				glVertex3f(xx/HH,yy/HH,zz/HH);
		glPopMatrix();
			}
//			Polygon(MemDC,a,o.np);
			glEnd();
			
		glPopMatrix();
		}
		//glPopMatrix();
		if(mode==GL_SELECT)
		glPopName();
	}
	glPrint("haha");
	
}

	//if(rot>359) rot=1.0f;
/*
	return;
	HBITMAP bit, *old, *old2;
	HDC MemDC;
	HDC bitDC;

	bit=CreateCompatibleBitmap(hDC,1200,1200);
	MemDC=CreateCompatibleDC(hDC);
	bitDC=CreateCompatibleDC(hDC);
	//hDC=BeginPaint(hWnd,&ps);
	old = (HBITMAP *)SelectObject(MemDC,bit);

	Rectangle(MemDC,0,0,1024,768);

	SetBkMode(MemDC,0);

	//배경  출력
	//	old2=(HBITMAP *)SelectObject(bitDC,bgbit);
	//	BitBlt(MemDC,0,0,1024,768,bitDC,0,0,SRCCOPY);
	//	SelectObject(bitDC,old2);

	double xx, yy, zz;
	

	for(i=1 ; i<=99 ; i++)
	{
		for(j=1 ; j<=99 ; j++){
			if(mapp[i][j].x+deft.x>0 && mapp[i][j].y+deft.z>0 && mapp[i][j].x+deft.x<900 && mapp[i][j].y+deft.z<700){
	HPEN pen, *op;
	HBRUSH brs, *ob;
	pen=CreatePen(0,1,RGB(0,0,0));//RGB((map[i][j]+map[i][j+1]+map[i+1][j+1])*2+50,255,(map[i][j]+map[i][j+1]+map[i+1][j+1])*2+50));//(l-mmin)/(mmax-mmin)*255,(m-mmmin)/(mmmax-mmmin)*255,(l-mmin)/(mmax-mmin)*127+(m-mmmin)/(mmmax-mmmin)*127));//secondc);
	op=(HPEN *)SelectObject(MemDC,pen);
	brs=CreateSolidBrush(RGB(150,150,(map[i][j]+map[i][j+1]+map[i+1][j+1])+60));
	ob=(HBRUSH *)SelectObject(MemDC,brs);
			POINT a[5];
			a[0].x=mapp[i][j].x+deft.x;
			a[0].y=mapp[i][j].y+deft.z;
			a[1].x=mapp[i][j+1].x+deft.x;
			a[1].y=mapp[i][j+1].y+deft.z;
			a[2].x=mapp[i+1][j+1].x+deft.x;
			a[2].y=mapp[i+1][j+1].y+deft.z;
			a[3].x=mapp[i+1][j].x+deft.x;
			a[3].y=mapp[i+1][j].y+deft.z;
			Polygon(MemDC,a,4);
	SelectObject(MemDC,ob);
	SelectObject(MemDC,op);
	DeleteObject(op);
	DeleteObject(ob);
	DeleteObject(pen);
	DeleteObject(brs);
			}
		}
	}
	for(k=1 ; k<=1000 ; k++){
		if(unit[k].energy<=0)
			continue;
		point rr;
		rr=unit[k].calpt;
		point temp;
		temp=rr;
		if(sbuild==k)
			if(rr.x-30+deft.x>0 && rr.x-30+deft.x<900 && rr.y-30+deft.z>0 && rr.y-30+deft.z<700){
				HBRUSH brs, *ob;
				brs=(HBRUSH)GetStockObject(NULL_BRUSH);
				ob=(HBRUSH *)SelectObject(MemDC,brs);
		Ellipse(MemDC,rr.x-150+deft.x,rr.y-75+deft.z,rr.x+150+deft.x,rr.y+75+deft.z);
		SelectObject(MemDC,ob);
		DeleteObject(brs);
		DeleteObject(ob);
		

			}
		if(sunit[k])
			if(rr.x-30+deft.x>0 && rr.x-30+deft.x<900 && rr.y-30+deft.z>0 && rr.y-30+deft.z<700){
				HBRUSH brs, *ob;
				brs=(HBRUSH)GetStockObject(NULL_BRUSH);
				ob=(HBRUSH *)SelectObject(MemDC,brs);
		Ellipse(MemDC,rr.x-40+deft.x,rr.y-20+deft.z,rr.x+40+deft.x,rr.y+20+deft.z);
		SelectObject(MemDC,ob);
		DeleteObject(brs);
		DeleteObject(ob);
		

			}
		if(unit[k].what==1 || unit[k].what==3)
			Rectangle(MemDC,rr.x-unit[k].energy/10+deft.x,rr.y+15+deft.z,rr.x+unit[k].energy/10+deft.x,rr.y+20+deft.z);
		else if(unit[k].what!=0)
			Rectangle(MemDC,rr.x-unit[k].energy/10+deft.x,rr.y+50+deft.z,rr.x+unit[k].energy/10+deft.x,rr.y+55+deft.z);
		rr=unit[k].pt;
		rr.y=GRAPH(rr.x/SPLIT,rr.z/SPLIT);
		rot(Ax,Ay,Az,rr.x,rr.y,rr.z);
		HBRUSH brs, *ob;
		brs=(HBRUSH)GetStockObject(BLACK_BRUSH);
		ob=(HBRUSH *)SelectObject(MemDC,brs);
		SelectObject(MemDC,brs);
		Ellipse(MemDC,rr.x-6+deft.x,rr.y-4+deft.z,rr.x+6+deft.x,rr.y+4+deft.z);
		SelectObject(MemDC,ob);
		DeleteObject(brs);
		DeleteObject(ob);
		
	}
	int u;
	//0번폭탄 1번육군 2번공군 3번해군 4번건물 5번자원 6번건물짓는중 7번자원짓는중
	int iii[8]={0,4,6,1,5,7,0,2};
	for(u=1 ; u<=7 ; u++)
	for(k=1 ; k<=1000 ; k++){

		if(unit[k].energy<=0 || unit[k].what!=iii[u])
			continue;
		int uuu;
		uuu=shape[unit[k].kind].nlayer;
		if(unit[k].what==6 || unit[k].what==7)
		{
			uuu=double(uuu)*(double(unit[k].making)/double(shape[unit[k].kind].makeftime));
		}
		for(i=1 ; i<=uuu ; i++)
		{
			Layer o;
			o=shape[unit[k].kind].layer[unit[k].order[i]];
			POINT a[1000];
			for(j=1 ; j<=o.np ; j++)
			{
				xx=o.p[j].x;
				yy=o.p[j].y;
				zz=o.p[j].z;
				//rot(unit[k].ax-Ax,unit[k].ay-Ay,unit[k].az-Az,xx,yy,zz);
				if(unit[k].what>=1){
				rot(0,unit[k].ay,0,xx,yy,zz);
				rot(unit[k].ax,0,unit[k].az,xx,yy,zz);
				}
				if(unit[k].what==0){
				rot(unit[k].ax,0,unit[k].az,xx,yy,zz);
				rot(0,unit[k].ay,0,xx,yy,zz);
				}
				xx+=unit[k].pt.x;
				yy+=unit[k].pt.y;
				zz+=unit[k].pt.z;
				rot(Ax,Ay,Az,xx,yy,zz);
				a[j-1].x=xx+deft.x;
				a[j-1].y=yy+deft.z;
			}
			if(o.np>=3 && a[1].x>0 && a[1].y>0 && a[1].x<900 && a[1].y<700)
			{
			HBRUSH brs, *ob;
	
			COLORREF f;
			f=shape[unit[k].kind].layer[unit[k].order[i]].rgb;
			if(GetRValue(f)*GetGValue(f)/255/255*GetBValue(f)/255==1){
				brs=CreateSolidBrush(RGB(unit[k].team*100,unit[k].team*150,unit[k].team*200));
			}
			else
			brs=CreateSolidBrush(shape[unit[k].kind].layer[unit[k].order[i]].rgb);
	ob=(HBRUSH *)SelectObject(MemDC,brs);
	
				Polygon(MemDC,a,o.np);
				SelectObject(MemDC,ob);
	DeleteObject(ob);
	DeleteObject(brs);
			//	Sleep(1000);
			}

		}
	}

	//rot(Ax,Ay,Az,nowx,nowy,nowz);
	SetTextColor(MemDC,RGB(0,0,0));
	
	char str[256];
	sprintf(str,"Fe : %d",Fe[1]);
	TextOut(MemDC,700,110,str,strlen(str));
	SetTextColor(MemDC,RGB(0,0,0));
	HBITMAP *obit;
	obit=(HBITMAP *)SelectObject(bitDC,downbit);
	BitBlt(MemDC,100,520,800,150,bitDC,0,0,SRCCOPY);
	SelectObject(bitDC,obit);
	DeleteObject(obit);
	if(sbuild>0)
	{
		TextOut(MemDC,150,555,shape[unit[sbuild].kind].explain[unit[sbuild].temp3],strlen(shape[unit[sbuild].kind].explain[unit[sbuild].temp3]));
		sprintf(str,"%s 제작 진행중 : %d/%.0lf",shape[list[unit[sbuild].kind][unit[sbuild].temp2[1]]].name,unit[sbuild].temp,shape[list[unit[sbuild].kind][unit[sbuild].temp2[1]]].makeftime);
		TextOut(MemDC,150,580,str,strlen(str));
		//if(unit[sbuild].kind==3){
			TextOut(MemDC,715,547,shape[unit[sbuild].kind].but[1],strlen(shape[unit[sbuild].kind].but[1]));
			TextOut(MemDC,715,587,shape[unit[sbuild].kind].but[3],strlen(shape[unit[sbuild].kind].but[3]));
			TextOut(MemDC,715,627,shape[unit[sbuild].kind].but[5],strlen(shape[unit[sbuild].kind].but[5]));
			TextOut(MemDC,815,547,shape[unit[sbuild].kind].but[2],strlen(shape[unit[sbuild].kind].but[2]));
			TextOut(MemDC,815,587,shape[unit[sbuild].kind].but[4],strlen(shape[unit[sbuild].kind].but[4]));
			TextOut(MemDC,815,627,shape[unit[sbuild].kind].but[6],strlen(shape[unit[sbuild].kind].but[6]));
		
	}
	else if(ssunit>0)
	{
		TextOut(MemDC,150,555,shape[unit[ssunit].kind].explain[unit[ssunit].temp3],strlen(shape[unit[ssunit].kind].explain[unit[ssunit].temp3]));
		
			TextOut(MemDC,715,547,shape[unit[ssunit].kind].but[1],strlen(shape[unit[ssunit].kind].but[1]));
			TextOut(MemDC,715,587,shape[unit[ssunit].kind].but[3],strlen(shape[unit[ssunit].kind].but[3]));
			TextOut(MemDC,715,627,shape[unit[ssunit].kind].but[5],strlen(shape[unit[ssunit].kind].but[5]));
			TextOut(MemDC,815,547,shape[unit[ssunit].kind].but[2],strlen(shape[unit[ssunit].kind].but[2]));
			TextOut(MemDC,815,587,shape[unit[ssunit].kind].but[4],strlen(shape[unit[ssunit].kind].but[4]));
			TextOut(MemDC,815,627,shape[unit[ssunit].kind].but[6],strlen(shape[unit[ssunit].kind].but[6]));
		TextOut(MemDC,150,580,str,strlen(str));
		if(unit[ssunit].kind==3){
			TextOut(MemDC,715,547,"Basic",5);
		}
	}
BitBlt(hDC,0,0,800,600,MemDC,100,100,SRCCOPY);
	SelectObject(MemDC,old);
	ReleaseDC(hWnd,MemDC);
	ReleaseDC(hWnd,bitDC);
	ReleaseDC(hWnd,hDC);
	DeleteObject(bit);
	DeleteObject(old);
	DeleteObject(old2);
//	DeleteObject(old3);
	DeleteObject(MemDC);
	DeleteObject(bitDC);
	//if(DIRECT)
	//	lpDDS1->ReleaseDC(hDC);
//	DeleteObject(hDC);

*/
void rot(double ax, double ay, double az, double &x, double &y, double &z)
{
	double x1, x2, y1, y2, z1, z2;

	ax=torad(ax);
	ay=torad(ay);
	az=torad(az);
/*x축의 중심으로 */
	y1=y*cos(ax)-z*sin(ax);
	z1=y*sin(ax)+z*cos(ax);
/*y 축의 중심으로 */
	z2=z1*cos(ay)-x*sin(ay);
	x1=z1*sin(ay)+x*cos(ay);
/*z 축의 중심으로 */
	x2=x1*cos(az)-y1*sin(az);
	y2=x1*sin(az)+y1*cos(az);
	x=x2;
	y=y2;
	z=z2;
}

point makepoint(double xx, double yy, double zz)
{
	point a;
	a.x=xx;
	a.y=yy;
	a.z=zz;
	return a;
}

void init()
{
	/* Solget */
	shape[1].makebut("Center","Fe","Trabuild","","","Get [Fe]");
	shape[1].makeex("센터를 짓는다[Fe:1000]","자원 Fe캐는 기계를 설치한다.[Fe:2000]","Trabuild를 짓는다.[Fe:800]","","","자원 Fe를 캡니다.");
	shape[1].option("Solget",300,100,5,90);
	/* 폭탄 */
	shape[2].option("bomb",20,5,100,0);
	/* 센타 */
	shape[3].makebut("Solget","","","","","");
	shape[3].makeex("일꾼 Solget 을 제작합니다[Fe:90]","","","","","");
	shape[3].option("Center",1500,1500,0,1000);
	/* 자원 Fe */
	shape[4].option("Fe",1500,1500,0,2000);
	/* Twobox */
	shape[5].option("Twobox",300,50,4,150);
	/* Diatank */
	shape[6].option("Diatank",400,120,10,250);
	/* Forcetank */
	shape[7].option("Forcetank",600,200,5,600);
	/* trabuild */
	shape[8].makebut("Twobox","","","","","");
	shape[8].makeex("가장 기본공격유닛 Twobox 를 제작합니다[Fe:150]","","","","","");
	shape[8].option("Trabuild",1000,500,0,800);

	unit[1].createunit(4,1,3,000,0,000);
	unit[2].createunit(5,1,4,500,0,000);
	unit[3].createunit(1,1,1,200,0,200);
	unit[4].createunit(1,1,6,200,0,200);
	unit[5].createunit(1,1,7,200,0,200);
	
	int i,j;
	for(i=1 ; i<=100 ; i++)
		for(j=1 ; j<=100 ; j++){
			map[i][j]=GRAPH(i,j);
			mapp[i][j].x=i*SPLIT;
			mapp[i][j].y=map[i][j];
			mapp[i][j].z=j*SPLIT;
			//rot(Ax,Ay,Az,mapp[i][j].x,mapp[i][j].y,mapp[i][j].z);
		}
		fileopen("solget.txt",1,1.5);
		fileopen("basicbomb.txt",2,2);
		fileopen("center.txt",3,1);
		fileopen("fe.txt",4,1);
		fileopen("twobox.txt",5,2);
		fileopen("diatank.txt",6,1.25);
		fileopen("forcetank.txt",7,1.4);
		fileopen("trabuild.txt",8,1);
		for(int k=1 ; k<=100 ; k++)
		for(i=1 ; i<=shape[k].nlayer ; i++)
			for(j=1 ; j<=shape[k].layer[i].np ; j++)
				shape[k].layer[i].p[j].y*=-1;
		//deft.z=+700;
		//deft.x=-300;
defx=defy=0;

		/* direct 초기화 */
/*		HRESULT result;
		BOOL surface;
		m_ddrawok=false;
		m_pps=0;
		m_pbb=0;
		result=DirectDrawCreate(0,(LPDIRECTDRAW *)m_pddobj,IID_IDirectDraw);
*/}

double minus(double a)
{
	int kk=a/360;
	return a-double(kk)*360;
}

void autoattack()
{
	int i, j, k, mini;
	double dist, mindist;
	for(i=1 ; i<=1000 ; i++)
	{
		mindist=99999;
		mini=0;
		dist=0;
		if(unit[i].energy<=0 || unit[i].what!=1)
			continue;
		for(j=1 ; j<=1000 ; j++)
		{
			//i가 j 한테 공격
			if(unit[j].energy>0 && unit[i].team!=unit[j].team){

				dist=absol(unit[j].pt.x-unit[i].pt.x)+absol(unit[i].pt.z-unit[j].pt.z);
				if(mindist>dist){
					mindist=dist, mini=j;
				}
			}
		}
		if(mini==0)
			continue;
		if(unit[i].kind==5){
			if(mindist<600 && mindist>400)
			{
				if(unit[i].going==0 && unit[i].hard<=0){
					unit[i].ft.x+=(unit[mini].pt.x-unit[i].ft.x)*0.1;
					unit[i].ft.z+=(unit[mini].pt.z-unit[i].ft.z)*0.1;
				}
			}
			if(mindist<400)
			{
				if(unit[i].going==0)
				{
					unit[i].ft=unit[i].pt;
				}
				if(unit[i].delay>=unit[i].maxdelay)
				{
					for(k=1 ; k<=1000 ; k++)
					{
						if(unit[k].energy<=0)
						{
							unit[k].createunit(0,unit[i].team,2,unit[i].pt.x,unit[i].pt.y,unit[i].pt.z);
							unit[k].createbomb(unit[mini].pt.x,unit[mini].pt.z);
							break;
						}
					}
					unit[i].delay=0;
				}
			}
			
			if(mindist<200)
			{
				if(unit[i].going==0 && unit[i].hard<=0){
					unit[i].ft.x-=(unit[mini].pt.x-unit[i].ft.x)*0.1;
					unit[i].ft.z-=(unit[mini].pt.z-unit[i].ft.z)*0.1;
				}
			}
		}
	}
	
}


void Unit::createunit(int _what,int _team, int _kind, double x, double y, double z)
{
	ay=(rand()%1000)/100;
	hard=0;
	going=0;
	maxdelay=20;
	ft.x=-9999;
	ft.z=-9999;
	what=_what;
	team=_team;
	kind=_kind;
	pt.x=x;
	pt.y=y;
	pt.z=z;
	energy=shape[kind].energy;
	v=shape[kind].v;
}

void computer()
{
	int mape[101][101],mapp[101][101];
	int k, i, j, l, m;

	for(k=3 ; k<=3 ; k++)
	{
		for(i=0 ; i<=100 ; i++)
			for(j=0 ; j<=100 ; j++)
				mape[i][j]=mapp[i][j]=0;
		for(i=1 ; i<=1000 ; i++)
		{
			if(unit[i].team==k && unit[i].energy>0)
			{
				for(l=-10 ; l<=10 ; l++)
					for(m=-10 ; m<=10 ; m++)
					{
						int kx=unit[i].pt.x/SPLIT+l;
						int ky=unit[i].pt.z/SPLIT+m;
						if(kx<0 || ky<0 || kx>100 || ky>100)
							continue;
						mape[kx][ky]++;
					}
			}
			if(unit[i].team!=k && unit[i].energy>0)
			{
				for(l=-10 ; l<=10 ; l++)
					for(m=-10 ; m<=10 ; m++)
					{
						int kx=unit[i].pt.x/SPLIT+l;
						int ky=unit[i].pt.z/SPLIT+m;
						if(kx<0 || ky<0 || kx>100 || ky>100)
							continue;
						mapp[kx][ky]++;
					}
			}
		}
		int max=0, mmax=-1, wantx, wanty;
		for(i=0 ; i<=100 ; i++)
			for(j=0 ; j<=100 ; j++)
			{
				if(max<mape[i][j])
					max=mape[i][j];
			}
		for(i=0 ; i<=100 ; i++)
			for(j=0 ; j<=100 ; j++)
			{
				if(mmax<mapp[i][j])
					mmax=mapp[i][j], wantx=i,wanty=j;
			}
		if(max>0)
		{
			for(i=1 ; i<=1000 ; i++)
			{
				if(unit[i].team==k && unit[i].energy>0 && unit[i].hard<=0 && unit[i].what==1 && unit[i].kind==5)
				{
					double kx=unit[i].pt.x;
					double kz=unit[i].pt.z;
					if(kx>0 && kz>0 && kx<100 && kz<100)
					if(mape[int(kx)][int(kz)]>0){
						double ky=0;
						kx=(wantx+rand()%40-20);
						kz=(wanty+rand()%40-20);
						unit[i].ft.x=kx;
						unit[i].ft.z=kz;
						unit[i].hard=150;
					}
				}
				unit[i].hard--;
			}
		
		}
	}
}

void ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	xsize=width;
	ysize=height;
	ratio=(GLfloat)xsize/(GLfloat)ysize;
	glViewport(0,0,xsize,ysize);
	gluPerspective(45.0f,ratio,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

int InitGL(HWND hWnd)										// All Setup For OpenGL Goes Here
{
	//ReSizeGLScene(800,600);					// Set Up Our Perspective GL Screen

	
	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		16,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
/*
     memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
     pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
     pfd.nVersion = 1;
     pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;        
     pfd.iPixelType = PFD_TYPE_RGBA;
     pfd.cColorBits = 24;
     pfd.cDepthBits = 32;
     pfd.iLayerType = PFD_MAIN_PLANE; 
  */
	
	hDC=GetDC(hWnd);
    int nPixelFormat = ChoosePixelFormat(hDC, &pfd); 
    if (nPixelFormat == 0)
    {
		exit(1);
//        TRACE("ChoosePixelFormat Failed %d\r\n",GetLastError());
  }
//    TRACE("Pixel Format %d\r\n",nPixelFormat); 
 
    BOOL bResult = SetPixelFormat (hDC, nPixelFormat, &pfd); 
    if (!bResult)
    {
		exit(1);
  //      TRACE("SetPixelFormat Failed %d\r\n",GetLastError());
    }

	hRC = wglCreateContext(hDC); 
    if (!hRC)
    {
		exit(1);
//       TRACE("wglCreateContext Failed %x\r\n", GetLastError());
     } 
	
	if(!wglMakeCurrent(hDC,hRC)) // Try To Activate The Rendering Context
    {
        exit(1);
		return FALSE; // Return FALSE
    }



	//GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	
	//////////
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);
ReSizeGLScene(800,600);
/*******************************************************/
GLfloat ambientLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };     // <1>
	GLfloat diffuseLight[] = { 0.9f, 0.9f, 0.9f, 1.0f }; 
	GLfloat lightPos[] = { -100.0f, 130.0f, 150.0f, 1 };    // <3>
	
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };
/*  
	   GLfloat ambientLight[] = { 0.25f, 0.25f, 0.0f, 1.0f };
    GLfloat diffuseLight[] = { 0.9f, 0.9f, 0.0f, 1.0f };
    GLfloat lightPos[] = { -100.0f, 130.0f, 150.0f, 1.0f };
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };
*/

	
glShadeModel(GL_SMOOTH); // Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
    glClearDepth(1.0f); // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST); // Enables Depth Testing
    //glEnable(GL_CULL_FACE); // 뒷면에 대해서는 계산하지 말라
    glFrontFace(GL_CCW);   // 시계방향이 앞면이다.
    glEnable(GL_LIGHTING); // 빛을 사용한다.
   
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glEnable(GL_LIGHT0); // 0번빛을 사용한다.
   
    glEnable(GL_COLOR_MATERIAL);	
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
    glMateriali(GL_FRONT, GL_SHININESS, 10);
   
    glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really Nice Perspective    Calculations
	return TRUE;										// Initialization Went OK
}


void KillGLWindow(HWND hWnd)								// Properly Kill The Window
{
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(hWnd,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(hWnd,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(hWnd,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(hWnd,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(hWnd,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}
#define BUFSIZE 64

void lbut(HWND hWnd,GLint xx,GLint yy)
{
	int i;
	for(i=1 ; i<=1000 ; i++)
		sunit[i]=0;
	GLuint selectBuff[BUFSIZE];
	GLint hits, viewport[4];

	glSelectBuffer(BUFSIZE, selectBuff);
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glRenderMode(GL_SELECT);
		glLoadIdentity();
		gluPickMatrix(xx, viewport[3]-yy, 2, 2, viewport);
		gluPerspective(45.0f,ratio,0.1f,100.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		draw(hWnd,GL_SELECT);
		hits = glRenderMode(GL_RENDER);
			
		if(hits>0) 
		{
			for(i=1 ; i<=5 ; i++)
			{
				if(selectBuff[-1+4*i]>=1 && selectBuff[-1+4*i]<=999){
					sunit[selectBuff[-1+4*i]]=1-sunit[selectBuff[-1+4*i]];
					break;
				}
				
				if(selectBuff[-1+4*i]>=1000 && selectBuff[-1+4*i]<=9000)
				{
					smapx=mapctrl.where[selectBuff[-1+4*i]].x;
					smapy=mapctrl.where[selectBuff[-1+4*i]].z;
				}
			}
		}
		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);	
}
void slbut(HWND hWnd,GLint xx,GLint yy)
{
	int i, j;
//	for(i=1 ; i<=1000 ; i++)
//		sunit[i]=0;
	GLuint selectBuff[BUFSIZE];
	GLint hits, viewport[4];

	glSelectBuffer(BUFSIZE, selectBuff);
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glRenderMode(GL_SELECT);
		glLoadIdentity();
		gluPickMatrix(xx, viewport[3]-yy, 2, 2, viewport);
		gluPerspective(45.0f,ratio,0.1f,100.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		draw(hWnd,GL_SELECT);
		hits = glRenderMode(GL_RENDER);
			
		if(hits>0) 
		{
			for(i=1 ; i<=5 ; i++)
			{
				int fmapx;
				int fmapy;
				if(selectBuff[-1+4*i]>=1000 && selectBuff[-1+4*i]<=9000)
				{
					fmapx=mapctrl.where[selectBuff[-1+4*i]].x;
					fmapy=mapctrl.where[selectBuff[-1+4*i]].z;
					
					int temp;
					if(smapx>fmapx)
					{
						temp=smapx;
						smapx=fmapx;
						fmapx=temp;
					}
					if(smapy>fmapy)
					{
						temp=smapy;
						smapy=fmapy;
						fmapy=temp;	
					}
					//char str[256];
					//sprintf(str,"(%d %d)->(%d %d)",smapx,smapy,fmapx,fmapy);
					//MessageBox(0,str,0,0);
					for(j=1 ; j<=1000 ; j++)
					{
						if(smapx<unit[j].pt.x && unit[j].pt.x<fmapx)
							if(smapy<unit[j].pt.z && unit[j].pt.z<fmapy)
							{
								sunit[j]=1;
							}
					}
				}

			}
		}
		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);	
}
void rbut(HWND hWnd,GLint xx,GLint yy)
{
	int i, j;
	GLuint selectBuff[BUFSIZE];
	GLint hits, viewport[4];

	glSelectBuffer(BUFSIZE, selectBuff);
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glRenderMode(GL_SELECT);
		glLoadIdentity();
		gluPickMatrix(xx, viewport[3]-yy, 2, 2, viewport);
		gluPerspective(45.0f,ratio,0.1f,100.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		draw(hWnd,GL_SELECT);
		hits = glRenderMode(GL_RENDER);
			
		if(hits>0) 
		{
			for(i=1 ; i<=5 ; i++)
			{
				if(selectBuff[-1+4*i]>=1000 && selectBuff[-1+4*i]<=9000)
				{
					for(j=1 ; j<=1000 ; j++)
					{
						if(sunit[j])
						{
							unit[j].ft=mapctrl.where[selectBuff[-1+4*i]];
							unit[j].going=1;
						}
					}
				}
			}
		}
		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);	
}


void fileopen(char *filename,int number,double div)
{
	int i,j,k;
	double ffx, ffy, ffz, maxx, maxy,maxz, minx,miny,minz;
	double mmx,mmz,mix,miz;
	
		FILE *file;

		file=fopen(filename,"r");
		{
			fscanf(file,"%d",&shape[number].nlayer);
			for(i=1 ; i<=shape[number].nlayer ; i++)
			{
				mmx=mmz=mix=miz=0;
				maxx=maxy=maxz=-99999;
				minx=miny=minz=99999;
				int a,b,c;
					fscanf(file,";%d;%d;%d;",&a,&b,&c);
					shape[number].layer[i].rgb=RGB(a,b,c);
					
				fscanf(file,"\n%d\n",&shape[number].layer[i].np);
				for(j=1 ; j<=shape[number].layer[i].np ; j++){
					fscanf(file,"(%lf,%lf,%lf)\n",&shape[number].layer[i].p[j].x,&shape[number].layer[i].p[j].y,&shape[number].layer[i].p[j].z);
				shape[number].layer[i].p[j].x/=div;
				shape[number].layer[i].p[j].y/=div;
				shape[number].layer[i].p[j].z/=div;
				if(shape[number].layer[i].p[j].x<minx)
					minx=shape[number].layer[i].p[j].x, mix=shape[number].layer[i].p[j].y;
				if(shape[number].layer[i].p[j].z<minz)
					minz=shape[number].layer[i].p[j].z, miz=shape[number].layer[i].p[j].y;
				if(shape[number].layer[i].p[j].x>maxx)
					maxx=shape[number].layer[i].p[j].x, mmx=shape[number].layer[i].p[j].y;
				if(shape[number].layer[i].p[j].z>maxz)
					maxz=shape[number].layer[i].p[j].z, mmz=shape[number].layer[i].p[j].y;
				}
				ffx=atan((mmx-mix)/(maxx-minx+0.012))/PI;
				ffz=atan((mmz-miz)/(maxz-minz+0.012))/PI;
				shape[number].layer[i].nor.x=ffx;
				shape[number].layer[i].nor.y=1;
				shape[number].layer[i].nor.z=ffz;
				
			}
		}
		fclose(file);
}

