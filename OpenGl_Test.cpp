#include "GameClass.h"
#include<time.h>
#include<fstream>
#include<sstream>

//map Info.
#define map_col_size 50

#define blank 0
#define wall 1
#define player 2
#define enemy 3
#define item01 4
#define item02 5


using namespace std;

void reshape(int w, int h);
typedef struct rect {
	float x;
	float y;
	float width;
	float height;
} rect;
rect rectangle;

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
	glVertex2f(x + unit, y);
	glVertex2f(x + unit / 2, y + unit);
	glEnd();
}

void draw_wall(float x, float y, float unit) {
	//glClear(GL_COLOR_BUFFER_BIT);
	draw_square(x, y, unit, 0.7, 0.7, 0.7);
	//glutSwapBuffers();
}
void draw_blank(float x, float y, float unit) {
	draw_square(x, y, unit, 0, 0, 0);
}
void draw_player(float x, float y, float unit) {
	//glClear(GL_COLOR_BUFFER_BIT);
	draw_square(x, y, unit, 0, 0, 1);
	draw_square(x + unit * 0.15, y + unit * 0.65, unit*0.2, 1, 1, 1);
	draw_square(x + unit * 0.65, y + unit * 0.65, unit*0.2, 1, 1, 1);
	draw_triangle(x + unit * 0.4, y + unit * 0.2, unit*0.2, 1, 1, 1);
}

void draw_item01(float x, float y, float unit) {
	draw_square(x + unit * 0.4, y + unit * 0.7, unit*0.2, 0, 1, 0);
	draw_square(x + unit * 0.15, y + unit * 0.1, unit*0.7, 0, 1, 0);
	draw_triangle(x + unit * 0.3, y + unit * 0.2, unit*0.4, 0, 0, 0);
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
				draw_wall(j, (row - 1) - i, unit); break;
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


bool tick;
int region;



void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT); 
	User(1, 1);
	tick = false;
	region = 0;
	rectangle.x = User::get_pos_x();
	rectangle.y = User::get_pos_y();
	rectangle.width = 0.1;
	rectangle.height = 0.15;
}
void display(void)
{
	cout << "DISPLAY CALLED\n";

	glLoadIdentity();
	gluOrtho2D(User::get_pos_x() - 7.5, User::get_pos_x() + 7.5, User::get_pos_y() - 7.5, User::get_pos_y() + 7.5);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);

	int map[50][50];

	string filePath = "game_map.csv";
	ifstream openFile(filePath.data());
	if (openFile.is_open()) {
		string line;
		int i = 0;
		int j = 0;
		while (getline(openFile, line, '\n')) {
			stringstream lineStream(line);
			string cell;
			j = 0;
			while (getline(lineStream, cell, ',')) {
				map[i][j] = stoi(cell);
//				cout << cell << " ";
				j++;
			}
	//		cout << endl;
			i++;
		}
	//	cout << i << " " << j << endl;
		openFile.close();
	}

	/*for (int i = 0; i < 50; i++) {
	for (int j = 0; j < 50; j++) {
	cout << map[i][j] << " ";
	}
	cout << endl;
	}*/


	glClear(GL_COLOR_BUFFER_BIT);

	draw_map(map, 50, 50, 1);








	/*rectangle.x = User::get_pos_x() - 0.5;
	rectangle.y = User::get_pos_y() - 0.5;
	glVertex2f(rectangle.x, rectangle.y);
	glVertex2f(rectangle.x, rectangle.y + rectangle.width);
	glVertex2f(rectangle.x + rectangle.height, rectangle.y + rectangle.width);
	glVertex2f(rectangle.x + rectangle.height, rectangle.y);
	*/
	//draw_player(1, 1, 1);
	draw_player(User::get_pos_x() - 0.5, User::get_pos_y() - 0.5,1);

	for (int i = Bullet::get_bullet_num() - 1; i >= 0; i--)
	{
		draw_square(Bullet::get_bullet(i)->get_pos_x() - 0.05, Bullet::get_bullet(i)->get_pos_y() - 0.05, 0.1, 255,204,0);
	}
	
	//glEnd();
	glutSwapBuffers();


}
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	cout << "get_pos_x:" << User::get_pos_x() - 7.5 << '\n';
//	gluOrtho2D(User::get_pos_x() - 7.5, 20, -6, 20);
	//gluOrtho2D(User::get_pos_x() - 7.5, User::get_pos_x() + 7.5, User::get_pos_y() - 7.5, User::get_pos_y() + 7.5);
	cout << "RESHAPE CALLED" << '\n';
//	gluOrtho2D(0, 50, 0, 50);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == ' ')
	{
		new Bullet();
	}
	glutPostRedisplay();
}

void specialkeyboard(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		User::set_pos(User::get_pos_x(),User::get_pos_y()+UNIT_SPEED);
		User::set_direction(0);
		break;
	case GLUT_KEY_DOWN:
		User::set_pos(User::get_pos_x(), User::get_pos_y()- UNIT_SPEED);
		User::set_direction(2);
		break;
	case GLUT_KEY_RIGHT:
		User::set_pos(User::get_pos_x()+ UNIT_SPEED, User::get_pos_y());
		User::set_direction(1);
		break;
	case GLUT_KEY_LEFT:
		User::set_pos(User::get_pos_x()- UNIT_SPEED, User::get_pos_y());
		User::set_direction(3);
		break;
	}
	cout <<"User Position : " <<User::get_pos_x() << " , " << User::get_pos_y() << '\n';
	glutPostRedisplay();
}
/*
void timeFunc()
{
	if (!tick)
	{
		//
		
		int i = (GetTickCount() % 1000)/100;
		if (i != region)
		{
			region = i;
			tick = 1;
		}
	}
	else
	{
		tick
	}
}*/


void timer(int value) {

	for (int i = Bullet::get_bullet_num() - 1; i >= 0; i--)
	{
		//int state=Bullet::get_bullet(i)->move();
		cout << "BULLET POS : "<<Bullet::get_bullet(i)->get_pos_x() << ' , ' << Bullet::get_bullet(i)->get_pos_x() << '\n';
		Bullet::get_bullet(i)->move();
		/*
		if (state == -1)
		Bullet::get_bullet_list().erase(Bullet::get_bullet_list().begin()+i);*/
	}
//	glutReshapeFunc(reshape);
	glutPostRedisplay();      //윈도우를 다시 그리도록 요청
	glutTimerFunc(30, timer, 0); //다음 타이머 이벤트는 30밀리세컨트 후  호출됨.
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
	glutTimerFunc(0, timer, 0);
	glutMainLoop();
	return 0;
}