#ifndef DATA_H
#define DATA_H
struct Point
{
	float x;
	float y;
	float z;
};
struct TriAngle
{
	Point vertex[3];
	Point NormDir;
};
#endif // DATA_H

//#include <stdafx.h>
#include <gl\glut.h>
#include <vector>
#include <iostream>
#include <fstream>
//#include <data.h>
#include <string>
using namespace std;

GLfloat yRot = 0;
GLfloat xRot = 0;
vector<TriAngle> TrangleVector;
void ReadStlModel()
{
	//ifstream infile("C:\\Users\\FrankFang\\Desktop\\stl.txt");
	ifstream infile("C://Users//10537//Desktop//9910.stl");
	if (!infile.is_open())
	{
		return;
	}
	string temp, modelname;

	char dump[256];
	int trinumber = 0;
	TriAngle tempTriAngle;
	infile >> temp;
	int test = temp.compare("solid");
	if (test != 0)
	{
		return;
	}

	infile.getline(dump, 25);
	infile >> temp;
	while (temp.compare("facet") == 0)
	{
		trinumber++;//三角形数目
		infile >> temp;//get rid of "normal "
		infile >> tempTriAngle.NormDir.x;
		infile >> tempTriAngle.NormDir.y;
		infile >> tempTriAngle.NormDir.z;

		infile.getline(dump, 256); infile.getline(dump, 256);//get rid of "outer loop"
		for (int i = 0; i<3; i++)
		{
			infile >> temp;
			infile >> tempTriAngle.vertex[i].x;
			infile >> tempTriAngle.vertex[i].y;
			infile >> tempTriAngle.vertex[i].z;
			//tempTriAngle.push_back(temppoint[i]);
		}
		TrangleVector.push_back(tempTriAngle);
		infile >> temp;
		infile >> temp;
		infile >> temp;
	}//while()
}
//用来画一个坐标轴的函数
void axis(double length)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glBegin(GL_LINES);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, 0.0, length);
	glEnd();
	//将当前操作点移到指定位置
	glTranslated(0.0, 0.0, length - 0.2);
	glColor3f(1.0, 0.0, 0.0);
	glutWireCone(0.04, 0.3, 8, 8);
	glPopMatrix();
}
void paint(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-200.0, 200.0, -200.0, 200.0, -100, 100);
	glPointSize(1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 1.6, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	//画坐标系
	axis(2);

	glPushMatrix();
	glRotated(90.0, 0, 1.0, 0);//绕y轴正方向旋转90度
	axis(2);
	glPopMatrix();

	glPushMatrix();
	glRotated(-90.0, 1.0, 0.0, 0.0);//绕x轴负方向旋转
	axis(2);
	glPopMatrix();

	glPushMatrix();//旋转除坐标轴之外的物体
	glRotated(yRot, 0.0, yRot, 0.0);
	glRotated(xRot, xRot, 0.0, 0.0);
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 1.0f);
	glTranslated(0.125, 0.125, 0.125);
	glutWireCube(0.25);
	glPopMatrix();

	///stl model
	glPushMatrix();//旋转除坐标轴之外的物体
	vector<TriAngle>::iterator it2;
	for (it2 = TrangleVector.begin(); it2 != TrangleVector.end(); it2++)
	{
		glBegin(GL_TRIANGLES);
		glNormal3f(it2->NormDir.x, it2->NormDir.y, it2->NormDir.z);
		glVertex3f(it2->vertex[0].x, it2->vertex[0].y, it2->vertex[0].z);
		glVertex3f(it2->vertex[1].x, it2->vertex[1].y, it2->vertex[1].z);
		glVertex3f(it2->vertex[2].x, it2->vertex[2].y, it2->vertex[2].z);
		glEnd();
	}
	glPopMatrix();
	///stl end

	glPopMatrix();//用来整体绕y轴旋转
	glutSwapBuffers();
}
void Init()
{
	glClearColor(0.8, 0.8, 0.8, 1.0);


	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 90.0 };
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);


	GLfloat lmodel_ambient[] = { 0.8, 0.8, 0.7, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1.3, 1.6, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}
void SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT)
		yRot -= 5.0f;

	if (key == GLUT_KEY_RIGHT)
		yRot += 5.0f;

	if (key == GLUT_KEY_DOWN)
		xRot -= 5.0f;

	if (key == GLUT_KEY_UP)
		xRot += 5.0f;

	if (key> 356.0f)
		yRot = 0.0f;

	if (key< -1.0f)
		yRot = 355.0f;
	glutPostRedisplay();
}
int main(int argv, char *argc[])
{
	ReadStlModel();

	glutInit(&argv, argc);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(400, 300);
	glutCreateWindow("9910");
	Init();
	glutDisplayFunc(paint);
	glutReshapeFunc(reshape);
	glutSpecialFunc(SpecialKeys);
	glutMainLoop();
}