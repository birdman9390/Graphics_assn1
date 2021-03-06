
#include "Angel.h"
#include "GameClass.h"
#include<time.h>
#include<fstream>
#include<sstream>
#include <assert.h>

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)
bool isPlayer = true;
int window_w;
int window_h;

GLuint VAO_box;
GLuint VBO_box;
GLuint VAO_item01;
GLuint VBO_item01;
GLuint VAO_item02;
GLuint VBO_item02;
GLuint VAO_dead;
GLuint VBO_dead;
GLuint VAO_clear;
GLuint VBO_clear;

GLuint vao, enemy_vao, player_vao, bullet_vao, wall_vao, item1_vao, item2_vao;
point4 points[NumVertices];
point4 bullet_points[NumVertices];
color4 colors[NumVertices];
color4 enemy_colors[NumVertices];
color4 wall_colors[NumVertices];
color4 bullet_colors[NumVertices];

point4 Bpoints[6];
color4 Bcolors[6];

point4 I01points[6];
color4 I01colors[6];

point4 I02points[6];
color4 I02colors[6];

point4 DeadPoints[6];
color4 DeadColors[6];

point4 ClearPoints[6];
color4 ClearColors[6];

mat4 projection;

GLfloat myleft, myright;
GLfloat mybottom, mytop;
GLfloat zNear = -10.0, zFar = 10.0;

point4 vertices[8] = {
	point4(-0.5, -0.5, 0.5, 1.0),
	point4(-0.5, 0.5, 0.5, 1.0),
	point4(0.5, 0.5, 0.5, 1.0),
	point4(0.5, -0.5, 0.5, 1.0),
	point4(-0.5, -0.5, -0.5, 1.0),
	point4(-0.5, 0.5, -0.5, 1.0),
	point4(0.5, 0.5, -0.5, 1.0),
	point4(0.5, -0.5, -0.5, 1.0)
};
point4 bullet_vertices[8] = {
	point4(-0.05, -0.05, 0.05, 1.0),
	point4(-0.05, 0.05, 0.05, 1.0),
	point4(0.05, 0.05, 0.05, 1.0),
	point4(0.05, -0.05, 0.05, 1.0),
	point4(-0.05, -0.05, -0.05, 1.0),
	point4(-0.05, 0.05, -0.05, 1.0),
	point4(0.05, 0.05, -0.05, 1.0),
	point4(0.05, -0.05, -0.05, 1.0)
};


// RGBA olors
color4 vertex_colors[8] = {
	//   color4(1.0, 0.0, 0.0, 1.0),  // red

	color4(0.0, 0.0, 0.0, 1.0),  // black
	color4(1.0, 0.0, 0.0, 1.0),  // red->enemy
	color4(1.0, 1.0, 0.0, 1.0),  // yellow
	color4(0.0, 1.0, 0.0, 1.0),  // green
	color4(0.0, 0.0, 1.0, 1.0),  // blue ->player
	color4(1.0, 0.0, 1.0, 1.0),  // magenta
	color4(1.0, 1.0, 1.0, 1.0),  // white
	color4(0.7, 0.7, 0.7, 1.0)   // wallCOLOR
};


point4 vBox[6] = {
	point4(0.0, 0.0, 0.0, 1.0),
	point4(1.0, 0.0, 0.0, 1.0),
	point4(1.0, 1.0, 0.0,1.0),

	point4(0.0, 0.0, 0.0, 1.0),
	point4(0.0, 1.0, 0.0,1.0),
	point4(1.0, 1.0, 0.0,1.0)
};
color4 cBox[] = {
	color4(0.7, 0.7, 0.7, 1.0),
	color4(0.7, 0.7, 0.7, 1.0),
	color4(0.7, 0.7, 0.7, 1.0),

	color4(0.7, 0.7, 0.7, 1.0),
	color4(0.7, 0.7, 0.7, 1.0),
	color4(0.7, 0.7, 0.7, 1.0),
};

point4 vItem01[6] = {
	point4(0.0, 0.0, 0.0, 1.0),
	point4(1.0, 0.0, 0.0, 1.0),
	point4(1.0, 1.0, 0.0,1.0),

	point4(0.0, 0.0, 0.0, 1.0),
	point4(0.0, 1.0, 0.0,1.0),
	point4(1.0, 1.0, 0.0,1.0)
};
color4 cItem01[] = {
	color4(0.0, 1.0, 0.0, 1.0),
	color4(0.0, 1.0, 0.0, 1.0),
	color4(0.0, 1.0, 0.0, 1.0),

	color4(0.0, 1.0, 0.0, 1.0),
	color4(0.0, 1.0, 0.0, 1.0),
	color4(0.0, 1.0, 0.0, 1.0)
};
point4 vItem02[6] = {
	point4(0.0, 0.0, 0.0, 1.0),
	point4(1.0, 0.0, 0.0, 1.0),
	point4(1.0, 1.0, 0.0,1.0),

	point4(0.0, 0.0, 0.0, 1.0),
	point4(0.0, 1.0, 0.0,1.0),
	point4(1.0, 1.0, 0.0,1.0)
};
color4 cItem02[]{
	color4(1.0, 1.0, 0.0, 1.0),
	color4(1.0, 1.0, 0.0, 1.0),
	color4(1.0, 1.0, 0.0, 1.0),

	color4(1.0, 1.0, 0.0, 1.0),
	color4(1.0, 1.0, 0.0, 1.0),
	color4(1.0, 1.0, 0.0, 1.0)
};

point4 vDead[6] = {
	point4(0.0, 0.0, 0.0, 1.0),
	point4(1.0, 0.0, 0.0, 1.0),
	point4(1.0, 1.0, 0.0,1.0),

	point4(0.0, 0.0, 0.0, 1.0),
	point4(0.0, 1.0, 0.0,1.0),
	point4(1.0, 1.0, 0.0,1.0)
};
color4 cDead[]{
	color4(1.0, 0.0, 0.0, 1.0),
	color4(1.0, 0.0, 0.0, 1.0),
	color4(1.0, 0.0, 0.0, 1.0),

	color4(1.0, 0.0, 0.0, 1.0),
	color4(1.0, 0.0, 0.0, 1.0),
	color4(1.0, 0.0, 0.0, 1.0)
};

point4 vClear[6] = {
	point4(0.0, 0.0, 0.0, 1.0),
	point4(1.0, 0.0, 0.0, 1.0),
	point4(1.0, 1.0, 0.0,1.0),

	point4(0.0, 0.0, 0.0, 1.0),
	point4(0.0, 1.0, 0.0,1.0),
	point4(1.0, 1.0, 0.0,1.0)
};
color4 cClear[]{
	color4(0.0, 0.0, 1.0, 1.0),
	color4(0.0, 0.0, 1.0, 1.0),
	color4(0.0, 0.0, 1.0, 1.0),

	color4(0.0, 0.0, 1.0, 1.0),
	color4(0.0, 0.0, 1.0, 1.0),
	color4(0.0, 0.0, 1.0, 1.0),
};
//----------------------------------------------------------------------------

