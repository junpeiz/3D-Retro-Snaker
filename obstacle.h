#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Const_def.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include <cstdlib>
#include <cmath>
#include <math.h>
#include <cstdio>
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Obstacle {
public:
	//indicate the position of this obstacle
	GLfloat position[3];
	int kind;
	Obstacle();
	void display();

	//if not collison,return 0 
	//if so return 1
	int  collsion_test_obstacle(GLfloat x, GLfloat y, GLfloat z);
};
void collision_handler(int setup);
void collision_handler_1(int setup);
void collision_handler_2(int setup);
void collision_handler_3(int setup);
void collision_handler_4(int setup);
void collision_handler_5(int setup);
void collision_handler_6(int setup);
void collision_handler_7(int setup);

void display_plus_1_second();
void display_simple_obstacle();
void display_reporter();
void display_apple();
void display_watch();
void display_glasses();
void display_wallace();
void display_zju();

#endif
