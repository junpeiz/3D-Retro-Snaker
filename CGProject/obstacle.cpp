#include "obstacle.h"
#include "draw_snake.h"
#include "Dependencies\loadobj\load_obj.h"
using namespace std;

static int collision_state_1,collision_state_2,collision_state_3,collision_state_4,collision_state_5,collision_state_6,collision_state_7;
static int collision_counter_1,collision_counter_2,collision_counter_3,collision_counter_4,collision_counter_5,collision_counter_6,collision_counter_7;
extern Snake TA;
extern bool Transparent;
extern Object lantern;

Obstacle::Obstacle()
{
	memset(position, 0, sizeof(position));
	int i = rand() % 6;
	switch (i)
	{
	case 0:
		position[0] = LengthOfCube / 2 ;
		position[1] = rand() % LengthOfCube - LengthOfCube / 2 ;
		position[2] = rand() % LengthOfCube - LengthOfCube / 2;
		break;
	case 1:
		position[0] = -LengthOfCube / 2 ;
		position[1] = rand() % LengthOfCube - LengthOfCube / 2 ;
		position[2] = rand() % LengthOfCube - LengthOfCube / 2 ;
		break;
	case 2:
		position[1] = LengthOfCube / 2 ;
		position[0] = rand() % LengthOfCube - LengthOfCube / 2 ;
		position[2] = rand() % LengthOfCube - LengthOfCube / 2 ;
		break;
	case 3:
		position[1] = -LengthOfCube / 2 ;
		position[0] = rand() % LengthOfCube - LengthOfCube / 2 ;
		position[2] = rand() % LengthOfCube - LengthOfCube / 2 ;
		break;
	case 4:
		position[2] = LengthOfCube / 2 ;
		position[1] = rand() % LengthOfCube - LengthOfCube / 2 ;
		position[0] = rand() % LengthOfCube - LengthOfCube / 2 ;
		break;
	case 5:
		position[2] = -LengthOfCube / 2;
		position[1] = rand() % LengthOfCube - LengthOfCube / 2;
		position[0] = rand() % LengthOfCube - LengthOfCube / 2;
		break;
	}
	this->kind = rand()%6;
	//this->kind = 6;
	
}

void Obstacle::display()
{
	glPushMatrix();
	glTranslated(position[0], position[1], position[2]);
	glRotated(180, 1, 0, 0);
	glRotated(-90, 0, 0, 1);
switch (this->kind) {
		case 0 :
			//nothing
			break;
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
		case 7 :
			display_zju();
		default:
			break;
	}

	glPopMatrix();
}

int  Obstacle::collsion_test_obstacle(GLfloat x, GLfloat y, GLfloat z)
{
	int distance = (x - position[0])*(x - position[0]) + (y - position[1])*(y - position[1]) + (z - position[2])*(z - position[2]);
	if (distance < COLLISION_DISTANCE&&this->kind!=0)
	{
		int i = this->kind;
		this->kind = 0;
		return i;
	}
	else
		return 0;
}

void collision_handler(int setup)
{
	collision_handler_1(setup);
	collision_handler_2(setup);
	collision_handler_3(setup);
	collision_handler_4(setup);
	collision_handler_5(setup);
	collision_handler_6(setup);
	collision_handler_7(setup);
}

