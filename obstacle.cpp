#include "obstacle.h"
#include "draw_snake.h"
using namespace std;

int collision_state_1,collision_state_2,collision_state_3,collision_state_4,collision_state_5,collision_state_6;
static int collision_counter_1,collision_counter_2,collision_counter_3,collision_counter_4,collision_counter_5,collision_counter_6;
extern Snake TA;
extern bool Transparent;
int kind;
Obstacle::Obstacle()
{
	memset(position, 0, sizeof(position));
	int i = rand() % 6;
	switch (i)
	{
	case 0:
		position[0] = LengthOfCube / 2;
		position[1] = rand() % LengthOfCube - LengthOfCube / 2;
		position[2] = rand() % LengthOfCube - LengthOfCube / 2;
		break;
	case 1:
		position[0] = -LengthOfCube / 2;
		position[1] = rand() % LengthOfCube - LengthOfCube / 2;
		position[2] = rand() % LengthOfCube - LengthOfCube / 2;
		break;
	case 2:
		position[1] = LengthOfCube / 2;
		position[0] = rand() % LengthOfCube - LengthOfCube / 2;
		position[2] = rand() % LengthOfCube - LengthOfCube / 2;
		break;
	case 3:
		position[1] = -LengthOfCube / 2;
		position[0] = rand() % LengthOfCube - LengthOfCube / 2;
		position[2] = rand() % LengthOfCube - LengthOfCube / 2;
		break;
	case 4:
		position[2] = LengthOfCube / 2;
		position[1] = rand() % LengthOfCube - LengthOfCube / 2;
		position[0] = rand() % LengthOfCube - LengthOfCube / 2;
		break;
	case 5:
		position[2] = LengthOfCube / 2;
		position[1] = rand() % LengthOfCube - LengthOfCube / 2;
		position[0] = rand() % LengthOfCube - LengthOfCube / 2;
		break;
	}
	kind = rand()%6;
	printf("kind is %d\n", kind);
}

void Obstacle::display()
{
	glPushMatrix();
	glTranslated(position[0], position[1], position[2]);
	glRotated(180, 1, 0, 0);
	glRotated(-90, 0, 0, 1);
	switch (kind) {
		case 1 :	
			display_simple_obstacle();
			break;
		case 2 : 
			display_reporter();
			break;
		case 3 :
			display_apple();
			break;
		case 4 :
			display_wallace();
			break;
		case 5 :
			display_watch();
			break;
		case 6 :
			display_glasses();
			break;
		default:
			break;
	}

	glPopMatrix();
}

int  Obstacle::collsion_test_obstacle(GLfloat x, GLfloat y, GLfloat z)
{
	int distance = (x - position[0])*(x - position[0]) + (y - position[1])*(y - position[1]) + (z - position[2])*(z - position[2]);
	return (distance < COLLISION_DISTANCE) ? kind : 0;
}

void collison_handler(int setup)
{
	collison_handler_1(setup);
	collison_handler_2(setup);
	collison_handler_3(setup);
	collison_handler_4(setup);
	collison_handler_5(setup);
	collison_handler_6(setup);
}

//simplest kind of  coliison 
void collison_handler_1(int setup)
{
	if(setup==0&&collision_state_1==0)
		return;
	if(setup==1&&collision_state_1==0)
		collision_state_1=1;
	if (collision_state_1)
	{

		glPushMatrix();
		glTranslated(TA.head[0], TA.head[1], TA.head[2]);
		glRotated(180, 1, 0, 0);
		glRotated(-90, 0, 0, 1);

		if (collision_counter_1++ < 10)
			glutSolidTeapot(collision_counter_1 * 100);
		else
		{
			collision_counter_1 = 0;
			collision_state_1 = 0;
		}
		glPopMatrix();
	}

}

//吃一个香港记者
//	——加速移动
void collison_handler_2(int setup)
{
	if (setup==0&&collision_state_2==0) 
		return;
	if(setup==2&&collision_state_2==0)
		collision_state_2=1;
	
	if (collision_state_2) 
	{
		if(collision_counter_2++<10)
		{
			TA.Level++;	
		}
		else
		{
			TA.Level -= 10;
			collision_counter_2=0;
			collision_state_2=0;	
		}
	}

}
//	吃一个虵果
//		——   身长变短  +1s
void collison_handler_3(int setup)
{
	if(setup==0&&collision_state_3==0)
		return;
	if(setup==3&&collision_state_5==0)
		collision_state_5=1;
	if (collision_state_3) 
	{
		if(collision_counter_3++<10)
		{
			if(TA.num_body>=1)TA.num_body--;
			display_plus_1_second();
		}
		else
		{
			collision_counter_3=0;
			collision_state_3=0;	
		}
	}
}
//		碰到华莱士
//			——下次一遇到障碍物可以飞起来
void collison_handler_4(int setup)
{
	if(setup==0&&collision_state_4==0)
		return;
	if(setup==4&&collision_state_4==0)
		collision_state_4=1;
	if (collision_state_4) 
	{
		if(collision_state_1||collision_state_2||collision_state_3||collision_state_5||collision_state_6)
		{
			collision_counter_4 = 100;
		}
		if(collision_counter_4>0)
		{
			collision_counter_4--;
			for(int i = 0;i<TA.num_body;i++)
			{
	  			TA.body_position[i][0]+=sin(collision_counter_4/100*2*PI);
				TA.body_position[i][1]+=sin(collision_counter_4/100*2*PI);
				TA.body_position[i][2]+=sin(collision_counter_4/100*2*PI);
			}
		}
		else
		{
			collision_counter_4=0;
			collision_state_4=0;	
		}
	}


}

//			吃满三个手表
//				——积分翻倍
void collison_handler_5(int setup)
{			
	
	if (setup)
			TA.Level-=TA.Level/2;
	return;
}
//				吃一个黑框眼镜
//					——立体透视3秒
void collison_handler_6(int setup)
{
	//this seem to needs change the drawworld function
	if(setup==0&&collision_state_6==0)
		return;
	if(setup==1&&collision_state_6==0)
		collision_state_6=1;
	if (collision_state_6) 
	{
		if(collision_counter_6++<100)
		{
			// draw the world differently
			// in frame
			Transparent=1;
		}
		else
		{
			collision_counter_6=0;
			collision_state_6=0;	
			Transparent=0;
		}
	}
}

void display_plus_1_second()
{
	glutSolidCube(2.0);
}

void display_simple_obstacle()
{
	glutSolidCube(2.0);
}

void display_reporter()
{
	glColor3f(1.0,0,0);
	glutSolidCube(2.0);
}

void display_apple()
{
	glColor3f(0, 1.0, 0);
	glutSolidCube(2.0);
}

void display_wallace()
{
	glColor3f(0, 0, 1.0);
	glutSolidCube(2.0);
}

void display_watch()
{
	glColor3f(0.5, 0.5, 0.5);
	glutSolidCube(2.0);
}

void display_glasses()
{
	glColor3f(1.0, 1.0, 1.0);
	glutSolidCube(2.0);
}

		
