#include <windows.h>
#include <gl/gl.h>/
#include <gl/glut.h>
#include <vector>
#include<iostream>
#include<math.h>

#define DEATH_DISTANCE 0.5
using namespace std;
class test
{
private:
	static int count;
	static vector<test*> test_vector;
	int value;
	int* a;
public:
	test()
	{
		count++;
		test_vector.push_back(this);
	}
	void setvalue(int n)
	{
		value = n;
	}
	int getvalue()
	{
		return value;
	}
	static int getcount()
	{
		return count;
	}
	static vector<test*> get_test_vector()
	{
		return test_vector;
	}
};

int test::count = 0;
vector<test*> test::test_vector = vector<test*>();

int main()
{
	test a;
	test b;
	cout << "hello" << '\n';
	cout << test::getcount() << '\n';
	a.setvalue(5);
	b.setvalue(3);
	cout << test::get_test_vector()[0]->getvalue() << ',' << test::get_test_vector()[1]->getvalue() << '\n';
	return 0;
}

float distance(float x1, float y1, float x2, float y2)
{
	float dx, dy;
	dx = x1 - x2;
	dy = y1 - y2;
	return sqrt(pow(dx, 2) + pow(dy, 2));
}


class Map
{
private:
	static int map[100][100];//0 : 빈곳, 1: 벽, 2: User, 3: Enemy, 4: Item
	//여기서 게임을 진행하자!
public:
	Map() {
		//initializing Map! 한섭 is doing this!
	}
	static void setmap(int x, int y, int value)
	{
		map[x][y] = value;
	}
	static int getmap(int n, int m)
	{
		return map[n][m];
	}
};


class User
{
private:
	static int direction;// 0:북, 1: 동, 2: 남, 3: 서 
	//User의 direction은 default로 남쪽(2)으로 설정되어 있다.
	static float pos_x, pos_y;
	static int item_num;//유저가 먹은 아이템의 개수
public:
	User() {
		direction = 2;
	}
	User(float x,float y) {
		pos_x = x;
		pos_y = y;
		direction = 2;
	}
	void move()
	{
		//
	}
	//private 다루는 함수는 다 알아서 만들기
	static void set_direction(int dir)
	{
		direction = dir;
	}
	static int get_direction()
	{
		return direction;
	}
	static float get_pos_x()
	{
		return pos_x;
	}
	static float get_pos_y()
	{
		return pos_y;
	}
	static void set_pos(float x, float y)
	{
		pos_x = x;
		pos_y = y;
	}
	static int get_item_num()
	{
		return item_num;
	}
	//아이템 개수를 return
	static void set_item_num(int n)
	{
		item_num = n;
	}
	//아이템 개수를 n개로 맞춰주는 함수
	static void increase_item_num()
	{
		item_num++;
	}
	//유저가 아이템을 먹었을때 아이템개수를 증가시키는 함수
	static void decrease_item_num()
	{
		item_num--;
	}
	//유저가 아이템을 사용했을 때 아이템개수를 감소시키는 함수
};

class Enemy
{
private:
	float pos_x, pos_y;
	bool is_aggro;//0 : 어그로 안끌림, 1: 어그로 끌림.  default로 어그로 안끌린 상태(0)로 설정되어 있음
	static vector<Enemy*> enemy_list;
public:
	Enemy() {
		is_aggro = 0;
		enemy_list.push_back(this);
	}
	Enemy(float x,float y)
	{
		pos_x = x;
		pos_y = y;
		enemy_list.push_back(this);
	}
	void move(){
		//
	}//is_aggro 세팅된거에 따라 무브를 한다. 플레이어와 가까워지면 플레이어를 죽인다! 총알에 의해 죽는건 총알클래스에 넣어놓을거니 구현 미필요. 
	//벽으로는 못가게해야함. 아이템은 못먹음. 시간마다 랜덤하게 움직여야함. 기준은 구현자가 알아서 하자!
	float get_pos_x()
	{
		return pos_x;
	}
	float get_pos_y()
	{
		return pos_y;
	}
	void set_pos(float x, float y)
	{
		pos_x = x;
		pos_y = y;
	}
	static Enemy* get_enemy(int i)
	{
		return enemy_list[i];
	}
	//i번째 enemy를 불러옴
	static vector<Enemy*> get_enemy_list()
	{
		return enemy_list;
	}
	//enemy_list를 불러옴
	static void remove_enemy(int i)
	{
		enemy_list.erase(enemy_list.begin() + i);
	}
	//i 번째 enemy를 삭제하는 함수
	static int get_enemy_num()
	{
		return enemy_list.size();
	}
};
vector<Enemy*> Enemy::enemy_list = vector<Enemy*>();



class Bullet
{
private:
	float pos_x,pos_y;
	int direction;
	int velocity;
public:
	Bullet(){}
	Bullet(float x, float y,int dir) {
		pos_x = x;
		pos_y = y;
		direction = dir;
	}
	void move()//enemy랑 부딪히면 enemy죽이고 총알 사라짐, 벽이랑 부딪혀도 사라짐
	{
		///
	}
	int check_state()
	{
		if (Map::getmap(int(pos_x), int(pos_y)) == 1)
		{
			return -2;
		}
		else
		{
			for (int i = 0; i < Enemy::get_enemy_num(); i++)
			{
				if (distance(pos_x, pos_y, Enemy::get_enemy(i)->get_pos_x(), Enemy::get_enemy(i)->get_pos_y()) <= DEATH_DISTANCE)
				{
					return i;
				}
			}
			return -1;
		}
	}//일반적인 상황에선 -1을 return, 벽에 부딪힐경우 -2를 return, enemy랑 부딪힐때는 enemy의 index를 return
};

