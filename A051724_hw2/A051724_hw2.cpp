#include <glut.h>
#include <math.h>
#include <cstdlib>
#include <iostream>
#include <windows.h>

const GLfloat Pi = 3.1415926535f;
int mode = 0;
double pos[3] = { 0,0,20 };
double move = 0, angle = 0, lookaty, rotate = 0;
GLUquadricObj *quadratic;

void init(void) 
{
	GLfloat position[] = { 1.0, 1.0, 5.0, 0.0 };
	GLfloat ambient[] = { 0,0,1,1 };
	GLfloat diffuse[] = { 1,1,1,0 };
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
}

void draw_cube(bool in_mirror)
{
	//绘制立方体
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_ALWAYS);
	if (!in_mirror) 
	{
		glTranslated(0, 2, -1);
		glRotated(rotate / 1, -1, 1, -1);
	}
	else 
	{
		glTranslated(0, 2, 1);
		glRotated(180, 0, 1, 0);
		glRotated(-rotate / 1, -1, 1, -1);
	}
	glBegin(GL_QUADS);

	glColor3f(1.0, 1.0, 0.0);
	glVertex3f(1.0, 1.0, -1.0);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-1.0, 1.0, -1.0);
	glColor3f(0.0, 1.0, 1.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);

	glColor3f(1.0, 0.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(1.0, -1.0, -1.0);

	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glColor3f(0.0, 1.0, 1.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glColor3f(1.0, 0.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(1.0, -1.0, -1.0);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-1.0, 1.0, -1.0);
	glColor3f(1.0, 1.0, 0.0);
	glVertex3f(1.0, 1.0, -1.0);

	glColor3f(0.0, 1.0, 1.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-1.0, 1.0, -1.0);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(-1.0, -1.0, 1.0);

	glColor3f(1.0, 1.0, 0.0);
	glVertex3f(1.0, 1.0, -1.0);
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glColor3f(1.0, 0.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(1.0, -1.0, -1.0);

	glEnd();
	glPopMatrix();
	glDepthMask(GL_TRUE);
	glEnable(GL_LIGHTING);
}

void draw_apple(bool in_mirror)
{
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glPushMatrix();
	if (mode == 0)
	{
		glTranslated(2, 0, 0);
		if (!in_mirror) 
		{
			glRotated(rotate / 5, 0, 1, 0);
		}
		else 
		{
			glRotated(180, 0, 1, 0);
			glRotated(rotate / 5, 0, -1, 0);
		}
	}
	else 
	{
		glDisable(GL_BLEND);
		glBlendFunc(1, 0);
		glTranslated(2 - move, 0, 0);
		if (in_mirror) 
			glRotated(180, 0, 1, 0);
	}
	glutSolidSphere(1, 64, 64);
	glTranslated(0, 1.25, -0.5);
	glRotated(30, 1, 0, 0);
	glutSolidCone(0.0625, 0.5, 64, 64);
	glRotated(30, -1, 0, 0);
	glTranslated(0, -1.25, 0.5);
	glRotated(270, 1, 0, 0);
	glTranslated(0, 0, 1);
	glutSolidCone(0.0625, 0.5, 64, 64);
	
	glPopMatrix();
	if (mode >= 1) 
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
}

void draw_pen(bool in_mirror)
{
	quadratic = gluNewQuadric();
	gluQuadricNormals(quadratic, GLU_SMOOTH);
	gluQuadricTexture(quadratic, GL_TRUE);
	glPushMatrix(); 
	if (mode == 1)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	}
	if (mode == 0) 
	{
		glTranslated(-2, 0, 0);
		if (!in_mirror) 
		{
			glRotated(rotate / 10, 0, 1, 0);
			glRotated(285, 1, 0, 0);
		}
		else 
		{
			glRotated(180, 0, 1, 0);
			glRotated(rotate / 10, 0, -1, 0);
			glRotated(285, 1, 0, 0);
		}
	}
	else 
	{
		glDisable(GL_BLEND);
		glBlendFunc(1, 0);
		glTranslated(-2.125, 0, 0);
		glRotated(90, 0, 1, 0);
		glTranslated(0, 0, move);
	}
	gluCylinder(quadratic, 0.125, 0.125, 1, 32, 32);
	glRotated(180, 1, 0, 0);
	gluCylinder(quadratic, 0.125, 0.125, 1, 32, 32);
	glRotated(180, 1, 0, 0);
	glTranslated(0, 0, 1);
	glutSolidCone(0.125, 0.5, 64, 64);
	glPopMatrix();
	if (mode >= 1)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
}

void code_1() 
{
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glEnable(GL_DEPTH_TEST);	
	glDepthMask(GL_FALSE);				
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);	
	glStencilFunc(GL_ALWAYS, 0, 255);	
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);

	glPushMatrix();
	glBegin(GL_QUADS);
	
	glVertex3d(-3, -3, 15); 
	glVertex3d(-3, -3, -5);
	glVertex3d(-3, 3, -5); 
	glVertex3d(-3, 3, 15);
	
	glVertex3d(3, -3, 15);
	glVertex3d(3, 3, 15);  
	glVertex3d(3, 3, -5); 
	glVertex3d(3, -3, -5); 

	glVertex3d(-3, 3, 15); 
	glVertex3d(-3, 3, -5); 
	glVertex3d(3, 3, -5); 
	glVertex3d(3, 3, 15);

	glVertex3d(-3, -3, 15);
	glVertex3d(3, -3, 15); 
	glVertex3d(3, -3, -5); 
	glVertex3d(-3, -3, -5);

	glVertex3d(-3, -3, -5); 
	glVertex3d(0, -3, -2); 
	glVertex3d(0, 3, -2);
	glVertex3d(-3, 3, -5);

	glVertex3d(3, -3, -5); 
	glVertex3d(3, 3, -5); 
	glVertex3d(0, 3, -2); 
	glVertex3d(0, -3, -2);

	glEnd();
	glPopMatrix();
}

