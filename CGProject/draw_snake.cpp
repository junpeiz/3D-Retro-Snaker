#include "draw_snake.h"
#include "obstacle.h"
extern bool ChangingPlane;
GLuint Texture[num_of_pic];
GLuint Galaxy[num_of_galaxy];
extern Obstacle obs[obs_num];
extern int Up[2], Left[2], View[2], ChangingDire;
extern bool ChangingPlane;
extern GLfloat angleTurn, angleChangePlane;
GLfloat CurrentYAngle = 0;

void Rotated()
{
	int direc = 0;
	direc = Up[0];
	direc = (direc <<3) + Up[1] + 1;
	direc = (direc <<3) + View[0];
	direc = (direc <<3) + View[1] + 1;
	//printf("%o\n",direc);
	switch (direc)
	{
	case 00012:
		CurrentYAngle = 0;
		glRotatef(90, 0.0, 0.0, 1.0);
		break;
	case 00020:
		CurrentYAngle = 90;
		glRotatef(90, 0.0, 0.0, 1.0);
		break;
	case 00010:
		CurrentYAngle = 180;
		glRotatef(90, 0.0, 0.0, 1.0);
		break;
	case 00022:
		CurrentYAngle = 270;
		glRotatef(90, 0.0, 0.0, 1.0);
		break;
		 
	case 00210:
		CurrentYAngle = 0;
		glRotatef(-90, 0.0, 0.0, 1.0);
		break;
	case 00220:
		CurrentYAngle = 90;
		glRotatef(-90, 0.0, 0.0, 1.0);
		break;
	case 00212:
		CurrentYAngle = 180;
		glRotatef(-90, 0.0, 0.0, 1.0);
		break;
	case 00222:
		CurrentYAngle = 270;
		glRotatef(-90, 0.0, 0.0, 1.0);
		break;
		 
	case 01000:
		CurrentYAngle = 0;
		glRotatef(180, 0.0, 0.0, 1.0);
		break;
	case 01020:
		CurrentYAngle = 90;
		glRotatef(180, 0.0, 0.0, 1.0);
		break;
	case 01002:
		CurrentYAngle = 180;
		glRotatef(180, 0.0, 0.0, 1.0);
		break;
	case 01022:
		CurrentYAngle = 270;
		glRotatef(180, 0.0, 0.0, 1.0);
		break;

	case 01202:
		CurrentYAngle = 0;
		break;
	case 01220:
		CurrentYAngle = 90;
		break;
	case 01200:
		CurrentYAngle = 180;
		break;
	case 01222:
		CurrentYAngle = 270;
		break;

	case 02002:
		CurrentYAngle = 0;
		glRotatef(270, 1.0, 0.0, 0.0);
		break;
	case 02010:
		CurrentYAngle = 90;
		glRotatef(270, 1.0, 0.0, 0.0);
		break;
	case 02000:
		CurrentYAngle = 180;
		glRotatef(270, 1.0, 0.0, 0.0);
		break;
	case 02012:
		CurrentYAngle = 270;
		glRotatef(270, 1.0, 0.0, 0.0);
		break;
		 
	case 02202:
		CurrentYAngle = 0;
		glRotatef(90, 1.0, 0.0, 0.0);
		break;
	case 02212:
		CurrentYAngle = 90;
		glRotatef(90, 1.0, 0.0, 0.0);
		break;
	case 02200:
		CurrentYAngle = 180;
		glRotatef(90, 1.0, 0.0, 0.0);
		break;
	case 02210:
		CurrentYAngle = 270;
		glRotatef(90, 1.0, 0.0, 0.0);
		break;

	default:
		break;
	}
	if (ChangingDire)
		CurrentYAngle = CurrentYAngle + 90.0 +(- 90 + angleTurn)*ChangingDire;
	glRotatef(CurrentYAngle + 90.0, 0.0, 1.0, 0.0);
}


void Snake::displayHead()
{
	glPushMatrix();
	glTranslated(head[0], head[1], head[2]);

	Rotated();
	//glutSolidTeapot(2.0);
	Draw_snake_head(head_size);

	glPopMatrix();
}

void Snake::displayBody()
{
	for (int i = 0; i < num_body - 1; i++)
	{
		if (i >= max_body_num)i = 0;
		glPushMatrix();
		glTranslatef(body_position[i][0], body_position[i][1], body_position[i][2]);
		//glutSolidTeapot(0.5);
		Draw_snake_body(body_size);
		glPopMatrix();
	}
}

void Snake::displayTail()
{
	glPushMatrix();
	glTranslatef(body_position[num_body - 1][0], body_position[num_body - 1][1], body_position[num_body - 1][2]);
	//glutSolidTeapot(0.1);
	Draw_snake_body(tail_size);
	glPopMatrix();
}