class MatrixStack {
	int    _index;
	int    _size;
	mat4*  _matrices;

public:
	MatrixStack(int numMatrices = 32) :_index(0), _size(numMatrices)
	{
		_matrices = new mat4[numMatrices];
	}

	~MatrixStack()
	{
		delete[]_matrices;
	}

	void push(const mat4& m) {
		assert(_index + 1 < _size);
		_matrices[_index++] = m;
	}

	mat4& pop(void) {
		assert(_index - 1 >= 0);
		_index--;
		return _matrices[_index];
	}
};

MatrixStack  mvstack, enemy_mvstack;
mat4         model_view, enemy_model_view, map_model_view, item_model_view, state_model_view;
GLuint       ModelView, Projection;

//----------------------------------------------------------------------------
#define TORSO_HEIGHT 0.4
#define TORSO_WIDTH 0.1
#define UPPER_ARM_HEIGHT 0.15
#define LOWER_ARM_HEIGHT 0.12
#define UPPER_LEG_WIDTH  0.05
#define LOWER_LEG_WIDTH  0.05
#define LOWER_LEG_HEIGHT 0.15
#define UPPER_LEG_HEIGHT 0.25
#define UPPER_LEG_WIDTH  0.05
#define UPPER_ARM_WIDTH  0.05
#define LOWER_ARM_WIDTH  0.05
#define HEAD_HEIGHT 0.15
#define HEAD_WIDTH 0.1

#define PLAYER_COLOR 4
#define ENEMY_COLOR 1
#define WALL_COLOR 7
#define BULLET_COLOR 0
#define TIMELIMIT 100

//map Info.
#define blank 0
#define wall 1
#define player 2
#define enemy 3
#define item01 4
#define item02 5

// Set up menu item indices, which we can alos use with the joint angles
enum {
	Torso = 0,
	Head = 1,
	Head1 = 1,
	Head2 = 2,
	LeftUpperArm = 3,
	LeftLowerArm = 4,
	RightUpperArm = 5,
	RightLowerArm = 6,
	LeftUpperLeg = 7,
	LeftLowerLeg = 8,
	RightUpperLeg = 9,
	RightLowerLeg = 10,
	NumNodes,
	Quit
};

// Joint angles with initial values
GLfloat
theta[NumNodes] = {
	0.0,    // Torso
	0.0,    // Head1
	0.0,    // Head2
	180.0,    // LeftUpperArm
	0.0,    // LeftLowerArm
	180.0,    // RightUpperArm
	0.0,    // RightLowerArm
	180.0,  // LeftUpperLeg
	0.0,     // LeftLowerLeg
	180.0,  // RightUpperLeg
	0.0    // RightLowerLeg
};
GLfloat
enemy_theta[NumNodes] = {
	0.0,    // Torso
	0.0,    // Head1
	0.0,    // Head2
	180.0,    // LeftUpperArm
	0.0,    // LeftLowerArm
	180.0,    // RightUpperArm
	0.0,    // RightLowerArm
	180.0,  // LeftUpperLeg
	0.0,     // LeftLowerLeg
	180.0,  // RightUpperLeg
	0.0    // RightLowerLeg
};

GLint angle = Head2;

//----------------------------------------------------------------------------

struct Node {
	mat4  transform;
	void(*render)(void);
	Node* sibling;
	Node* child;

	Node() :
		render(NULL), sibling(NULL), child(NULL) {}

	Node(mat4& m, void(*render)(void), Node* sibling, Node* child) :
		transform(m), render(render), sibling(sibling), child(child) {}
};

Node  nodes[NumNodes];
Node enemy_nodes[NumNodes];
//----------------------------------------------------------------------------

int Index = 0;

void colorPlayer(int a, int b, int c, int d)
{
	colors[Index] = vertex_colors[PLAYER_COLOR]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[PLAYER_COLOR]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[PLAYER_COLOR]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[PLAYER_COLOR]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[PLAYER_COLOR]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[PLAYER_COLOR]; points[Index] = vertices[d]; Index++;

}

void colorEnemy(int a, int b, int c, int d)
{
	enemy_colors[Index] = vertex_colors[ENEMY_COLOR]; points[Index] = vertices[a]; Index++;
	enemy_colors[Index] = vertex_colors[ENEMY_COLOR]; points[Index] = vertices[b]; Index++;
	enemy_colors[Index] = vertex_colors[ENEMY_COLOR]; points[Index] = vertices[c]; Index++;
	enemy_colors[Index] = vertex_colors[ENEMY_COLOR]; points[Index] = vertices[a]; Index++;
	enemy_colors[Index] = vertex_colors[ENEMY_COLOR]; points[Index] = vertices[c]; Index++;
	enemy_colors[Index] = vertex_colors[ENEMY_COLOR]; points[Index] = vertices[d]; Index++;

}
void colorWall(int a, int b, int c, int d)
{
	wall_colors[Index] = vertex_colors[WALL_COLOR]; points[Index] = vertices[a]; Index++;
	wall_colors[Index] = vertex_colors[WALL_COLOR]; points[Index] = vertices[b]; Index++;
	wall_colors[Index] = vertex_colors[WALL_COLOR]; points[Index] = vertices[c]; Index++;
	wall_colors[Index] = vertex_colors[WALL_COLOR]; points[Index] = vertices[a]; Index++;
	wall_colors[Index] = vertex_colors[WALL_COLOR]; points[Index] = vertices[c]; Index++;
	wall_colors[Index] = vertex_colors[WALL_COLOR]; points[Index] = vertices[d]; Index++;

}

void colorBullet(int a, int b, int c, int d)
{
	bullet_colors[Index] = vertex_colors[BULLET_COLOR]; bullet_points[Index] = bullet_vertices[a]; Index++;
	bullet_colors[Index] = vertex_colors[BULLET_COLOR]; bullet_points[Index] = bullet_vertices[b]; Index++;
	bullet_colors[Index] = vertex_colors[BULLET_COLOR]; bullet_points[Index] = bullet_vertices[c]; Index++;
	bullet_colors[Index] = vertex_colors[BULLET_COLOR]; bullet_points[Index] = bullet_vertices[a]; Index++;
	bullet_colors[Index] = vertex_colors[BULLET_COLOR]; bullet_points[Index] = bullet_vertices[c]; Index++;
	bullet_colors[Index] = vertex_colors[BULLET_COLOR]; bullet_points[Index] = bullet_vertices[d]; Index++;
}


void Square() {
	for (int i = 0; i < 6; i++)
	{
		Bcolors[i] = cBox[i];
		Bpoints[i] = vBox[i];
	}
}