//simplest kind of  coliison 
void collision_handler_1(int setup)
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

		if (collision_counter_1++ < 1000)
		{
			glutSolidTeapot(collision_counter_1 * 100);
			////printf("collison state now : at simple obstacle \n");
		}
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
void collision_handler_2(int setup)
{
	if (setup==0&&collision_state_2==0) 
		return;
	if(setup==2&&collision_state_2==0)
		collision_state_2=1;
	
	if (collision_state_2) 
	{
		if(collision_counter_2++<20)
		{
			TA.Level++;	
			////printf("collison state now : at reporter , accelerate speed \n");
		}
		else
		{
			TA.Level -= 20;
			collision_counter_2=0;
			collision_state_2=0;	
		}
	}

}
//	吃一个虵果
//		——   身长变短  +1s
void collision_handler_3(int setup)
{
	if(setup==0&&collision_state_3==0)
		return;
	if(setup==3&&collision_state_5==0)
		collision_state_3=1;
	if (collision_state_3) 
	{
		if(collision_counter_3++<10)
		{
			if(TA.num_body>=5)TA.num_body-=TA.num_body/2;
			display_plus_1_second();
			////printf("collison state now : cut off body \n");
		}
		else
		{
			collision_counter_3=0;
			collision_state_3=0;	
		}
	}
}
/*		
 *		道具4，碰撞效果4
 *		贪吃虵的head部分正面撞击道具时触发效果
 *		飞起来 （不知道高到哪里去了）
 *		问题：
 *			1.华莱士还不知道怎么画 绘画请置于void display_wallace()中 导入obj？
 *			2.飞翔的姿势
 */
 void collision_handler_4(int setup)
{
	if(setup==0&&collision_state_4==0)
		return;
	if (setup == 4 && collision_state_4 == 0)
	{
		collision_state_4 = 1;
		collision_counter_4 = 1000;
	}
	if (collision_state_4) 
	{
		//if(collision_state_1||collision_state_2||collision_state_3||collision_state_5||collision_state_6)
		//{
			
		//}

		if(collision_counter_4>0)
		{
			collision_counter_4--;
			////printf("collison state now : at wallace ,flying now counter%d\n",collision_counter_4);
				TA.head[0] -= 0.05*TA.head[0] / abs(TA.head[0])*sin(collision_counter_4*1.0 / 1000 * 2 * PI);
				TA.head[1] -= 0.05*TA.head[0] / abs(TA.head[0])*sin(collision_counter_4*1.0 / 1000 * 2 * PI);
				TA.head[2] -= 0.05*TA.head[0] / abs(TA.head[0])*sin(collision_counter_4*1.0 / 1000 * 2 * PI);
	  			TA.body_position[0][0]-=0.05*TA.head[0] / abs(TA.head[0])*sin(collision_counter_4*1.0/1000*2*PI);
				TA.body_position[0][1]-=0.05*TA.head[0] / abs(TA.head[0])*sin(collision_counter_4*1.0/1000*2*PI);
				TA.body_position[0][2]-=0.05*TA.head[0] / abs(TA.head[0])*sin(collision_counter_4*1.0/1000*2*PI);
				////printf("position changed !\n");			
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
void collision_handler_5(int setup)
{			
	if (setup == 0 && collision_state_5 == 0)
		return;
	if (setup == 5 && collision_state_5 == 0)
		collision_state_5 = 1;
	if (collision_state_5)
	{
		if (collision_counter_5++<100)
		{
			return;
		}
		else
		{
			collision_counter_5 = 0;
			collision_state_5 = 0;
			TA.Level++;
		}
	}
	
}
//				吃一个黑框眼镜
//					——立体透视3秒
void collision_handler_6(int setup)
{
	//this seem to needs change the drawworld function
	if(setup==0&&collision_state_6==0)
		return;
	if(setup==6&&collision_state_6==0)
		collision_state_6=1;
	if (collision_state_6) 
	{
		if(collision_counter_6++<200)
		{
			// draw the world differently
			// in frame
			Transparent=1;
			////printf("collison state now : tranparent now \n");
		}
		else
		{
			collision_counter_6=0;
			collision_state_6=0;	
			Transparent=0;
		}
	}
}

//		碰到浙江大学
//			钻到了地底下
void collision_handler_7(int setup)
{
	if (setup == 0 && collision_state_7 == 0)
		return;
	if (setup == 7 && collision_state_7 == 0)
	{
		collision_state_7 = 1;
		collision_counter_7 = 1000;
	}
	if (collision_state_7)
	{
		//if(collision_state_1||collision_state_2||collision_state_3||collision_state_5||collision_state_6)
		//{

		//}

		if (collision_counter_7>0)
		{
			collision_counter_7--;
			////printf("collison state now : at wallace ,flying now counter%d\n", collision_counter_4);
			TA.head[0] += 0.05*TA.head[0] / abs(TA.head[0])*sin(collision_counter_7*1.0 / 1000 * 2 * PI);
			TA.head[1] += 0.05*TA.head[0] / abs(TA.head[0])*sin(collision_counter_7*1.0 / 1000 * 2 * PI);
			TA.head[2] += 0.05*TA.head[0] / abs(TA.head[0])*sin(collision_counter_7*1.0 / 1000 * 2 * PI);
			TA.body_position[0][0] += 0.05*TA.head[0] / abs(TA.head[0])*sin(collision_counter_7*1.0 / 1000 * 2 * PI);
			TA.body_position[0][1] += 0.05*TA.head[0] / abs(TA.head[0])*sin(collision_counter_7*1.0 / 1000 * 2 * PI);
			TA.body_position[0][2] += 0.05*TA.head[0] / abs(TA.head[0])*sin(collision_counter_7*1.0 / 1000 * 2 * PI);
			////printf("position changed !\n");
		}
		else
		{
			collision_counter_7 = 0;
			collision_state_7 = 0;
		}
	}


}

void display_plus_1_second()
{
	//glutSolidCone(2.0, 2.0, 50, 50);
	glutSolidCube(2.0);
	//lantern.draw();
}

void display_simple_obstacle()
{
	//glutSolidCube(2.0);
	glutSolidCylinder(1.0, 2.0, 50, 50);
	//lantern.draw();
}

void display_reporter()
{
	glColor3f(1.0,0,0);
	//glutSolidCube(2.0);
	glutSolidDodecahedron();
	//lantern.draw();
}

void display_apple()
{
	glColor3f(0, 1.0, 0);
	//glutSolidCube(2.0);
	glutSolidIcosahedron();
	//lantern.draw();
}

void display_wallace()
{
	glColor3f(0, 0, 1.0);
	//glutSolidCube(2.0);
	glutSolidDodecahedron();
}

void display_watch()
{
	glColor3f(0.5, 0.5, 0.5);
	//glutSolidCube(2.0);
	glutSolidRhombicDodecahedron();
}

void display_glasses()
{
	glColor3f(1.0, 1.0, 1.0);
	//glutSolidCube(2.0);
	glutSolidDodecahedron();
}

void display_zju()
{
	glColor3f(1.0, 1.0, 1.0);
	glutSolidCube(2.0);
	//glutSolidTeapot(2.0);
	//lantern.draw();
}

		
