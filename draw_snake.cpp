#include "draw_snake.h"
#include "obstacle.h"

//GLuint Texture[num_of_pic];
extern Obstacle obs[100];

void Snake::displayHead()
{
	glPushMatrix();
	glTranslated(head[0], head[1], head[2]);
	glRotated(180, 1, 0, 0);
	glRotated(-90, 0, 0, 1);

	glutSolidTeapot(2.0);
	//Draw_snake_head(head_size);

	glPopMatrix();
}

void Snake::displayBody()
{
	for (int i = 0; i < num_body - 1; i++)
	{
		if (i >= max_body_num)i = 0;
		glPushMatrix();
		glTranslatef(body_position[i][0], body_position[i][1], body_position[i][2]);
		glutSolidTeapot(0.5);
		//Draw_snake_body(body_size);
		glPopMatrix();
	}
}

void Snake::displayTail()
{
	glPushMatrix();
	glTranslatef(body_position[num_body - 1][0], body_position[num_body - 1][1], body_position[num_body - 1][2]);
	glutSolidTeapot(0.1);
	//Draw_snake_body(tail_size);
	glPopMatrix();
}

void Snake::update_num()
{
	count_grow++;
	if (count_grow >= 50 && num_body <= max_body_num)
	{
		num_body++;

		count_grow = 0;
	}

}

void Snake::update_position()
{
	count++;
	if (count>50) {
		for (int i = 0; i < num_body - 1; i++)
		{
			interval[num_body - 1 - i][0] = interval[num_body - 2 - i][0];
			interval[num_body - 1 - i][1] = interval[num_body - 2 - i][1];
			interval[num_body - 1 - i][2] = interval[num_body - 2 - i][2];
		}

		interval[0][0] = 2 * (body_position[0][0] - head[0]);
		interval[0][1] = 2 * (body_position[0][1] - head[1]);
		interval[0][2] = 2 * (body_position[0][2] - head[2]);

		body_position[0][0] = head[0];
		body_position[0][1] = head[1];
		body_position[0][2] = head[2];

		count = 0;
	}

	for (int i = 1; i < num_body - 1; i++)
	{
		body_position[i][0] = body_position[i - 1][0] + interval[i - 1][0];
		body_position[i][1] = body_position[i - 1][1] + interval[i - 1][1];
		body_position[i][2] = body_position[i - 1][2] + interval[i - 1][2];
	}
}

void Snake::collision_test()
{
	collision_result = 0;
	for (int i = 0; i < 100; i++)
		collision_result += obs[i].collsion_test_obstacle(head[0], head[1], head[2]);
	//for (int i = 8; i < num_body; i++)
		//collision_result += collision_test_selfbody(i);
}

int Snake::collision_test_selfbody(int i)
{

	int distance = (head[0] - body_position[i][0])*(head[0] - body_position[i][0]) + (head[1] - body_position[i][1])*(head[1] - body_position[i][1]) + (head[2] - body_position[i][2])*(head[2] - body_position[i][2]);
	return (distance < COLLISION_DISTANCE&& distance!=0) ? 1 : 0;
}