void Item01() {
	for (int i = 0; i < 6; i++)
	{
		I01colors[i] = cItem01[i];
		I01points[i] = vItem01[i];
	}
}

void Item02() {
	for (int i = 0; i < 6; i++)
	{
		I02colors[i] = cItem02[i];
		I02points[i] = vItem02[i];
	}
}

void Dead() {
	for (int i = 0; i < 6; i++)
	{
		DeadColors[i] = cDead[i];
		DeadPoints[i] = vDead[i];
	}
}

void Clear() {
	for (int i = 0; i < 6; i++)
	{
		ClearColors[i] = cClear[i];
		ClearPoints[i] = vClear[i];
	}
}


void
colorcube()
{
	colorPlayer(1, 0, 3, 2);
	colorPlayer(2, 3, 7, 6);
	colorPlayer(3, 0, 4, 7);
	colorPlayer(6, 5, 1, 2);
	colorPlayer(4, 5, 6, 7);
	colorPlayer(5, 4, 0, 1);

	Index = 0;
	colorEnemy(1, 0, 3, 2);
	colorEnemy(2, 3, 7, 6);
	colorEnemy(3, 0, 4, 7);
	colorEnemy(6, 5, 1, 2);
	colorEnemy(4, 5, 6, 7);
	colorEnemy(5, 4, 0, 1);

	Index = 0;
	colorWall(1, 0, 3, 2);
	colorWall(2, 3, 7, 6);
	colorWall(3, 0, 4, 7);
	colorWall(6, 5, 1, 2);
	colorWall(4, 5, 6, 7);
	colorWall(5, 4, 0, 1);

	Index = 0;
	colorBullet(1, 0, 3, 2);
	colorBullet(2, 3, 7, 6);
	colorBullet(3, 0, 4, 7);
	colorBullet(6, 5, 1, 2);
	colorBullet(4, 5, 6, 7);
	colorBullet(5, 4, 0, 1);

}

//----------------------------------------------------------------------------
int enemy_index;
void
traverse(Node* node)
{
	if (node == NULL) { return; }
	if (isPlayer)
	{
		mvstack.push(model_view);
		model_view *= node->transform;
		node->render();
		if (node->child) { traverse(node->child); }
		model_view = mvstack.pop();
		if (node->sibling) { traverse(node->sibling); }
	}
	else
	{
		enemy_mvstack.push(enemy_model_view);
		enemy_model_view *= node->transform;
		node->render();
		if (node->child) { traverse(node->child); }
		enemy_model_view = enemy_mvstack.pop();
		if (node->sibling) { traverse(node->sibling); }
	}
}
//----------------------------------------------------------------------------
void drawBullet()
{
	mvstack.push(model_view);
	glBindVertexArray(bullet_vao);

	for (int i = 0; i < Bullet::get_bullet_num(); i++)
	{
		model_view = Translate(Bullet::get_bullet_list()[i]->get_pos_x(), Bullet::get_bullet_list()[i]->get_pos_y(), 0);
		glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}

	model_view = mvstack.pop();
}

void
torso()
{
	if (isPlayer)
	{
		glBindVertexArray(vao);
		mvstack.push(model_view);

		mat4 instance = (Translate(0.0, 0.5 * TORSO_HEIGHT, 0.0) *
			Scale(TORSO_WIDTH, TORSO_HEIGHT, TORSO_WIDTH));

		glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);

		model_view = mvstack.pop();
	}

	else
	{
		glBindVertexArray(enemy_vao);
		enemy_mvstack.push(enemy_model_view);

		mat4 instance = (Translate(0.0, 0.5 * TORSO_HEIGHT, 0.0) *
			Scale(TORSO_WIDTH, TORSO_HEIGHT, TORSO_WIDTH));

		glUniformMatrix4fv(ModelView, 1, GL_TRUE, enemy_model_view * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);

		enemy_model_view = enemy_mvstack.pop();
	}
}

