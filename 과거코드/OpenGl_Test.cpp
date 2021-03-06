/*


#include "Angel.h"
#include <assert.h>

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)
bool isPlayer = true;


GLuint vao, enemy_vao, player_vao, bullet_vao, wall_vao, item1_vao, item2_vao;
point4 points[NumVertices];
color4 colors[NumVertices];
color4 enemy_colors[NumVertices];
color4 wall_colors[NumVertices];


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

// RGBA olors
color4 vertex_colors[8] = {
//	color4(1.0, 0.0, 0.0, 1.0),  // red

	color4(0.0, 0.0, 0.0, 1.0),  // black
	color4(1.0, 0.0, 0.0, 1.0),  // red->enemy
	color4(1.0, 1.0, 0.0, 1.0),  // yellow
	color4(0.0, 1.0, 0.0, 1.0),  // green
	color4(0.0, 0.0, 1.0, 1.0),  // blue ->player
	color4(1.0, 0.0, 1.0, 1.0),  // magenta
	color4(1.0, 1.0, 1.0, 1.0),  // white
	color4(0.7, 0.7, 0.7, 1.0)   // wallCOLOR
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

MatrixStack  mvstack;
mat4         model_view;
GLuint       ModelView, Projection;

//----------------------------------------------------------------------------

#define TORSO_HEIGHT 0.5
#define TORSO_WIDTH 0.1
#define UPPER_ARM_HEIGHT 0.2
#define LOWER_ARM_HEIGHT 0.15
#define UPPER_LEG_WIDTH  0.05
#define LOWER_LEG_WIDTH  0.05
#define LOWER_LEG_HEIGHT 0.2
#define UPPER_LEG_HEIGHT 0.3
#define UPPER_LEG_WIDTH  0.05
#define UPPER_ARM_WIDTH  0.05
#define LOWER_ARM_WIDTH  0.05
#define HEAD_HEIGHT 0.15
#define HEAD_WIDTH 0.1

#define PLAYER_COLOR 4
#define ENEMY_COLOR 1
#define WALL_COLOR 7

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

}




//----------------------------------------------------------------------------

void
traverse(Node* node)
{
	if (node == NULL) { return; }

	mvstack.push(model_view);

	model_view *= node->transform;
	node->render();

	if (node->child) { traverse(node->child); }

	model_view = mvstack.pop();

	if (node->sibling) { traverse(node->sibling); }
}

//----------------------------------------------------------------------------

void
torso()
{
	if(isPlayer)
		glBindVertexArray(vao);
	else
		glBindVertexArray(enemy_vao);
	mvstack.push(model_view);

	mat4 instance = (Translate(0.0, 0.5 * TORSO_HEIGHT, 0.0) *
		Scale(TORSO_WIDTH, TORSO_HEIGHT, TORSO_WIDTH));

	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	model_view = mvstack.pop();
}

void
head()
{
	//mvstack.push(model_view);

	mat4 instance = (Translate(0.0, 0.5 * HEAD_HEIGHT, 0.0) *
		Scale(HEAD_WIDTH, HEAD_HEIGHT, HEAD_WIDTH));

	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	//model_view = mvstack.pop();
}

void
left_upper_arm()
{
	//mvstack.push(model_view);

	mat4 instance = (Translate(0.0, 0.5 * UPPER_ARM_HEIGHT, 0.0) *
		Scale(UPPER_ARM_WIDTH,
			UPPER_ARM_HEIGHT,
			UPPER_ARM_WIDTH));

	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	//model_view = mvstack.pop();
}

void
left_lower_arm()
{
//	mvstack.push(model_view);

	mat4 instance = (Translate(0.0, 0.5 * LOWER_ARM_HEIGHT, 0.0) *
		Scale(LOWER_ARM_WIDTH,
			LOWER_ARM_HEIGHT,
			LOWER_ARM_WIDTH));

	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	//model_view = mvstack.pop();
}

void
right_upper_arm()
{
	mvstack.push(model_view);

	mat4 instance = (Translate(0.0, 0.5 * UPPER_ARM_HEIGHT, 0.0) *
		Scale(UPPER_ARM_WIDTH,
			UPPER_ARM_HEIGHT,
			UPPER_ARM_WIDTH));

	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	model_view = mvstack.pop();
}

void
right_lower_arm()
{
	mvstack.push(model_view);

	mat4 instance = (Translate(0.0, 0.5 * LOWER_ARM_HEIGHT, 0.0) *
		Scale(LOWER_ARM_WIDTH,
			LOWER_ARM_HEIGHT,
			LOWER_ARM_WIDTH));

	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	model_view = mvstack.pop();
}

void
left_upper_leg()
{
	mvstack.push(model_view);

	mat4 instance = (Translate(0.0, 0.5 * UPPER_LEG_HEIGHT, 0.0) *
		Scale(UPPER_LEG_WIDTH,
			UPPER_LEG_HEIGHT,
			UPPER_LEG_WIDTH));

	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	model_view = mvstack.pop();
}

void
left_lower_leg()
{
	mvstack.push(model_view);

	mat4 instance = (Translate(0.0, 0.5 * LOWER_LEG_HEIGHT, 0.0) *
		Scale(LOWER_LEG_WIDTH,
			LOWER_LEG_HEIGHT,
			LOWER_LEG_WIDTH));

	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	model_view = mvstack.pop();
}

void
right_upper_leg()
{
	mvstack.push(model_view);

	mat4 instance = (Translate(0.0, 0.5 * UPPER_LEG_HEIGHT, 0.0) *
		Scale(UPPER_LEG_WIDTH,
			UPPER_LEG_HEIGHT,
			UPPER_LEG_WIDTH));

	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	model_view = mvstack.pop();
}

void
right_lower_leg()
{
	mvstack.push(model_view);

	mat4 instance = (Translate(0.0, 0.5 * LOWER_LEG_HEIGHT, 0.0) *
		Scale(LOWER_LEG_WIDTH,
			LOWER_LEG_HEIGHT,
			LOWER_LEG_WIDTH));

	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	model_view = mvstack.pop();
}

//----------------------------------------------------------------------------

void
display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mvstack.push(model_view);

	isPlayer = true;
	traverse(&nodes[Torso]);

	model_view *= Translate(-5.0, -5.0, 0.0);
	isPlayer = false;
	traverse(&enemy_nodes[Torso]);

	model_view = mvstack.pop();
	glutSwapBuffers();
}

//----------------------------------------------------------------------------

void
mouse(int button, int state, int x, int y)
{
	//
}

//----------------------------------------------------------------------------

void
menu(int option)
{
	if (option == Quit) {
		exit(EXIT_SUCCESS);
	}

	angle = option;
}

//----------------------------------------------------------------------------

void
reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	GLfloat left = -10.0, right = 10.0;
	GLfloat bottom = -10.0, top = 10.0;
	GLfloat zNear = -10.0, zFar = 10.0;

	GLfloat aspect = GLfloat(width) / height;

	if (aspect > 1.0) {
		left *= aspect;
		right *= aspect;
	}
	else {
		bottom /= aspect;
		top /= aspect;
	}

	mat4 projection = Ortho(left, right, bottom, top, zNear, zFar);
	glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);

	model_view = mat4(1.0);   // An Identity matrix
}

//----------------------------------------------------------------------------

void
initNodes(void)
{
	mat4  m,enemy_m;

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








	////////////////////////

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

//----------------------------------------------------------------------------

void
init(void)
{
	colorcube();

//	for (int i = 0; i < NumVertices; i++)
	//{
	//	printf("%d : %x\n", i,colors[i]);
	//}
	// Initialize tree
	initNodes();

	// Create a vertex array object
	GLuint buffer, enemy_buffer, player_buffer, bullet_buffer, wall_buffer, item1_buffer, item2_buffer;
	GLuint program = InitShader("vshader_a9.glsl", "fshader_a9.glsl");
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	GLuint vColor = glGetAttribLocation(program, "vColor");
	GLuint vPosition_enemy = glGetAttribLocation(program, "vPosition");
	GLuint vColor_enemy = glGetAttribLocation(program, "vColor");
	GLuint vPosition_wall = glGetAttribLocation(program, "vPosition");
	GLuint vColor_wall = glGetAttribLocation(program, "vColor");

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
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors),colors);


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


	glEnableVertexAttribArray(vPosition_enemy);
	glVertexAttribPointer(vPosition_enemy, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vColor_enemy);

	glVertexAttribPointer(vColor_enemy, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points)));


	///////////////////////////












	ModelView = glGetUniformLocation(program, "ModelView");
	Projection = glGetUniformLocation(program, "Projection");



	glEnable(GL_DEPTH_TEST);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glClearColor(1.0, 1.0, 1.0, 1.0);

}

//----------------------------------------------------------------------------
bool lr = 0;
void
keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'd':
		theta[LeftUpperArm] += 5.0;
		if (theta[LeftUpperArm] > 360.0) { theta[LeftUpperArm] -= 360.0; }
		theta[RightUpperArm] -= 5.0;
		if (theta[RightUpperArm] < 0.0) { theta[RightUpperArm] += 360.0; }
		theta[LeftLowerArm] += 5.0;
		if (theta[LeftLowerArm] > 360.0) { theta[LeftLowerArm] -= 360.0; }
		theta[RightLowerArm] -= 5.0;
		if (theta[RightLowerArm] < 0.0) { theta[RightLowerArm] += 360.0; }


		theta[LeftUpperLeg] += 5.0;
		if (theta[LeftUpperLeg] > 360.0) { theta[LeftUpperLeg] -= 360.0; }
		theta[RightUpperLeg] -= 5.0;
		if (theta[RightUpperLeg] < 0.0) { theta[RightUpperLeg] += 360.0; }
		theta[LeftLowerLeg] += 5.0;
		if (theta[LeftLowerLeg] > 360.0) { theta[LeftLowerLeg] -= 360.0; }
		theta[RightLowerLeg] -= 5.0;
		if (theta[RightLowerLeg] < 0.0) { theta[RightLowerLeg] += 360.0; }
		
		
		
		nodes[LeftUpperArm].transform =
			Translate(-(TORSO_WIDTH + UPPER_ARM_WIDTH),
				0.9*TORSO_HEIGHT, 0.0) *
			RotateZ(-50*sin((M_PI/180)*theta[LeftUpperArm]) + 180);

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
			RotateZ(-15 * sin((M_PI / 180)*theta[RightUpperLeg])+180);
		nodes[LeftUpperLeg].transform =
			Translate(-(TORSO_WIDTH + UPPER_LEG_WIDTH),
				0.1*UPPER_LEG_HEIGHT, 0.0) *
			RotateZ(-15 * sin((M_PI / 180)*theta[LeftUpperLeg])+180);
		nodes[LeftLowerLeg].transform =
			Translate(0.0, UPPER_LEG_HEIGHT, 0.0) *
			RotateZ(15 * sin((M_PI / 180)*theta[LeftLowerLeg]));
		nodes[RightLowerLeg].transform =
			Translate(0.0, UPPER_LEG_HEIGHT, 0.0) *
			RotateZ(15 * sin((M_PI / 180)*theta[RightLowerLeg]));







		break;
	case 'a':
		theta[LeftUpperArm] -= 5.0;
		if (theta[LeftUpperArm] < 0.0) { theta[LeftUpperArm] += 360.0; }
		theta[RightUpperArm] += 5.0;
		if (theta[RightUpperArm] > 360.0) { theta[RightUpperArm] -= 360.0; }
		theta[LeftLowerArm] -= 5.0;
		if (theta[LeftLowerArm]< 0.0) { theta[LeftLowerArm] += 360.0; }
		theta[RightLowerArm] += 5.0;
		if (theta[RightLowerArm] > 360.0) { theta[RightLowerArm] -= 360.0; }


		theta[LeftUpperLeg] -= 5.0;
		if (theta[LeftUpperLeg] < 0.0) { theta[LeftUpperLeg] += 360.0; }
		theta[RightUpperLeg] += 5.0;
		if (theta[RightUpperLeg] > 180.0) { theta[RightUpperLeg] -= 360.0; }
		theta[LeftLowerLeg] -= 5.0;
		if (theta[LeftLowerLeg] < 0.0) { theta[LeftLowerLeg] += 360.0; }
		theta[RightLowerLeg] += 5.0;
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


		break;
	case 033: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
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
	glutInitWindowSize(512, 512);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("robot");

	glewInit();

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);


	glutMainLoop();
	return 0;
}


*/



