void Snake::update_num()
{
	if( !ChangingPlane && !ChangingDire )
		count_grow++;
	if (count_grow >= 10 && num_body <= max_body_num)
	{
		num_body++;

		count_grow = 0;
	}

}

void Snake::update_position()
{
	if (!ChangingPlane && !ChangingDire)
		count++;
	if (count>10) {
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
	for (int i = 8; i < num_body; i++)
		collision_result += collision_test_selfbody(i);
}

int Snake::collision_test_selfbody(int i)
{

	int distance = (head[0] - body_position[i][0])*(head[0] - body_position[i][0]) + (head[1] - body_position[i][1])*(head[1] - body_position[i][1]) + (head[2] - body_position[i][2])*(head[2] - body_position[i][2]);
	return (distance < COLLISION_DISTANCE&& distance!=0) ? 1 : 0;
}


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

// ����λͼ(��������Ĵ���)��ת�������� 
int LoadGLTextures(GLuint& unTexture, const char* chFileName)
{
	int Status = FALSE; // Status ״ָ̬ʾ��  
	AUX_RGBImageRec *TextureImage;	//������ͼ���ݵ�ָ��
	char* file_name = (char*)chFileName;
	if (TextureImage = LoadBMP(file_name))	//������ͼ����
	{
		Status = 1;
		//����һ��������unTexture
		glGenTextures(1, &unTexture);
		//��������Ȼ��Ը�������������������
		glBindTexture(GL_TEXTURE_2D, unTexture);
		// �������� 
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage->sizeX, TextureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
		//�����˲�Ϊ�����˲�
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//�����˲�
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//�ͷ���Դ
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

	// ����ǰ�棬���￪ʼȷ���������꣬Ȼ����ȷ�����λ��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, size);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size, -size, size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, size);
	// ���ƺ���
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, -size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size, -size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size, -size);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, -size);
	// ����
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, size, size);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size, size, size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, -size);
	//����
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, -size, -size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(size, -size, -size);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, size);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, size);
	//����
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size, -size, -size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, -size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size, size);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, size);
	//����
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, -size);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size, size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size);

	glEnd();
}

void Draw_head_cube(GLfloat size)
{
	glBindTexture(GL_TEXTURE_2D, Texture[1]);
	glBegin(GL_QUADS);
	// ����ǰ�棬���￪ʼȷ���������꣬Ȼ����ȷ�����λ��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, size);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size, -size, size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, size);
	// ���ƺ���
	glEnd();
	glBindTexture(GL_TEXTURE_2D, Texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, -size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size, -size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size, -size);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, -size);
	// ����
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, size, size);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size, size, size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, -size);
	//����
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, -size, -size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(size, -size, -size);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, size);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, size);
	//����
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size, -size, -size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, -size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size, size);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, size);
	//����
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, -size);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size, size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size);

	glEnd();
}

void Draw_mouse_cube(GLfloat size)
{
	glBindTexture(GL_TEXTURE_2D, Texture[3]);
	glBegin(GL_QUADS);
	// ����ǰ�棬���￪ʼȷ���������꣬Ȼ����ȷ�����λ��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, size / 2.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size, -size, size / 2.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, size / 2.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, size / 2.0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, Texture[2]);
	glBegin(GL_QUADS);
	// ���ƺ���
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, -size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size, -size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size, -size);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, -size);
	// ����
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, size, size / 2.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size, size, size / 2.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, -size);
	//����
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, -size, -size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(size, -size, -size);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, size / 2.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, size / 2.0);
	//����
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size, -size, -size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, -size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size, size / 2.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, size / 2.0);
	//����
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, -size);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, size / 2.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size, size / 2.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size);

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
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//ʹ����������
	LoadGLTextures(Texture[0], "snake_head.bmp");
	LoadGLTextures(Texture[1], "snake_eye.bmp");
	LoadGLTextures(Texture[2], "snake_mouse.bmp");
	LoadGLTextures(Texture[3], "snake_teeth.bmp");
	LoadGLTextures(Texture[4], "ordinary_grass.bmp");

	LoadGLTextures(Galaxy[0], "galaxy1.bmp");
	LoadGLTextures(Galaxy[1], "galaxy2.bmp");
	LoadGLTextures(Galaxy[2], "galaxy3.bmp");
	LoadGLTextures(Galaxy[3], "galaxy4.bmp");
	LoadGLTextures(Galaxy[4], "galaxy5.bmp");
	LoadGLTextures(Galaxy[5], "galaxy6.bmp");
}