/*
AUX_RGBImageRec *LoadBMP(char * Filename)
{
	FILE *File = NULL;
	if (!Filename)
		return NULL;
	File = fopen(Filename, "r");
	if (File)
	{
		fclose(File);
		return auxDIBImageLoadA(Filename);
	}
	return NULL;
}

// 载入位图(调用上面的代码)并转换成纹理 
int LoadGLTextures(GLuint& unTexture, const char* chFileName)
{
	int Status = FALSE; // Status 状态指示器  
	AUX_RGBImageRec *TextureImage;	//保存贴图数据的指针
	char* file_name = (char*)chFileName;
	if (TextureImage = LoadBMP(file_name))	//载入贴图数据
	{
		Status = 1;
		//创建一个纹理，unTexture
		glGenTextures(1, &unTexture);
		//绑定纹理，然后对该纹理区添加纹理数据
		glBindTexture(GL_TEXTURE_2D, unTexture);
		// 生成纹理 
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage->sizeX, TextureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
		//设置滤波为线性滤波
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//线性滤波
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//释放资源
		if (TextureImage)
		{
			if (TextureImage->data)
			{
				free(TextureImage->data);
			}
			free(TextureImage);
		}
	}
	return Status;
}

void Draw_cube(GLfloat size)
{
	glBegin(GL_QUADS);

	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, 0.0, size);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size, 0.0, size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size * 2, size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size * 2, size);
	// 绘制后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, 0.0, -size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size * 2, -size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size * 2, -size);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(size, 0.0, -size);
	// 上面
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size * 2, -size);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, size * 2, size);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size, size * 2, size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size * 2, -size);
	//底面
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, 0.0, -size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(size, 0.0, -size);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(size, 0.0, size);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, 0.0, size);
	//右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size, 0.0, -size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size * 2, -size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size * 2, size);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(size, 0.0, size);
	//左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, 0.0, -size);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, 0.0, size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size * 2, size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size * 2, -size);

	glEnd();
}

void Draw_head_cube(GLfloat size)
{
	glBindTexture(GL_TEXTURE_2D, Texture[1]);
	glBegin(GL_QUADS);
	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, 0.0, size);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size, 0.0, size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size * 2, size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size * 2, size);
	// 绘制后面
	glEnd();
	glBindTexture(GL_TEXTURE_2D, Texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, 0.0, -size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size * 2, -size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size * 2, -size);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(size, 0.0, -size);
	// 上面
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size * 2, -size);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, size * 2, size);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size, size * 2, size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size * 2, -size);
	//底面
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, 0.0, -size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(size, 0.0, -size);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(size, 0.0, size);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, 0.0, size);
	//右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size, 0.0, -size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size * 2, -size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size * 2, size);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(size, 0.0, size);
	//左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, 0.0, -size);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, 0.0, size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size * 2, size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size * 2, -size);

	glEnd();
}

void Draw_mouse_cube(GLfloat size)
{
	glBindTexture(GL_TEXTURE_2D, Texture[3]);
	glBegin(GL_QUADS);
	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, 0.0, size / 2.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size, 0.0, size / 2.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size * 2, size / 2.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size * 2, size / 2.0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, Texture[2]);
	glBegin(GL_QUADS);
	// 绘制后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, 0.0, -size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size * 2, -size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size * 2, -size);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(size, 0.0, -size);
	// 上面
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size * 2, -size);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, size * 2, size / 2.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size, size * 2, size / 2.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size * 2, -size);
	//底面
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, 0.0, -size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(size, 0.0, -size);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(size, 0.0, size / 2.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, 0.0, size / 2.0);
	//右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size, 0.0, -size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size * 2, -size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size * 2, size / 2.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(size, 0.0, size / 2.0);
	//左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, 0.0, -size);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, 0.0, size / 2.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size * 2, size / 2.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size * 2, -size);

	glEnd();
}

void Draw_snake_body(GLfloat size)
{
	glBindTexture(GL_TEXTURE_2D, Texture[0]);
	glPushMatrix();
	//glutSolidCube(size);
	Draw_cube(size/2.0);     //the length of the cube is size
	glPopMatrix();
	glFlush();
}

void Draw_snake_head(GLfloat size)
{
	glPushMatrix();
	Draw_head_cube(size / 2.0);

	glPushMatrix();
	glTranslatef(0, 0, 3.0*size / 4.0);
	Draw_mouse_cube(size / 4.0);
	glPopMatrix();

	glPopMatrix();
	glFlush();
}

void Draw_snake_init()
{
	//glEnable(GL_TEXTURE_2D);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//使纹理不变形
	LoadGLTextures(Texture[0], "snake_head.bmp");
	LoadGLTextures(Texture[1], "snake_eye.bmp");
	LoadGLTextures(Texture[2], "snake_mouse.bmp");
	LoadGLTextures(Texture[3], "snake_teeth.bmp");
	LoadGLTextures(Texture[4], "ordinary_grass.bmp");
}
*/