/*

#include "GameClass.h"
#include<fstream>
#include<iostream>
//map Info.
#define blank 0
#define wall 1
#define player 2
#define enemy 3
#define item01 4
#define item02 5
using namespace std;

int window_w;
int window_h;

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
	draw_square(x, y, unit, 0.7, 0.7, 0.7);
}
void draw_blank(float x, float y, float unit) {
	draw_square(x, y, unit, 0, 0, 0);
}
void draw_player(float x, float y, float unit) {
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

void draw_map(int row, int col, int unit) {
	int object;
	for (int i = 0; i <row; i++) {
		for (int j = 0; j < col; j++) {
			object = Map::getmap(i, j);
			switch (object) {
			case blank:
				draw_blank(i, j, unit); break;
			case wall:
				draw_wall(i, j, unit); break;
			case item01:
				draw_blank(i, j, unit);
				draw_item01(i, j, unit); break;
			case item02:
				draw_blank(i, j, unit);
				draw_item02(i, j, unit); break;
			}
		}
	}
}


void init(void)
{
	srand((unsigned int)time(NULL));
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);


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
void display(void)
{
	//====================================첫 번째 Viewport 구간 시작====================================
	glViewport(0, 0, window_w, window_h*0.85);
	glLoadIdentity();
	gluOrtho2D(User::get_pos_x() - 7.5, User::get_pos_x() + 7.5, User::get_pos_y() - 7.5, User::get_pos_y() + 7.5);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glClear(GL_COLOR_BUFFER_BIT);

	draw_map(50, 50, 1);

	draw_player(User::get_pos_x() - 0.5, User::get_pos_y() - 0.5, 1);

	for (int i = 0; i < Enemy::get_enemy_num(); i++)
	{
		draw_enemy(Enemy::get_enemy(i)->get_pos_x() - 0.5, Enemy::get_enemy(i)->get_pos_y() - 0.5, 1);
	}

	for (int i = Bullet::get_bullet_num() - 1; i >= 0; i--)
	{
		draw_square(Bullet::get_bullet(i)->get_pos_x() - 0.05, Bullet::get_bullet(i)->get_pos_y() - 0.05, 0.1, 255, 204, 0);
	}

	if (User::get_isDead()) {
		cout << "User is dead :(" << endl;
		draw_square(0, 0, 50, 1, 0, 0);//패배의 붉은색.
	}

	if (User::get_clear()) {
		cout << "Game Clear!!!" << endl;
		draw_square(0, 0, 50, 0, 0, 1);//승리의 파란색.
	}

	//====================================두 번째 Viewport 구간 시작====================================
	glViewport(0, window_h*0.85, window_w, window_h - window_h * 0.85);
	glLoadIdentity();
	gluOrtho2D(0, 10, 0, 2); //2x10 크기의 아이템 창 설정. unit단위가 변경됨.

	int item01_num = User::get_item_num(1);
	int item02_num = User::get_item_num(2);
	for (int i = 0; i < item01_num; i++)
		draw_item01(i, 0, 1);
	for (int i = 0; i < item02_num; i++)
		draw_item02(i, 1, 1);

	glutSwapBuffers();
}
void reshape(int w, int h)
{
	window_w = w;
	window_h = h;
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
	glutPostRedisplay();
}



void timer(int value) {

	for (int i = Bullet::get_bullet_num() - 1; i >= 0; i--)
	{
		int state = Bullet::get_bullet(i)->move();
		cout << "TIMER!!!, " << state << '\n';
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
			User::set_isDead();
		}
	}	//유저가 죽었는지 체크.
	User::itemCheck();	//먹은 아이템종류의 수를 증가시키고 콘솔에 출력.

	glutPostRedisplay();      //윈도우를 다시 그리도록 요청
	glutTimerFunc(30, timer, 0); //다음 타이머 이벤트는 30밀리세컨트 후  호출됨.
}





int main(int argc, char** argv)
{
	
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 600); //viewport 2개 사용: 게임 플레이 사이즈는 500x500, 상단 아이템 정보창 사이즈는 500x100
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
*/

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