void
head()
{
	if (isPlayer)
	{
		mat4 instance = (Translate(0.0, 0.5 * HEAD_HEIGHT, 0.0) *
			Scale(HEAD_WIDTH, HEAD_HEIGHT, HEAD_WIDTH));

		glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}
	else
	{
		mat4 instance = (Translate(0.0, 0.5 * HEAD_HEIGHT, 0.0) *
			Scale(HEAD_WIDTH, HEAD_HEIGHT, HEAD_WIDTH));

		glUniformMatrix4fv(ModelView, 1, GL_TRUE, enemy_model_view * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}
}

void
left_upper_arm()
{
	if (isPlayer)
	{
		mat4 instance = (Translate(0.0, 0.5 * UPPER_ARM_HEIGHT, 0.0) *
			Scale(UPPER_ARM_WIDTH,
				UPPER_ARM_HEIGHT,
				UPPER_ARM_WIDTH));

		glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}
	else {
		mat4 instance = (Translate(0.0, 0.5 * UPPER_ARM_HEIGHT, 0.0) *
			Scale(UPPER_ARM_WIDTH,
				UPPER_ARM_HEIGHT,
				UPPER_ARM_WIDTH));

		glUniformMatrix4fv(ModelView, 1, GL_TRUE, enemy_model_view * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	}
}

void
left_lower_arm()
{
	if (isPlayer)
	{
		mat4 instance = (Translate(0.0, 0.5 * LOWER_ARM_HEIGHT, 0.0) *
			Scale(LOWER_ARM_WIDTH,
				LOWER_ARM_HEIGHT,
				LOWER_ARM_WIDTH));

		glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}
	else {
		mat4 instance = (Translate(0.0, 0.5 * LOWER_ARM_HEIGHT, 0.0) *
			Scale(LOWER_ARM_WIDTH,
				LOWER_ARM_HEIGHT,
				LOWER_ARM_WIDTH));

		glUniformMatrix4fv(ModelView, 1, GL_TRUE, enemy_model_view * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	}
}

void
right_upper_arm()
{
	if (isPlayer)
	{
		mat4 instance = (Translate(0.0, 0.5 * UPPER_ARM_HEIGHT, 0.0) *
			Scale(UPPER_ARM_WIDTH,
				UPPER_ARM_HEIGHT,
				UPPER_ARM_WIDTH));

		glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}
	else
	{

		mat4 instance = (Translate(0.0, 0.5 * UPPER_ARM_HEIGHT, 0.0) *
			Scale(UPPER_ARM_WIDTH,
				UPPER_ARM_HEIGHT,
				UPPER_ARM_WIDTH));

		glUniformMatrix4fv(ModelView, 1, GL_TRUE, enemy_model_view * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}
}

void
right_lower_arm()
{
	if (isPlayer)
	{
		mat4 instance = (Translate(0.0, 0.5 * LOWER_ARM_HEIGHT, 0.0) *
			Scale(LOWER_ARM_WIDTH,
				LOWER_ARM_HEIGHT,
				LOWER_ARM_WIDTH));

		glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}
	else {
		mat4 instance = (Translate(0.0, 0.5 * LOWER_ARM_HEIGHT, 0.0) *
			Scale(LOWER_ARM_WIDTH,
				LOWER_ARM_HEIGHT,
				LOWER_ARM_WIDTH));

		glUniformMatrix4fv(ModelView, 1, GL_TRUE, enemy_model_view * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	}
}

void
left_upper_leg()
{
	if (isPlayer)
	{
		mat4 instance = (Translate(0.0, 0.5 * UPPER_LEG_HEIGHT, 0.0) *
			Scale(UPPER_LEG_WIDTH,
				UPPER_LEG_HEIGHT,
				UPPER_LEG_WIDTH));

		glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}
	else
	{
		mat4 instance = (Translate(0.0, 0.5 * UPPER_LEG_HEIGHT, 0.0) *
			Scale(UPPER_LEG_WIDTH,
				UPPER_LEG_HEIGHT,
				UPPER_LEG_WIDTH));

		glUniformMatrix4fv(ModelView, 1, GL_TRUE, enemy_model_view * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}
}

void
left_lower_leg()
{
	if (isPlayer)
	{
		mat4 instance = (Translate(0.0, 0.5 * LOWER_LEG_HEIGHT, 0.0) *
			Scale(LOWER_LEG_WIDTH,
				LOWER_LEG_HEIGHT,
				LOWER_LEG_WIDTH));

		glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}
	else
	{
		mat4 instance = (Translate(0.0, 0.5 * LOWER_LEG_HEIGHT, 0.0) *
			Scale(LOWER_LEG_WIDTH,
				LOWER_LEG_HEIGHT,
				LOWER_LEG_WIDTH));

		glUniformMatrix4fv(ModelView, 1, GL_TRUE, enemy_model_view * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}
}

void
right_upper_leg()
{
	if (isPlayer)
	{
		mat4 instance = (Translate(0.0, 0.5 * UPPER_LEG_HEIGHT, 0.0) *
			Scale(UPPER_LEG_WIDTH,
				UPPER_LEG_HEIGHT,
				UPPER_LEG_WIDTH));

		glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}
	else {
		mat4 instance = (Translate(0.0, 0.5 * UPPER_LEG_HEIGHT, 0.0) *
			Scale(UPPER_LEG_WIDTH,
				UPPER_LEG_HEIGHT,
				UPPER_LEG_WIDTH));

		glUniformMatrix4fv(ModelView, 1, GL_TRUE, enemy_model_view * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}
}

void
right_lower_leg()
{
	if (isPlayer)
	{
		mat4 instance = (Translate(0.0, 0.5 * LOWER_LEG_HEIGHT, 0.0) *
			Scale(LOWER_LEG_WIDTH,
				LOWER_LEG_HEIGHT,
				LOWER_LEG_WIDTH));

		glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}
	else {
		mat4 instance = (Translate(0.0, 0.5 * LOWER_LEG_HEIGHT, 0.0) *
			Scale(LOWER_LEG_WIDTH,
				LOWER_LEG_HEIGHT,
				LOWER_LEG_WIDTH));

		glUniformMatrix4fv(ModelView, 1, GL_TRUE, enemy_model_view * instance);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}
}



void
initNodes(void)
{
	mat4  m, enemy_m;

	m = RotateY(theta[Torso]);
	nodes[Torso] = Node(m, torso, NULL, &nodes[Head1]);

	m = Translate(0.0, TORSO_HEIGHT + 0.5*HEAD_HEIGHT, 0.0) *
		RotateX(theta[Head1]) *
		RotateY(theta[Head2]);
	nodes[Head1] = Node(m, head, &nodes[LeftUpperArm], NULL);

	m = Translate(-(TORSO_WIDTH + UPPER_ARM_WIDTH), 0.9*TORSO_HEIGHT, 0.0) *
		RotateX(theta[LeftUpperArm]);
	nodes[LeftUpperArm] =
		Node(m, left_upper_arm, &nodes[RightUpperArm], &nodes[LeftLowerArm]);

	m = Translate(TORSO_WIDTH + UPPER_ARM_WIDTH, 0.9*TORSO_HEIGHT, 0.0) *
		RotateX(theta[RightUpperArm]);
	nodes[RightUpperArm] =
		Node(m, right_upper_arm,
			&nodes[LeftUpperLeg], &nodes[RightLowerArm]);

	m = Translate(-(TORSO_WIDTH + UPPER_LEG_WIDTH), 0.1*UPPER_LEG_HEIGHT, 0.0) *
		RotateX(theta[LeftUpperLeg]);
	nodes[LeftUpperLeg] =
		Node(m, left_upper_leg, &nodes[RightUpperLeg], &nodes[LeftLowerLeg]);

	m = Translate(TORSO_WIDTH + UPPER_LEG_WIDTH, 0.1*UPPER_LEG_HEIGHT, 0.0) *
		RotateX(theta[RightUpperLeg]);
	nodes[RightUpperLeg] =
		Node(m, right_upper_leg, NULL, &nodes[RightLowerLeg]);

	m = Translate(0.0, UPPER_ARM_HEIGHT, 0.0) * RotateX(theta[LeftLowerArm]);
	nodes[LeftLowerArm] = Node(m, left_lower_arm, NULL, NULL);

	m = Translate(0.0, UPPER_ARM_HEIGHT, 0.0) * RotateX(theta[RightLowerArm]);
	nodes[RightLowerArm] = Node(m, right_lower_arm, NULL, NULL);

	m = Translate(0.0, UPPER_LEG_HEIGHT, 0.0) * RotateX(theta[LeftLowerLeg]);
	nodes[LeftLowerLeg] = Node(m, left_lower_leg, NULL, NULL);

	m = Translate(0.0, UPPER_LEG_HEIGHT, 0.0) * RotateX(theta[RightLowerLeg]);
	nodes[RightLowerLeg] = Node(m, right_lower_leg, NULL, NULL);

	m = RotateY(theta[Torso]);
	enemy_nodes[Torso] = Node(m, torso, NULL, &enemy_nodes[Head1]);

	m = Translate(0.0, TORSO_HEIGHT + 0.5*HEAD_HEIGHT, 0.0) *
		RotateX(theta[Head1]) *
		RotateY(theta[Head2]);
	enemy_nodes[Head1] = Node(m, head, &enemy_nodes[LeftUpperArm], NULL);

	m = Translate(-(TORSO_WIDTH + UPPER_ARM_WIDTH), 0.9*TORSO_HEIGHT, 0.0) *
		RotateX(theta[LeftUpperArm]);
	enemy_nodes[LeftUpperArm] =
		Node(m, left_upper_arm, &enemy_nodes[RightUpperArm], &enemy_nodes[LeftLowerArm]);

	m = Translate(TORSO_WIDTH + UPPER_ARM_WIDTH, 0.9*TORSO_HEIGHT, 0.0) *
		RotateX(theta[RightUpperArm]);
	enemy_nodes[RightUpperArm] =
		Node(m, right_upper_arm,
			&enemy_nodes[LeftUpperLeg], &enemy_nodes[RightLowerArm]);

	m = Translate(-(TORSO_WIDTH + UPPER_LEG_WIDTH), 0.1*UPPER_LEG_HEIGHT, 0.0) *
		RotateX(theta[LeftUpperLeg]);
	enemy_nodes[LeftUpperLeg] =
		Node(m, left_upper_leg, &enemy_nodes[RightUpperLeg], &enemy_nodes[LeftLowerLeg]);

	m = Translate(TORSO_WIDTH + UPPER_LEG_WIDTH, 0.1*UPPER_LEG_HEIGHT, 0.0) *
		RotateX(theta[RightUpperLeg]);
	enemy_nodes[RightUpperLeg] =
		Node(m, right_upper_leg, NULL, &enemy_nodes[RightLowerLeg]);

	m = Translate(0.0, UPPER_ARM_HEIGHT, 0.0) * RotateX(theta[LeftLowerArm]);
	enemy_nodes[LeftLowerArm] = Node(m, left_lower_arm, NULL, NULL);

	m = Translate(0.0, UPPER_ARM_HEIGHT, 0.0) * RotateX(theta[RightLowerArm]);
	enemy_nodes[RightLowerArm] = Node(m, right_lower_arm, NULL, NULL);

	m = Translate(0.0, UPPER_LEG_HEIGHT, 0.0) * RotateX(theta[LeftLowerLeg]);
	enemy_nodes[LeftLowerLeg] = Node(m, left_lower_leg, NULL, NULL);

	m = Translate(0.0, UPPER_LEG_HEIGHT, 0.0) * RotateX(theta[RightLowerLeg]);
	enemy_nodes[RightLowerLeg] = Node(m, right_lower_leg, NULL, NULL);
}
int time_limit;


void init(void)
{
	srand((unsigned int)time(NULL));
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	time_limit = TIMELIMIT / 0.03;

	string filePath = "game_map_easy.csv";
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
				Map::setmap(j, (50 - 1) - i, stoi(cell));
				if (stoi(cell) == player)
					new User((float)j + 0.5, (50 - 1) - (float)i + 0.5);
				else if (stoi(cell) == enemy)
					new Enemy((float)j + 0.5, (50 - 1) - (float)i + 0.5);
				j++;
			}
			i++;
		}
		openFile.close();
	}

	colorcube();
	initNodes();

	// Create a vertex array object
	GLuint buffer, enemy_buffer, player_buffer, bullet_buffer, wall_buffer, item1_buffer, item2_buffer;
	GLuint program = InitShader("vshader_a9.glsl", "fshader_a9.glsl");
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	GLuint vColor = glGetAttribLocation(program, "vColor");

	// Load shaders and use the resulting shader program
	glUseProgram(program);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
		NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vColor);

	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points)));

	///////////////////

	glGenVertexArrays(1, &enemy_vao);
	glBindVertexArray(enemy_vao);

	// Create and initialize a buffer object
	glGenBuffers(1, &enemy_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, enemy_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(enemy_colors),
		NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(enemy_colors), enemy_colors);


	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vColor);

	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points)));

	//////////////////////////

	glGenVertexArrays(1, &bullet_vao);
	glBindVertexArray(bullet_vao);
	glGenBuffers(1, &bullet_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, bullet_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bullet_points) + sizeof(bullet_colors),
		NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(bullet_points), bullet_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(bullet_points), sizeof(bullet_colors), bullet_colors);

	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vColor);

	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(bullet_points)));

	///////////////////////////
	////////////여기부터 아이템 및 벽///////////////

	Square();

	glGenVertexArrays(1, &VAO_box);
	glGenBuffers(1, &VBO_box);

	glBindVertexArray(VAO_box);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_box);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Bpoints) + sizeof(Bcolors), NULL, GL_DYNAMIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Bpoints), Bpoints);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Bpoints), sizeof(Bcolors), Bcolors);

	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(Bpoints)));

	Item01();

	glGenVertexArrays(1, &VAO_item01);
	glGenBuffers(1, &VBO_item01);

	glBindVertexArray(VAO_item01);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_item01);
	glBufferData(GL_ARRAY_BUFFER, sizeof(I01points) + sizeof(I01colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(I01points), I01points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(I01points), sizeof(I01colors), I01colors);

	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(I01points)));

	Item02();

	glGenVertexArrays(1, &VAO_item02);
	glGenBuffers(1, &VBO_item02);

	glBindVertexArray(VAO_item02);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_item02);
	glBufferData(GL_ARRAY_BUFFER, sizeof(I02points) + sizeof(I02colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(I02points), I02points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(I02points), sizeof(I02colors), I02colors);

	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(I02points)));

	//////////////////////////아이템,벽//////////////////////

	Dead();
	Clear();
	glGenVertexArrays(1, &VAO_dead);
	glGenBuffers(1, &VBO_dead);

	glBindVertexArray(VAO_dead);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_dead);
	glBufferData(GL_ARRAY_BUFFER, sizeof(DeadPoints) + sizeof(DeadColors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(DeadPoints), DeadPoints);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(DeadPoints), sizeof(DeadColors), DeadColors);

	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(DeadPoints)));


	glGenVertexArrays(1, &VAO_clear);
	glGenBuffers(1, &VBO_clear);

	glBindVertexArray(VAO_clear);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_clear);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ClearPoints) + sizeof(ClearColors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ClearPoints), ClearPoints);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(ClearPoints), sizeof(ClearColors), ClearColors);

	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(ClearPoints)));

	ModelView = glGetUniformLocation(program, "ModelView");
	Projection = glGetUniformLocation(program, "Projection");

	glEnable(GL_DEPTH_TEST);

	glClearColor(1.0, 1.0, 1.0, 1.0);

}
void draw_shooting()
{

	int rl, rr;
	switch (User::get_direction())
	{
		//북동남서
	case 0:
		rl = 0;
		rr = 0;
		break;
	case 1:
		rl = 240;
		rr = 270;
		break;
	case 2:
		rl = 180;
		rr = 180;
		break;
	case 3:
		rl = 90;
		rr = 120;
		break;
	}

	nodes[LeftUpperArm].transform =
		Translate(-(TORSO_WIDTH + UPPER_ARM_WIDTH),
			0.9*TORSO_HEIGHT, 0.0) *
		RotateZ(rl);

	nodes[RightUpperArm].transform =
		Translate(TORSO_WIDTH + UPPER_ARM_WIDTH, 0.9*TORSO_HEIGHT, 0.0) *
		RotateZ(rr);

	nodes[LeftLowerArm].transform =
		Translate(0.0, UPPER_ARM_HEIGHT, 0.0) *
		RotateZ(0);

	nodes[RightLowerArm].transform =
		Translate(0.0, UPPER_ARM_HEIGHT, 0.0) *
		RotateZ(0);

	traverse(&nodes[Torso]);
}


