#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>

#define MAX_CHAR       128
typedef struct { GLfloat x, y; } point;
point p0 = { 0, -1 };
double pi = 3.14159;
GLfloat step = 0.1;
int time_interval = 200;

void drawObjects();
void drawBalloon(int n, double R, double movex, double movey);
void drawString(const char* str);
void selectFont(int size, int charset, const char* face);
void when_in_mainloop();
void OnTimer(int value);
void keyboard_input(unsigned char key, int x, int y);

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	//set the window
	glutInitWindowSize(1000, 600);
	glutInitWindowPosition(0, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0);
	glutCreateWindow("Birthday Card");
	glutDisplayFunc(drawObjects);
	glutIdleFunc(when_in_mainloop);
	glutTimerFunc(time_interval, OnTimer, 1);
	glutKeyboardFunc(keyboard_input); // keyboard callback function
	glutMainLoop();
}

void drawObjects() {
	glClearColor(1, 0.7, 0.7, 1.0);// Set background color: Lightpink
	glClear(GL_COLOR_BUFFER_BIT);
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	// Objecct 1: Balloon
	glColor3f(0.94f, 1.0f, 1.0f);// Set color: Azure
	drawBalloon(1000, 0.1, 0, 0);
	glColor3f(0.80f, 0.71f, 0.80f);// Set color: Thistle
	drawBalloon(1000, 0.08, 0.3, -0.2);
	glColor3f(0.80f, 0.75f, 0.69f);// Set color: AntiqueWhite
	drawBalloon(1000, 0.12, 0.45, -0.1);
	glColor3f(0.39f, 0.58f, 0.93f);// Set color: CornflowerBlue
	drawBalloon(1000, 0.07, -0.6, 0.3);
	// Object 2: Birthday cake
	glColor3f(1.0f, 0.98f, 0.8f);// Set color: LemonChiffon
	glRectf(-0.8, -0.7, -0.2, -0.3);
	glColor3f(0.61f, 0.80f, 0.61f);// Set color: DarkSeaGreen
	glRectf(-0.7, -0.3, -0.3, 0);
	glColor3f(0.74f, 0.56f, 0.56f);// Set color: RosyBrown
	glRectf(-0.6, 0, -0.4, 0.2);
	// Draw the candle on the cake
	glColor3f(1.0f, 0.45f, 0.34f);// Set color: Coral
	glRectf(-0.51, 0.2, -0.49, 0.3);
	glColor3f(1.0f, 0.0f, 0.0f);// Set color: Red
	glBegin(GL_POLYGON);
	glVertex2f(-0.5, 0.33);
	glVertex2f(-0.505, 0.31);
	glVertex2f(-0.505, 0.3);
	glVertex2f(-0.495, 0.3);
	glVertex2f(-0.495, 0.31);
	glEnd();
	// Object 3: "Happy Birthday!" text
	selectFont((width + height) / 40, ANSI_CHARSET, "Comic Sans MS");
	glColor3f(1.0f, 1.0f, 1.0f);// Set color: White
	glRasterPos2f(0.0f, 0.0f);
	drawString("Happy Birthday!");
	// Object 4: Ribbon
	double widthOfRibbon = (width + height) / 24;
	glLineWidth(widthOfRibbon);
	glColor3f(0.55f, 0.15f, 0.15f);// Set ribbon color: Firebrick
	glBegin(GL_LINES);
	glVertex2f(-1, -0.8);
	glVertex2f(1, -0.8);
	glEnd();
	glColor3f(0.55, 0.15, 0.15);
	glBegin(GL_LINES);
	glVertex2f(0.8, 1);
	glVertex2f(0.8, -1);
	glEnd();

	glFlush();
}

void drawBalloon(int n, double R, double movex, double movey) {
	int i;
	glBegin(GL_POLYGON);
	for (i = 0; i < n; i++) {
		glVertex2f(R * cos(2 * pi / n * i) * 0.5 + movex, R * sin(2 * pi / n * i) + p0.y + movey);
	}
	glEnd();
	glColor3f(0.0, 0.0, 1.0);
	glLineWidth(1);
	glBegin(GL_LINE_STRIP);
	glVertex2f(0 + movex, p0.y - R + movey);
	glVertex2f(0.01 + movex, p0.y - 2 * R + movey);
	glVertex2f(-0.01 + movex, p0.y - 4 * R + movey);
	glVertex2f(0 + movex, p0.y - 5 * R + movey);
	glEnd();
}

void drawString(const char* str) {
	GLuint lists;
	// Request for MAX_CHAR numbers of consecutive display list 
	lists = glGenLists(MAX_CHAR);
	// Load the draw command for each character into the corresponding display list
	wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	// Call the display list for each character and draw each character
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}

void selectFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT holdFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(holdFont);
}

void when_in_mainloop() { // idle callback function
	glutPostRedisplay(); // force OpenGL to redraw the current window
}

void OnTimer(int value) {
	p0.y += step;
	if (p0.y >= 1)
		p0.y = -1;
	else if (p0.y <= -1)
		p0.y = 0.9;
	// when_in_mainloop();
	glutTimerFunc(time_interval, OnTimer, 1);
}


void keyboard_input(unsigned char key, int x, int y) {// keyboard interaction
	if (key == 'q' || key == 'Q')
		exit(0);
	else if (key == 's' || key == 'S')// stop
		step = 0;
	else if (key == 'r' || key == 'R')// Restart
		step = 0.1;
}