void draw_map(int row, int col, int unit) {
	int object;
	for (int i = 0; i <row; i++) {
		for (int j = 0; j < col; j++) {
			object = Map::getmap(i, j);
			switch (object) {
			case blank:
				draw_blank(i, j , unit); break;
			case wall:
				draw_wall(i, j , unit); break;
			case item01:
				draw_blank(i, j, unit);
				draw_item01(i, j, unit); break;
			case item02:
				draw_blank(i, j, unit);
				draw_item02(i, j, unit); break;
			}
		}
	}
}


void init(void)
{

	srand((unsigned int)time(NULL));
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT); 


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
					new User((float)j + 0.5, (50-1)-(float)i+ 0.5);
				else if (stoi(cell) == enemy)
					new Enemy((float)j + 0.5, (50 - 1) - (float)i + 0.5);
				j++;
			}
			i++;
		}
		openFile.close();
	}
}
void display(void)
{

	glLoadIdentity();
	gluOrtho2D(User::get_pos_x() - 7.5, User::get_pos_x() + 7.5, User::get_pos_y() - 7.5, User::get_pos_y() + 7.5);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);

//	int map[50][50];


	glClear(GL_COLOR_BUFFER_BIT);

	draw_map(50, 50, 1);

	draw_player(User::get_pos_x() - 0.5, User::get_pos_y() - 0.5, 1);

	for (int i = 0; i < Enemy::get_enemy_num(); i++)
	{
		draw_enemy(Enemy::get_enemy(i)->get_pos_x() - 0.5, Enemy::get_enemy(i)->get_pos_y() - 0.5,1);
	}

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
//	gluOrtho2D(User::get_pos_x() - 7.5, 20, -6, 20);
	//gluOrtho2D(User::get_pos_x() - 7.5, User::get_pos_x() + 7.5, User::get_pos_y() - 7.5, User::get_pos_y() + 7.5);
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
		User::set_direction(0);
		if(User::isvalidDirection(0))
			User::set_pos(User::get_pos_x(),User::get_pos_y()+UNIT_SPEED);
		break;
	case GLUT_KEY_DOWN:
		User::set_direction(2);
		if (User::isvalidDirection(2))
			User::set_pos(User::get_pos_x(), User::get_pos_y()- UNIT_SPEED);
		break;
	case GLUT_KEY_RIGHT:
		User::set_direction(1);
		if (User::isvalidDirection(1))
			User::set_pos(User::get_pos_x()+ UNIT_SPEED, User::get_pos_y());
		break;
	case GLUT_KEY_LEFT:
		User::set_direction(3);
		if (User::isvalidDirection(3))
			User::set_pos(User::get_pos_x()- UNIT_SPEED, User::get_pos_y());
		break;
	}
//	cout << "DIRECIONT VALID?: " << (bool)(User::isvalidDirection(0)) << '\n';
	glutPostRedisplay();
}



void timer(int value) {

	for (int i = Bullet::get_bullet_num() - 1; i >= 0; i--)
	{
		int state=Bullet::get_bullet(i)->move();
		cout << "TIMER!!!, "<<state<<'\n';
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