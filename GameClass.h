#pragma once

#include <windows.h>
#include <gl/gl.h>/
#include <gl/glut.h>
#include <vector>
#include<iostream>
#include<math.h>

#define DEATH_DISTANCE 0.5
#define AGGRO_DISTANCE 5
#define BULLET_VELOCITY 0.2
#define UNIT_SPEED 0.05
using namespace std;

float distance(float x1, float y1, float x2, float y2);
class Map
{
private:
	static int map[100][100];//0 : 빈곳, 1: 벽, 2: User, 3: Enemy, 4: Item
							 //여기서 게임을 진행하자!
public:
	Map();
	static void setmap(int x, int y, int value);
	static int getmap(int n, int m);
};

class User
{
private:
	static int direction;// 0:북, 1: 동, 2: 남, 3: 서 
						 //User의 direction은 default로 남쪽(2)으로 설정되어 있다.
	static float pos_x, pos_y;
	static int item_num;//유저가 먹은 아이템의 개수
public:
	User();
	User(float x, float y);
	void move();
	//private 다루는 함수는 다 알아서 만들기
	static void set_direction(int dir);
	static int get_direction();
	static float get_pos_x();
	static float get_pos_y();
	static void set_pos(float x, float y);
	static int get_item_num();
	//아이템 개수를 return
	static void set_item_num(int n);
	//아이템 개수를 n개로 맞춰주는 함수
	static void increase_item_num();
	//유저가 아이템을 먹었을때 아이템개수를 증가시키는 함수
	static void decrease_item_num();
	//유저가 아이템을 사용했을 때 아이템개수를 감소시키는 함수
	bool isvalidDirection(int dir);
	//dir 방향으로 현재 움직일수 있는지 확인해서 움직일수 있으면 true를 return한다
};
class Enemy
{
private:
	float pos_x, pos_y;
	bool is_aggro;//0 : 어그로 안끌림, 1: 어그로 끌림.  default로 어그로 안끌린 상태(0)로 설정되어 있음
	static vector<Enemy*> enemy_list;
public:
	Enemy();
	Enemy(float x, float y);
	void move();
	 //벽으로는 못가게해야함. 아이템은 못먹음. 시간마다 랜덤하게 움직여야함. 기준은 구현자가 알아서 하자!
	float get_pos_x();
	float get_pos_y();
	void set_pos(float x, float y);
	static Enemy* get_enemy(int i);
	//i번째 enemy를 불러옴
	static vector<Enemy*> get_enemy_list();
	//enemy_list를 불러옴
	static void remove_enemy(int i);
	//i 번째 enemy를 삭제하는 함수
	static int get_enemy_num();
	bool isUserNear();
	//유저가 가까운 경우 true를 return한다. 거리 기준은 AGGRO_DISTANCE보다 가까운 경우를 기준으로 함
	bool isUserCollide();
	//유저와 충돌한 경우 true를 return한다. 거리 기준은 DEATH_DISTANCE보다 가까운 경우를 기준으로 함.
	bool isvalidDirection(int dir);
	//dir 방향으로 현재 움직일수 있는지 확인해서 움직일수 있으면 true를 return한다
};
//vector<Enemy*> Enemy::enemy_list = vector<Enemy*>();


class Bullet
{
private:
	float pos_x, pos_y;
	int direction;
	int velocity;
	static vector<Bullet*> bullet_list;
public:
	Bullet();
	Bullet(float x, float y, int dir);
	int move();
	int check_state();
	static Bullet* get_bullet(int i);
	static vector<Bullet*> get_bullet_list();
	static int get_bullet_num();
	float get_pos_x();
	float get_pos_y();
};
