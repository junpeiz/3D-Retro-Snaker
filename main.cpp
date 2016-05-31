#include "draw_snake.h"
#include "obstacle.h"
#include <mmsystem.h>   //head file for import music

#pragma comment(lib,"winmm.lib")	//import music 

using namespace std;

bool Transparent=0;
bool
AutoRun = 1,
ChangingPlane = 0,
EnableKeyboard = 1;

int
Up[2] = { 0, 1 },
View[2] = { 1, 1 },
Left[2] = { 2, 1 },
OldUp[2] = { 0, 1 },
OldView[2] = { 1, 1 },
OldLeft[2] = { 2, 1 },
ChangingDire = 0,
Score = 0;

GLfloat
camera[3] = { StartViewX, -HeadCameDistance, 0.0 },
angleTurn = 0.0,
angleChangePlane = 0.0,
lightPosition[][4] = { { 0.0, -1.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0, 1.0 } },
wallSpecular[4] = { 0.3, 0.3, 0.3, 1.0 },
wallShininess = 50.0,     
CameraUp[3] = { 1.0, 0.0, 0.0 }; 

Obstacle obs[obs_num];
Snake TA;

void init();
void BackUpVectors();
void keyboardFunc(unsigned char key, int x, int y);
void display();
void processMousePassiveMotion(int x, int y);
void Update();
void drawWorld();
void setMatirial(const GLfloat mat_diffuse[4], GLfloat mat_shininess);
void ChangingPlaneFunc();
void ChangingDireFunc();
void PlayMusic();

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1200, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("3D greedy snake");

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboardFunc);
	glutIdleFunc(Update);
	//glutPassiveMotionFunc(processMousePassiveMotion);
	init();
	Draw_snake_init();
	glutMainLoop();
	return 0;
}

void Update()
{
	TA.update();

	if (AutoRun)
	{
		camera[View[0]] += TA.Speed*TA.Level*View[1];
		TA.head[View[0]] += TA.Speed*TA.Level*View[1];
	}

	if (Score > TA.Level * 100)
	{
		TA.Level++;
	}

	if (TA.head[View[0]] * View[1] >= StartHeadX)
	{
		ChangingPlane = 1;
		angleChangePlane = 30.0;
		AutoRun = 0;

		BackUpVectors();
		swap(View[0], Up[0]);
		swap(View[1], Up[1]);
		View[1] = -View[1];
	}

	if (ChangingPlane)
	{
		EnableKeyboard = 0;
		ChangingPlaneFunc();
	}

	if (ChangingDire)
	{
		ChangingDireFunc();
	}
	glutPostRedisplay();
}

void ChangingPlaneFunc()
{
	angleChangePlane += 0.1;

	camera[View[0]] = TA.head[View[0]] + RadiusOfCamera * OldUp[1] * sin(j2h(angleChangePlane)) / (0.44*cos(j2h(4 * angleChangePlane - 120)) + 0.56);
	camera[Up[0]] = TA.head[Up[0]] - RadiusOfCamera*OldView[1] * cos(j2h(angleChangePlane)) / (0.44*cos(j2h(4 * angleChangePlane - 120)) + 0.56);

	CameraUp[OldUp[0]] = cos(j2h(angleChangePlane - 30))*OldUp[1];
	CameraUp[OldView[0]] = sin(j2h(angleChangePlane - 30))*OldView[1];
	CameraUp[OldLeft[0]] = 0;
	if (angleChangePlane >= 120.0)
	{
		CameraUp[Up[0]] = Up[1];
		CameraUp[View[0]] = 0;
		CameraUp[Left[0]] = 0;
		camera[Up[0]] = NewCamPosUp*Up[1];
		camera[View[0]] = -NewCamPosView*View[1];
		ChangingPlane = 0;
		AutoRun = 1;
		EnableKeyboard = 1;
	}
}

void ChangingDireFunc()
{
	angleTurn += 0.3;

	camera[Left[0]] = TA.head[Left[0]] - HeadCameDistance * OldView[1] * cos(j2h(angleTurn))*(0.4*cos(j2h(4 * angleTurn)) + 0.6);
	camera[View[0]] = TA.head[View[0]] - HeadCameDistance * OldLeft[1] * sin(j2h(angleTurn))*(0.4*cos(j2h(4 * angleTurn)) + 0.6)*ChangingDire;

	if (angleTurn<75.1&&angleTurn>74.9)
	{
		TA.head[OldView[0]] = (int)TA.head[OldView[0]] + 0.5*OldView[1];
	}

	if (angleTurn >= 90.0)
	{
		camera[Left[0]] = TA.head[Left[0]];
		camera[View[0]] = TA.head[View[0]] - HeadCameDistance * OldLeft[1] * ChangingDire;
		EnableKeyboard = 1;
		AutoRun = 1;
		angleTurn = 0.0;
		ChangingDire = 0;
	}
}

void display()
{
	GLfloat headColor[4] = { 1.0, 0.494, 0.11, 1.0 };
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100.0f, 1.5f, 0.1f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera[0], camera[1], camera[2], TA.head[0], TA.head[1], TA.head[2], CameraUp[0], CameraUp[1], CameraUp[2]); //Ïà»úÎ»ÖÃ,ÖÐÐÄÎ»ÖÃ£¬ÉÏÏòÁ¿

	drawWorld();

	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, headColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, headColor);

	glEnable(GL_TEXTURE_2D);
	TA.display();
	glDisable(GL_TEXTURE_2D);

	for (int i = 0; i < obs_num; i++)
		obs[i].display();

	printf("%d\n", TA.collision_result);

	collison_handler(TA.collision_result);

	glutSwapBuffers();
}

