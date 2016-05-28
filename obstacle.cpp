#include "obstacle.h"
#include "draw_snake.h"

using namespace std;

int collision_state_1;
static int collision_counter_1;
extern Snake TA;

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
}

void Obstacle::display()
{
	glPushMatrix();
	glTranslated(position[0], position[1], position[2]);
	glRotated(180, 1, 0, 0);
	glRotated(-90, 0, 0, 1);

	glutSolidCube(2.0);

	glPopMatrix();
}

int  Obstacle::collsion_test_obstacle(GLfloat x, GLfloat y, GLfloat z)
{
	int distance = (x - position[0])*(x - position[0]) + (y - position[1])*(y - position[1]) + (z - position[2])*(z - position[2]);
	return (distance < COLLISION_DISTANCE) ? 1 : 0;
}

void collison_handler_1()
{
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
	else
	{
		collision_state_1 = 1;
	}
}