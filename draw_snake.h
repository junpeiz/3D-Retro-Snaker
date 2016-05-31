#ifndef DRAW_SNAKE_H
#define DRAW_SNAKE_H

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

void displayHead();
void displayBody();
void displayTail();

class Snake {
private:
	//a int indicated the length of snake's body
	int num_body;
	//use in body_num calcul,when reach a certain time
	int count_grow;
	//a count used in recoding the position of 
	int count;
	//to record the position of every body piece
	GLfloat body_position[max_body_num][3];

	void displayHead();
	void displayBody();
	void displayTail();
	void update_num();
	void update_position();
	void collision_test();
	int  collision_test_selfbody(int i);
	
public:
	GLfloat head[3];
	int collision_result;
	int Level = 1;
	double Speed = 0.02;

	Snake() {
		head[0] = StartHeadX;
		head[1] = 0.0;
		head[2] = 0.0;
		num_body = 1;
		memset(body_position, 0, sizeof(body_position));
	}

	void display() {
		displayHead();
		displayBody();
		displayTail();
	}

	void update() {
		update_num();
		update_position();
		collision_test();
	}
};

void Draw_snake_init();

void Draw_cube(GLfloat size);
void Draw_head_cube(GLfloat size);
void Draw_mouse_cube(GLfloat size);
void Draw_snake_head(GLfloat size);
void Draw_snake_body(GLfloat size);

AUX_RGBImageRec *LoadBMP(char * Filename);
int LoadGLTextures(GLuint& unTexture, const char* chFileName);

#endif