void
drawMap() {
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 50; j++) {
			int object = Map::getmap(i, j);
			glUniformMatrix4fv(ModelView, 1, GL_TRUE, map_model_view * Translate(i, j, 0));
			if (object == wall) {
				glBindVertexArray(VAO_box);
				//glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view * Translate(2, 2, 2));
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			if (object == item01) {
				glBindVertexArray(VAO_item01);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			if (object == item02) {
				glBindVertexArray(VAO_item02);
				//glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view * Translate(-2, -2, -2));
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
	}
}

void display(void)
{
	glViewport(0, 0, window_w, window_h*0.75);
	model_view = mat4(1.0);
	model_view *= Translate(User::get_pos_x(), User::get_pos_y(), 0);   // An Identity matrix

	myleft = User::get_pos_x() - 5.0;
	myright = User::get_pos_x() + 5.0;
	mybottom = User::get_pos_y() - 5.0;
	mytop = User::get_pos_y() + 5.0;
	projection = Ortho(myleft, myright, mybottom, mytop, zNear, zFar);
	glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawMap();
	mvstack.push(model_view);

	isPlayer = true;
	if (User::get_shooting_count() == 0)
	{
		nodes[LeftUpperArm].transform =
			Translate(-(TORSO_WIDTH + UPPER_ARM_WIDTH),
				0.9*TORSO_HEIGHT, 0.0) *
			RotateZ(-50 * sin((M_PI / 180)*theta[LeftUpperArm]) + 180);

		nodes[RightUpperArm].transform =
			Translate(TORSO_WIDTH + UPPER_ARM_WIDTH, 0.9*TORSO_HEIGHT, 0.0) *
			RotateZ(-50 * sin((M_PI / 180)*theta[RightUpperArm]) + 180);

		nodes[LeftLowerArm].transform =
			Translate(0.0, UPPER_ARM_HEIGHT, 0.0) *
			RotateZ(15 * sin((M_PI / 180)*theta[LeftLowerArm]));

		nodes[RightLowerArm].transform =
			Translate(0.0, UPPER_ARM_HEIGHT, 0.0) *
			RotateZ(15 * sin((M_PI / 180)*theta[RightLowerArm]));

		traverse(&nodes[Torso]);
	}

	else
	{
		User::decrease_shooting_count();
		draw_shooting();
	}
	model_view = mvstack.pop();

	enemy_mvstack.push(enemy_model_view);
	isPlayer = false;
	for (enemy_index = 0; enemy_index < Enemy::get_enemy_num(); enemy_index++)
	{
		enemy_model_view = Translate(Enemy::get_enemy(enemy_index)->get_pos_x(), Enemy::get_enemy(enemy_index)->get_pos_y(), 0);
		traverse(&enemy_nodes[Torso]);
	}
	enemy_model_view = enemy_mvstack.pop();

	drawBullet();

	if (User::get_isDead()) {
		state_model_view = Translate(0, 0, 1)*Scale(50, 50, 1)*mat4(1.0);
		projection = Ortho(0, 50, 0, 50, zNear, zFar);
		cout << "user is dead" << endl;
		glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
		glUniformMatrix4fv(ModelView, 1, GL_TRUE, state_model_view);
		glBindVertexArray(VAO_dead);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	if (User::get_clear()) {
		state_model_view = Translate(0, 0, 1)*Scale(50, 50, 1)*mat4(1.0);
		projection = Ortho(0, 50, 0, 50, zNear, zFar);
		cout << "user is dead" << endl;
		glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
		glUniformMatrix4fv(ModelView, 1, GL_TRUE, state_model_view);
		glBindVertexArray(VAO_clear);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	/////////2nd view port///////////
	glViewport(0, window_h*0.75, window_w, window_h - window_h * 0.75);
	projection = Ortho(0, 10, 0, 3, zNear, zFar);
	glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);

	item_model_view = mat4(1.0);
	item_model_view = item_model_view * Translate(0.5, 0.5, 1)*Scale(0.7, 0.7, 1)*Translate(-0.5, -0.5, 1);

	int item01_num = User::get_item_num(1);
	int item02_num = User::get_item_num(2);
	for (int i = 0; i < item01_num; i++)
	{
		glUniformMatrix4fv(ModelView, 1, GL_TRUE, Translate(i, 1, 0)*item_model_view);
		glBindVertexArray(VAO_item01);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	for (int i = 0; i < item02_num; i++)
	{
		glUniformMatrix4fv(ModelView, 1, GL_TRUE, Translate(i, 2, 0)*item_model_view);
		glBindVertexArray(VAO_item02);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	int life_num = User::get_life();
	for (int i = 0; i < life_num; i++) {
		glUniformMatrix4fv(ModelView, 1, GL_TRUE, Translate(i, 0, 0)*item_model_view);
		glBindVertexArray(VAO_dead);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glutSwapBuffers();
}



void
reshape(int width, int height)
{
	window_w = width;
	window_h = height;

	myleft = User::get_pos_x() - 5.0;
	myright = User::get_pos_x() + 5.0;
	mybottom = User::get_pos_y() - 5.0;
	mytop = User::get_pos_y() + 5.0;

	GLfloat aspect = GLfloat(width) / height;

	if (aspect > 1.0) {
		myleft *= aspect;
		myright *= aspect;
	}
	else {
		mybottom /= aspect;
		mytop /= aspect;
	}

	mat4 projection = Ortho(myleft, myright, mybottom, mytop, zNear, zFar);

	glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
	model_view = mat4(1.0);
	model_view *= Translate(User::get_pos_x(), User::get_pos_y(), 0);   // An Identity matrix
	map_model_view = mat4(1.0);

	enemy_model_view = Translate(Enemy::get_enemy(0)->get_pos_x(), Enemy::get_enemy(0)->get_pos_y(), 0);
}

void reinit()
{
	time_limit = TIMELIMIT / 0.03;
	while (Enemy::get_enemy_num() != 0)
		Enemy::remove_enemy(0);
	//   loadMap();
	string filePath = "game_map_easy.csv";
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
				Map::setmap(j, (50 - 1) - i, stoi(cell));
				if (stoi(cell) == player)
					new User((float)j + 0.5, (50 - 1) - (float)i + 0.5);
				else if (stoi(cell) == enemy)
					new Enemy((float)j + 0.5, (50 - 1) - (float)i + 0.5);
				j++;
			}
			i++;
		}
		openFile.close();
	}
}
void keyboard(unsigned char key, int x, int y)
{
	if (key == ' ')
	{
		new Bullet();
		User::set_shooting_count(20);
	}
	else if (key == 'r' || key == 'R')
		reinit();
	glutPostRedisplay();
}

void timer(int value) {
	time_limit--;
	if (time_limit <= 0)
		User::set_isDead();

	cout << "MYLIFE :" << User::get_life() << '\n';
	for (int i = Bullet::get_bullet_num() - 1; i >= 0; i--)
	{
		int state = Bullet::get_bullet(i)->move();
		if (state == -2)
		{
			Bullet::remove_bullet(i);
		}
		else if (state >= 0)
		{
			Enemy::remove_enemy(state);
			Bullet::remove_bullet(i);
		}
	}
	Enemy::moveAll();

	for (int i = 0; i < Enemy::get_enemy_num(); i++) {
		if (Enemy::get_enemy(i)->isUserCollide()) {

			if (User::get_invincible_count() != 0)
			{
				break;
			}
			else if (User::get_life() > 1)
			{
				User::decrease_life();
				break;
			}
			else if (User::get_life() == 1)
			{
				User::set_isDead();
				User::decrease_life();
				break;
			}
		}
	}   //유저가 죽었는지 체크.
	User::itemCheck();   //먹은 아이템종류의 수를 증가시키고 콘솔에 출력.
	if (User::get_invincible_count() > 0)
		User::decrease_invincible_count();
	//여기서부터 에너미가 꾸물꾸물 움직이게 해준다.

	enemy_theta[LeftUpperArm] += 5.0;
	if (enemy_theta[LeftUpperArm] > 360.0) { enemy_theta[LeftUpperArm] -= 360.0; }
	enemy_theta[RightUpperArm] -= 5.0;
	if (enemy_theta[RightUpperArm] < 0.0) { enemy_theta[RightUpperArm] += 360.0; }
	enemy_theta[LeftLowerArm] += 5.0;
	if (enemy_theta[LeftLowerArm] > 360.0) { enemy_theta[LeftLowerArm] -= 360.0; }
	enemy_theta[RightLowerArm] -= 5.0;
	if (enemy_theta[RightLowerArm] < 0.0) { enemy_theta[RightLowerArm] += 360.0; }

	enemy_theta[LeftUpperLeg] += 5.0;
	if (enemy_theta[LeftUpperLeg] > 360.0) { enemy_theta[LeftUpperLeg] -= 360.0; }
	enemy_theta[RightUpperLeg] -= 5.0;
	if (enemy_theta[RightUpperLeg] < 0.0) { enemy_theta[RightUpperLeg] += 360.0; }
	enemy_theta[LeftLowerLeg] += 5.0;
	if (enemy_theta[LeftLowerLeg] > 360.0) { enemy_theta[LeftLowerLeg] -= 360.0; }
	enemy_theta[RightLowerLeg] -= 5.0;
	if (enemy_theta[RightLowerLeg] < 0.0) { enemy_theta[RightLowerLeg] += 360.0; }

	enemy_nodes[LeftUpperArm].transform =
		Translate(-(TORSO_WIDTH + UPPER_ARM_WIDTH),
			0.9*TORSO_HEIGHT, 0.0) *
		RotateZ(-50 * sin((M_PI / 180)*enemy_theta[LeftUpperArm]) + 180);

	enemy_nodes[RightUpperArm].transform =
		Translate(TORSO_WIDTH + UPPER_ARM_WIDTH, 0.9*TORSO_HEIGHT, 0.0) *
		RotateZ(-50 * sin((M_PI / 180)*enemy_theta[RightUpperArm]) + 180);


	enemy_nodes[LeftLowerArm].transform =
		Translate(0.0, UPPER_ARM_HEIGHT, 0.0) *
		RotateZ(15 * sin((M_PI / 180)*enemy_theta[LeftLowerArm]));


	enemy_nodes[RightLowerArm].transform =
		Translate(0.0, UPPER_ARM_HEIGHT, 0.0) *
		RotateZ(15 * sin((M_PI / 180)*enemy_theta[RightLowerArm]));

	enemy_nodes[RightUpperLeg].transform =
		Translate(TORSO_WIDTH + UPPER_LEG_WIDTH,
			0.1*UPPER_LEG_HEIGHT, 0.0) *
		RotateZ(-15 * sin((M_PI / 180)*enemy_theta[RightUpperLeg]) + 180);
	enemy_nodes[LeftUpperLeg].transform =
		Translate(-(TORSO_WIDTH + UPPER_LEG_WIDTH),
			0.1*UPPER_LEG_HEIGHT, 0.0) *
		RotateZ(-15 * sin((M_PI / 180)*enemy_theta[LeftUpperLeg]) + 180);
	enemy_nodes[LeftLowerLeg].transform =
		Translate(0.0, UPPER_LEG_HEIGHT, 0.0) *
		RotateZ(15 * sin((M_PI / 180)*enemy_theta[LeftLowerLeg]));
	enemy_nodes[RightLowerLeg].transform =
		Translate(0.0, UPPER_LEG_HEIGHT, 0.0) *
		RotateZ(15 * sin((M_PI / 180)*enemy_theta[RightLowerLeg]));

	glutPostRedisplay();      //윈도우를 다시 그리도록 요청
	glutTimerFunc(30, timer, 0); //다음 타이머 이벤트는 30밀리세컨트 후  호출됨.
}
bool lr = 0;
void
specialkeyboard(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_RIGHT: case GLUT_KEY_UP:
		theta[LeftUpperArm] += 25.0;
		if (theta[LeftUpperArm] > 360.0) { theta[LeftUpperArm] -= 360.0; }
		theta[RightUpperArm] -= 25.0;
		if (theta[RightUpperArm] < 0.0) { theta[RightUpperArm] += 360.0; }
		theta[LeftLowerArm] += 25.0;
		if (theta[LeftLowerArm] > 360.0) { theta[LeftLowerArm] -= 360.0; }
		theta[RightLowerArm] -= 25.0;
		if (theta[RightLowerArm] < 0.0) { theta[RightLowerArm] += 360.0; }


		theta[LeftUpperLeg] += 25.0;
		if (theta[LeftUpperLeg] > 360.0) { theta[LeftUpperLeg] -= 360.0; }
		theta[RightUpperLeg] -= 25.0;
		if (theta[RightUpperLeg] < 0.0) { theta[RightUpperLeg] += 360.0; }
		theta[LeftLowerLeg] += 25.0;
		if (theta[LeftLowerLeg] > 360.0) { theta[LeftLowerLeg] -= 360.0; }
		theta[RightLowerLeg] -= 25.0;
		if (theta[RightLowerLeg] < 0.0) { theta[RightLowerLeg] += 360.0; }



		nodes[LeftUpperArm].transform =
			Translate(-(TORSO_WIDTH + UPPER_ARM_WIDTH),
				0.9*TORSO_HEIGHT, 0.0) *
			RotateZ(-50 * sin((M_PI / 180)*theta[LeftUpperArm]) + 180);

		nodes[RightUpperArm].transform =
			Translate(TORSO_WIDTH + UPPER_ARM_WIDTH, 0.9*TORSO_HEIGHT, 0.0) *
			RotateZ(-50 * sin((M_PI / 180)*theta[RightUpperArm]) + 180);


		nodes[LeftLowerArm].transform =
			Translate(0.0, UPPER_ARM_HEIGHT, 0.0) *
			RotateZ(15 * sin((M_PI / 180)*theta[LeftLowerArm]));


		nodes[RightLowerArm].transform =
			Translate(0.0, UPPER_ARM_HEIGHT, 0.0) *
			RotateZ(15 * sin((M_PI / 180)*theta[RightLowerArm]));



		nodes[RightUpperLeg].transform =
			Translate(TORSO_WIDTH + UPPER_LEG_WIDTH,
				0.1*UPPER_LEG_HEIGHT, 0.0) *
			RotateZ(-15 * sin((M_PI / 180)*theta[RightUpperLeg]) + 180);
		nodes[LeftUpperLeg].transform =
			Translate(-(TORSO_WIDTH + UPPER_LEG_WIDTH),
				0.1*UPPER_LEG_HEIGHT, 0.0) *
			RotateZ(-15 * sin((M_PI / 180)*theta[LeftUpperLeg]) + 180);
		nodes[LeftLowerLeg].transform =
			Translate(0.0, UPPER_LEG_HEIGHT, 0.0) *
			RotateZ(15 * sin((M_PI / 180)*theta[LeftLowerLeg]));
		nodes[RightLowerLeg].transform =
			Translate(0.0, UPPER_LEG_HEIGHT, 0.0) *
			RotateZ(15 * sin((M_PI / 180)*theta[RightLowerLeg]));


		User::set_shooting_count(0);




		break;
	case GLUT_KEY_DOWN: case GLUT_KEY_LEFT:
		theta[LeftUpperArm] -= 25.0;
		if (theta[LeftUpperArm] < 0.0) { theta[LeftUpperArm] += 360.0; }
		theta[RightUpperArm] += 25.0;
		if (theta[RightUpperArm] > 360.0) { theta[RightUpperArm] -= 360.0; }
		theta[LeftLowerArm] -= 25.0;
		if (theta[LeftLowerArm] < 0.0) { theta[LeftLowerArm] += 360.0; }
		theta[RightLowerArm] += 25.0;
		if (theta[RightLowerArm] > 360.0) { theta[RightLowerArm] -= 360.0; }


		theta[LeftUpperLeg] -= 25.0;
		if (theta[LeftUpperLeg] < 0.0) { theta[LeftUpperLeg] += 360.0; }
		theta[RightUpperLeg] += 25.0;
		if (theta[RightUpperLeg] > 180.0) { theta[RightUpperLeg] -= 360.0; }
		theta[LeftLowerLeg] -= 25.0;
		if (theta[LeftLowerLeg] < 0.0) { theta[LeftLowerLeg] += 360.0; }
		theta[RightLowerLeg] += 25.0;
		if (theta[RightLowerLeg] > 180.0) { theta[RightLowerLeg] -= 360.0; }

		nodes[LeftUpperArm].transform =
			Translate(-(TORSO_WIDTH + UPPER_ARM_WIDTH),
				0.9*TORSO_HEIGHT, 0.0) *
			RotateZ(-50 * sin((M_PI / 180)*theta[LeftUpperArm]) + 180);

		nodes[RightUpperArm].transform =
			Translate(TORSO_WIDTH + UPPER_ARM_WIDTH, 0.9*TORSO_HEIGHT, 0.0) *
			RotateZ(-50 * sin((M_PI / 180)*theta[RightUpperArm]) + 180);


		nodes[LeftLowerArm].transform =
			Translate(0.0, UPPER_ARM_HEIGHT, 0.0) *
			RotateZ(15 * sin((M_PI / 180)*theta[LeftLowerArm]));


		nodes[RightLowerArm].transform =
			Translate(0.0, UPPER_ARM_HEIGHT, 0.0) *
			RotateZ(15 * sin((M_PI / 180)*theta[RightLowerArm]));



		nodes[RightUpperLeg].transform =
			Translate(TORSO_WIDTH + UPPER_LEG_WIDTH,
				0.1*UPPER_LEG_HEIGHT, 0.0) *
			RotateZ(-15 * sin((M_PI / 180)*theta[RightUpperLeg]) + 180);
		nodes[LeftUpperLeg].transform =
			Translate(-(TORSO_WIDTH + UPPER_LEG_WIDTH),
				0.1*UPPER_LEG_HEIGHT, 0.0) *
			RotateZ(-15 * sin((M_PI / 180)*theta[LeftUpperLeg]) + 180);
		nodes[LeftLowerLeg].transform =
			Translate(0.0, UPPER_LEG_HEIGHT, 0.0) *
			RotateZ(15 * sin((M_PI / 180)*theta[LeftLowerLeg]));
		nodes[RightLowerLeg].transform =
			Translate(0.0, UPPER_LEG_HEIGHT, 0.0) *
			RotateZ(15 * sin((M_PI / 180)*theta[RightLowerLeg]));

		User::set_shooting_count(0);

		break;
	case 033: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	}

	switch (key) {
	case GLUT_KEY_UP:
		User::set_direction(0);
		if (User::isvalidDirection(0))
			User::set_pos(User::get_pos_x(), User::get_pos_y() + UNIT_SPEED);
		break;
	case GLUT_KEY_DOWN:
		User::set_direction(2);
		if (User::isvalidDirection(2))
			User::set_pos(User::get_pos_x(), User::get_pos_y() - UNIT_SPEED);
		break;
	case GLUT_KEY_RIGHT:
		User::set_direction(1);
		if (User::isvalidDirection(1))
			User::set_pos(User::get_pos_x() + UNIT_SPEED, User::get_pos_y());
		break;
	case GLUT_KEY_LEFT:
		User::set_direction(3);
		if (User::isvalidDirection(3))
			User::set_pos(User::get_pos_x() - UNIT_SPEED, User::get_pos_y());
		break;
	}
	mvstack.push(model_view);

	model_view = mvstack.pop();
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

int
main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 700);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("robot");

	glewInit();

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialkeyboard);
	glutTimerFunc(0, timer, 0);

	glutMainLoop();
	return 0;
}