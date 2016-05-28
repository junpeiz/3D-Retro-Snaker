#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Const_def.h"
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <GL\GLAUX.H>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Obstacle {
public:
	//indicate the position of this obstacle
	GLfloat position[3];
	Obstacle();
	void display();

	//if not collison,return 0 
	//if so return 1
	int  collsion_test_obstacle(GLfloat x, GLfloat y, GLfloat z);
};

void collison_handler_1();

#endif