void code_2() 
{
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LESS);
	
	draw_cube(false);
	glStencilFunc(GL_EQUAL, 1, 255); 
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glDepthMask(GL_TRUE); 
	glDepthFunc(GL_LESS);
	glPushMatrix();
	glTranslated(-6, 0, 0);	
	glRotated(180, 0, 1, 0);
	draw_pen(true); 
	draw_apple(true);
	draw_cube(true);
	glPopMatrix();
	//Right Mirror
	glPushMatrix();
	glTranslated(6, 0, 0); 
	glRotated(180, 0, 1, 0);
	draw_pen(true);
	draw_apple(true);
	draw_cube(true);
	glPopMatrix();
	//Up Mirror
	glPushMatrix();
	glTranslated(0, 6, 0); 
	glRotated(180, 1, 0, 0);
	draw_pen(true); 
	draw_apple(true); 
	draw_cube(true);
	glPopMatrix();
	//Down Mirror
	glPushMatrix();
	glTranslated(0, -6, 0);
	glRotated(180, 1, 0, 0);
	draw_pen(true); 
	draw_apple(true); 
	draw_cube(true);
	glPopMatrix();
}

void code_3() 
{
	glStencilFunc(GL_ALWAYS, 1, 1); 
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glDepthMask(GL_TRUE); glDepthFunc(GL_LESS);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glPushMatrix();
	
	glBegin(GL_QUADS);
	//左侧镜子
	glColor4d(0, 1, 0, 0.5); 
	glVertex3d(-3, -3, 15); 
	glVertex3d(-3, -3, -5); 
	glVertex3d(-3, 3, -5);
	glVertex3d(-3, 3, 15);
	//右侧镜子
	glColor4d(0, 0, 1, 0.5); 
	glVertex3d(3, -3, 15);
	glVertex3d(3, 3, 15); 
	glVertex3d(3, 3, -5); 
	glVertex3d(3, -3, -5); 
	//上方镜子
	glColor4d(1, 0, 0, 0.5); 
	glVertex3d(-3, 3, 15); 
	glVertex3d(-3, 3, -5); 
	glVertex3d(3, 3, -5); 
	glVertex3d(3, 3, 15);
	//下方镜子
	glColor4d(0, 1, 1, 0.5);
	glVertex3d(-3, -3, 15);
	glVertex3d(3, -3, 15); 
	glVertex3d(3, -3, -5); 
	glVertex3d(-3, -3, -5);
	//左前方镜子
	glColor4d(1, 1, 0, 0.5); 
	glVertex3d(-3, -3, -5); 
	glVertex3d(0, -3, -2); 
	glVertex3d(0, 3, -2); 
	glVertex3d(-3, 3, -5);
	//右前方镜子
	glColor4d(1, 0, 1, 0.5); 
	glVertex3d(3, -3, -5); 
	glVertex3d(3, 3, -5); 
	glVertex3d(0, 3, -2); 
	glVertex3d(0, -3, -2);
	
	glEnd();
	glPopMatrix();
	glDisable(GL_LIGHTING);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glPushMatrix();
	glBegin(GL_QUADS);
	glColor4d(0, 1, 0, 0.5); 
	glVertex3d(-3, -3, 15); 
	glVertex3d(-3, -3, -5); 
	glVertex3d(-3, 3, -5); 
	glVertex3d(-3, 3, 15);
	
	glColor4d(0, 0, 1, 0.5); 
	glVertex3d(3, -3, 15);
	glVertex3d(3, 3, 15);  
	glVertex3d(3, 3, -5); 
	glVertex3d(3, -3, -5); 

	glColor4d(1, 0, 0, 0.5); 
	glVertex3d(-3, 3, 15); 
	glVertex3d(-3, 3, -5); 
	glVertex3d(3, 3, -5); 
	glVertex3d(3, 3, 15);

	glColor4d(0, 1, 1, 0.5);
	glVertex3d(-3, -3, 15); 
	glVertex3d(3, -3, 15); 
	glVertex3d(3, -3, -5); 
	glVertex3d(-3, -3, -5);

	glColor4d(1, 1, 0, 0.5);
	glVertex3d(-3, -3, -5); 
	glVertex3d(0, -3, -2); 
	glVertex3d(0, 3, -2); 
	glVertex3d(-3, 3, -5);

	glColor4d(1, 0, 1, 0.5);
	glVertex3d(3, -3, -5);
	glVertex3d(3, 3, -5); 
	glVertex3d(0, 3, -2); 
	glVertex3d(0, -3, -2);
	glEnd();
	glPopMatrix();
	glEnable(GL_LIGHTING);
	draw_apple(false); 
	draw_pen(false);
}