void drawWorld()
{

	const static GLfloat a[][6] = { { 1.0f, 0.0f, 0.0f, 0.4f }, { 0.0f, 1.0f, 0.0f, 0.4f }, { 0.0f, 0.0f, 1.0f, 0.4f }, { 1.0f, 1.0f, 0.0f, 0.4f }, { 0.0f, 1.0f, 1.0f, 0.4f }, { 1.0f, 0.0f, 1.0f, 0.4f } };

	const static GLfloat blue_color[] = { 0.0f, 0.0f, 0.5f, 0.4f };
	if (Transparent)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		setMatirial(blue_color, 30.0);

	}
	glPushMatrix();
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPosition[1]);
	if (!Transparent)
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blue_color);
	glutSolidCube(LengthOfCube);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, 100);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, a[0]);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, -100);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, a[1]);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 100, 0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, a[2]);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, -100, 0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, a[3]);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(100, 0, 0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, a[4]);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-100, 0, 0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, a[5]);
	glutSolidCube(20);
	glPopMatrix();

	glDepthMask(GL_TRUE);

}

void keyboardFunc(unsigned char key, int x, int y)
{
	// use this function to accomplish the control input
	// to actuallt control the snake ta move
	// given the snake is always moving forward
	// use 'a' to turn left and use 'd' to turn right
	if (EnableKeyboard)
	{
		switch (key)
		{
		case'a':
			angleChangePlane = 0.0;
			AutoRun = 0;
			EnableKeyboard = 0;
			BackUpVectors();
			ChangingDire = 1;
			swap(View[0], Left[0]);
			swap(View[1], Left[1]);
			Left[1] = -1 * Left[1];
			break;
		case'd':
			angleChangePlane = 0.0;
			AutoRun = 0;
			BackUpVectors();
			EnableKeyboard = 0;
			swap(View[0], Left[0]);
			swap(View[1], Left[1]);
			View[1] = -1 * View[1];
			ChangingDire = -1;
			break;
		}
		glutPostRedisplay();
	}
}

void init()
{
	GLfloat backEmi[] = { 1.0, 0.9, 0.01, 0.9 };
	glClearColor(0.0, 0.0, 0.0, 1.0);//±³¾°ÑÕÉ«
	glClearDepth(1.0);				//´ÓÕâ¸ödepth¿ªÊ¼¿´²»¼û
	glMatrixMode(GL_MODELVIEW);  //ÉèÖÃµ±Ç°²Ù×÷µÄ¾ØÕóÎª¡°Ä£ÐÍÊÓÍ¼¾ØÕó¡±
	glLoadIdentity();            //ÔÚ½øÐÐ±ä»»Ç°°Ñµ±Ç°¾ØÕóÉèÖÃÎªµ¥Î»¾ØÕó
	glShadeModel(GL_SMOOTH);     //¹â»¬×ÅÉ«Ä£Ê½
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);         //ÆôÓÃ»·¾³¹â
	glEnable(GL_DEPTH_TEST);     //¸ù¾Ý×ø±êÔ¶½üÒþ²Ø±»ÕÚ×¡µÄÍ¼ÐÎ
	glLoadIdentity();//ÔÚ½øÐÐ±ä»»Ç°°Ñµ±Ç°¾ØÕóÉèÖÃÎªµ¥Î»¾ØÕó
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition[0]);
	glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, backEmi);
	glLightfv(GL_LIGHT0, GL_SPECULAR, backEmi);
	PlayMusic();
}

void calcLeft()
{
	Left[0] = 3 - View[0] - Up[0];
	int v1[3] = { 0, 0, 0 };
	int v2[3] = { 0, 0, 0 };
	v1[Up[0]] = Up[1];
	v2[View[0]] = View[1];
	Left[1] = v1[1] * v2[2] + v1[2] * v2[0] + v1[0] * v2[1] - v1[2] * v2[1] - v1[0] * v2[2] - v1[1] * v2[0];
}

void debugDisplay()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	int camera0[3] = { 0, 0, 0 }, view0[3] = { 0, 0, 0 };
	camera0[Up[0]] = 100 * Up[1];
	view0[View[0]] = View[1] * 1.0;
	gluLookAt(camera0[0], camera0[1], camera0[2], 0, 0, 0, view0[0], view0[1], view0[2]);
	glViewport(-80, 100, 400, 400);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, 1200, 800);
}

void setMatirial(const GLfloat mat_diffuse[4], GLfloat mat_shininess)
{
	static const GLfloat mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const GLfloat mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
}

void BackUpVectors()
{
	for (int i = 0; i < 2; i++)
	{
		OldUp[i] = Up[i];
		OldView[i] = View[i];
		OldLeft[i] = Left[i];
	}
}

void PlayMusic()
{
	char buf[128];
	char str[128] = { 0 };
	int i = 0;
	MCI_OPEN_PARMS mciOpen;
	MCIERROR mciError;
	mciOpen.lpstrDeviceType = "mpegvideo";
	mciOpen.lpstrElementName = "FortunateSon.mp3";
	mciError = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)&mciOpen);
	if (mciError) {
		mciGetErrorString(mciError, buf, 128);
		printf("send MCI_OPEN command failed:%s\n", buf);
		return;
	}
	UINT DeviceID = mciOpen.wDeviceID;
	MCI_PLAY_PARMS mciPlay;
	mciError = mciSendCommand(DeviceID, MCI_PLAY, 0, (DWORD)&mciPlay);
	if (mciError) {
		printf("send MCI_PLAY command failed\n");
		return;
	}
}
