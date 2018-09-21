#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
using namespace std;

//map Info.
#define map_col_size 50

#define blank 0
#define wall 1
#define player 2
#define enemy 3
#define item01 4
#define item02 5

typedef struct rect {
	float x;
	float y;
	float width;
	float height;
} rect;
rect rectangle;
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	rectangle.x = 0.45;
	rectangle.y = 0.48;
	rectangle.width = 0.1;
	rectangle.height = 0.15;
}
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 50, 0, 50);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void draw_square(float x, float y, float unit, float R, float G, float B) {
	glColor3f(R, G, B);
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x, y + unit);
	glVertex2f(x + unit, y + unit);
	glVertex2f(x + unit, y);
	glEnd();
}

void draw_triangle(float x, float y, float unit, float R, float G, float B) {
	glColor3f(R, G, B);
	glBegin(GL_TRIANGLES);
	glVertex2f(x, y);
	glVertex2f(x+unit, y);
	glVertex2f(x + unit/2, y+unit);
	glEnd();
}

void draw_wall(float x, float y, float unit) {
	//glClear(GL_COLOR_BUFFER_BIT);
	draw_square(x, y, unit, 0.7, 0.7, 0.7);
	//glutSwapBuffers();
}
void draw_blank(float x, float y, float unit) {
	draw_square(x, y, unit, 0,0,0);
}
void draw_player(float x, float y, float unit) {
	//glClear(GL_COLOR_BUFFER_BIT);
	draw_square(x, y, unit, 0, 0, 1);
	draw_square(x + unit * 0.15, y + unit * 0.65, unit*0.2, 1, 1, 1);
	draw_square(x + unit * 0.65, y + unit * 0.65, unit*0.2, 1, 1, 1);
	draw_triangle(x + unit*0.4, y+unit*0.2, unit*0.2, 1, 1, 1);
}

void draw_item01(float x, float y, float unit) {
	draw_square(x + unit * 0.4, y + unit * 0.7, unit*0.2, 0, 1, 0);
	draw_square(x + unit * 0.15, y + unit * 0.1, unit*0.7, 0, 1, 0);
	draw_triangle(x + unit * 0.3 ,y + unit * 0.2, unit*0.4, 0, 0, 0);
}
void draw_item02(float x, float y, float unit) {
	draw_square(x + unit * 0.4, y + unit * 0.7, unit*0.2, 1, 1, 0);
	draw_square(x + unit * 0.15, y + unit * 0.1, unit*0.7, 1, 1, 0);
	draw_triangle(x + unit * 0.3, y + unit * 0.2, unit*0.4, 0, 0, 0);
}

void draw_enemy(float x, float y, float unit) {
	draw_square(x, y, unit, 1, 0, 0);
	draw_square(x + unit * 0.15, y + unit * 0.65, unit*0.2, 1, 1, 1);
	draw_square(x + unit * 0.65, y + unit * 0.65, unit*0.2, 1, 1, 1);
	draw_triangle(x + unit * 0.4, y + unit * 0.2, unit*0.2, 1, 1, 1);
}

void draw_map(int map[][map_col_size], int row, int col, int unit) {
	int object;
	for (int i = 0; i <row; i++) {
		for (int j = 0; j < col; j++) {
			object = map[i][j];
			switch (object) {
			case blank:
				draw_blank(j, (row - 1) - i, unit); break;
			case wall:
				draw_wall(j, (row-1)-i, unit); break;
			case player:
				draw_player(j, (row - 1) - i, unit); break;
			case enemy:
				draw_enemy(j, (row - 1) - i, unit); break;
			case item01:
				draw_blank(j, (row - 1) - i, unit);
				draw_item01(j, (row - 1) - i, unit); break;
			case item02:
				draw_blank(j, (row - 1) - i, unit);
				draw_item02(j, (row - 1) - i, unit); break;
			}
		}
	}
}
void display(void)
{
	int map[50][50];
	
	string filePath = "game_map.csv";
	ifstream openFile(filePath.data());
	if (openFile.is_open()) {
		string line;
		int i = 0;
		int j = 0;
		while (getline(openFile, line,'\n')) {
			stringstream lineStream(line);
			string cell;
			j = 0;
			while (getline(lineStream, cell,',')) {
				map[i][j] = stoi(cell);
				cout << cell <<" ";
				j++;
			}
			cout << endl;
			i++;
		}
		cout << i <<" "<< j << endl;
		openFile.close();
	}

	/*for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 50; j++) {
			cout << map[i][j] << " ";
		}
		cout << endl;
	}*/

	
	glClear(GL_COLOR_BUFFER_BIT);
	//draw_wall(50, 50, 10);
	//draw_player(10, 10, 10);
	//draw_enemy(30, 30, 10);
	//draw_item01(10, 30, 10);
	//draw_item02(30, 10, 10);
	draw_map(map, 50, 50, 1);
	glutSwapBuffers();
}
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'i':
		rectangle.y += 0.005;
		break;
	case 'm':
		rectangle.y -= 0.005;
		break;
	case 'k':
		rectangle.x += 0.005;
		break;
	case 'j':
		rectangle.x -= 0.005;
		break;
	}
	glutPostRedisplay();
}
void specialkeyboard(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		rectangle.y += 0.005;
		break;
	case GLUT_KEY_DOWN:
		rectangle.y -= 0.005;
		break;
	case GLUT_KEY_RIGHT:
		rectangle.x += 0.005;
		break;
	case GLUT_KEY_LEFT:
		rectangle.x -= 0.005;
		break;
	}
	glutPostRedisplay();
}
int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialkeyboard);
	
	glutMainLoop();
	return 0;
}