void code_4() 
{
	glStencilFunc(GL_EQUAL, 3, 255); 
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glClearDepth(1.0); 
	glClear(GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	
	glPushMatrix();	
	glTranslated(2, 0, -2); 
	glRotated(270, 0, 1, 0);
	draw_apple(true); 
	draw_pen(true);
	draw_cube(true);
	glPopMatrix();

	glPushMatrix(); 
	glTranslated(-2, 0, -2); 
	glRotated(90, 0, 1, 0);
	draw_pen(true); 
	draw_apple(true);
	draw_cube(true);
	glPopMatrix();
}

void myDisplay(void)
{
	glClearStencil(0);
	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(pos[0], pos[1], pos[2], /* Eye pos XYZ */
			  pos[0] + sin(angle), pos[1] + lookaty, pos[2] - cos(angle), /* Target pos XYZ */
			  0.0, 1.0, 0.0); /* Up vector */
	code_1();
	code_2();
	code_3();
	code_4();
	glutSwapBuffers();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, (GLfloat)width / (GLfloat)height, 0.1, 100.0);
}

void keyboard(unsigned char key, int x, int y) 
{
	switch (key) 
	{
	case 13: //Enter
	{
		if (mode == 0) 
		{ 
		mode = 1;
		move = 0;
		} 
		else 
		{
			mode = 0; 
			move = 0.01;
		}
		break;
	}
	case 27: //ESC
	{
		exit(0);
	}
	//These are the 'wsadqe' buttons
	case 119: //w
	{ 
		pos[0] += 0.2*sin(angle); 
		pos[2] -= 0.2*cos(angle); 
		break; 
	}
	case 115: //s
	{ 
		pos[0] -= 0.2*sin(angle); 
		pos[2] += 0.2*cos(angle); 
		break;
	}
	case 97: //a
	{
		pos[0] -= 0.2*cos(angle); 
		pos[2] -= 0.2*sin(angle);
		break;
	}
	case 100: //d
	{ 
		pos[0] += 0.2*cos(angle);
		pos[2] += 0.2*sin(angle); 
		break;
	}
	case 113: //q
	{
		angle -= 0.1;
		break;
	}
	case 101: //e
	{	
		angle += 0.1; 
		break; 
	}
	case 105: //i
	{ 
		lookaty += 0.05;
		break; 
	}
	case 107: //k
	{
		lookaty -= 0.05; 
		break; 
	}
	case 120: //x
	{ 
		pos[1] += 0.2;
		break; 
	}
	case 122: //z
	{
		pos[1] -= 0.2;
		break;
	}
	case 114: //r
	{ 
		angle = 0; 
		pos[0] = 0; 
		pos[1] = 0; 
		pos[2] = 20; 
		lookaty = 0; 
		break;
	}
	case 118: //v
	{ 
		pos[0] = 1.9;
		pos[2] = -3.8; 
		angle = -2.334; 
		break; 
	}
	case 99: //c
	{ 
		pos[0] = -1.5; 
		pos[2] = -3.4; 
		angle = 2.334; 
		break; 
	}
	default: 
	{ 
		break;
	}
	}
}


	
void myIdle(void)
{
	if (rotate >= 3600) 
		rotate = 0;
	else rotate++;
	if (mode == 0) 
	{
		move = 0;
	}
	else 
	{					
		if (mode == 2 && move < 2) move = move + 0.01;
		else mode = 2;
	}
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	glutCreateWindow("HW2:OpenGL Assignment 2  A051724 劉宇望");
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	init();
	glutReshapeFunc(&reshape);
	glutDisplayFunc(&myDisplay);
	glutIdleFunc(&myIdle);
	glutKeyboardFunc(&keyboard);
	glutMainLoop();
	return 0;
}

