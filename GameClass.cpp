#include "GameClass.h"
using namespace std;

float distance(float x1, float y1, float x2, float y2)
{
	float dx, dy;
	dx = x1 - x2;
	dy = y1 - y2;
	return sqrt(pow(dx, 2) + pow(dy, 2));
}

Map::Map() {
	//initializing Map! 한섭 is doing this!
}
void Map::setmap(int x, int y, int value)
{
	map[x][y] = value;
}
int Map::getmap(int n, int m)
{
	return map[n][m];
}



User::User() {
	direction = 2;
}
User::User(float x, float y) {
	pos_x = x;
	pos_y = y;
	direction = 2;
}
void User::move()
{
	//
}
//private 다루는 함수는 다 알아서 만들기
void User::set_direction(int dir)
{
	direction = dir;
}
int User::get_direction()
{
	return direction;
}
float User::get_pos_x()
{
	return pos_x;
}
float User::get_pos_y()
{
	return pos_y;
}
void User::set_pos(float x, float y)
{
	pos_x = x;
	pos_y = y;
}
int User::get_item_num()
{
	return item_num;
}
//아이템 개수를 return
void User::set_item_num(int n)
{
	item_num = n;
}
//아이템 개수를 n개로 맞춰주는 함수
void User::increase_item_num()
{
	item_num++;
}
//유저가 아이템을 먹었을때 아이템개수를 증가시키는 함수
void User::decrease_item_num()
{
	item_num--;
}
//유저가 아이템을 사용했을 때 아이템개수를 감소시키는 함수
bool User::isvalidDirection(int dir)
{
	switch (dir)
	{
	case 0:
		//북
		if (Map::getmap(int(pos_x - 0.5), int(pos_y + 0.5)) == 1 || Map::getmap(int(pos_x + 0.5), int(pos_y + 0.5)) == 1)
			return false;
		return true;
	case 1:
		//동
		if (Map::getmap(int(pos_x + 0.5), int(pos_y + 0.5)) == 1 || Map::getmap(int(pos_x + 0.5), int(pos_y - 0.5)) == 1)
			return false;
		return true;
	case 2:
		//남
		if (Map::getmap(int(pos_x - 0.5), int(pos_y - 0.5)) == 1 || Map::getmap(int(pos_x + 0.5), int(pos_y - 0.5)) == 1)
			return false;
		return true;
	case 3:
		//서
		if (Map::getmap(int(pos_x - 0.5), int(pos_y + 0.5)) == 1 || Map::getmap(int(pos_x - 0.5), int(pos_y - 0.5)) == 1)
			return false;
		return true;
	}
}
//dir 방향으로 현재 움직일수 있는지 확인해서 움직일수 있으면 true를 return한다






Enemy::Enemy() {
	is_aggro = 0;
	enemy_list.push_back(this);
}
Enemy::Enemy(float x, float y)
{
	pos_x = x;
	pos_y = y;
	enemy_list.push_back(this);
}
void Enemy::move() {
	//
}//is_aggro 세팅된거에 따라 무브를 한다. 플레이어와 가까워지면 플레이어를 죽인다! 총알에 의해 죽는건 총알클래스에 넣어놓을거니 구현 미필요. 
 //벽으로는 못가게해야함. 아이템은 못먹음. 시간마다 랜덤하게 움직여야함. 기준은 구현자가 알아서 하자!
float Enemy::get_pos_x()
{
	return pos_x;
}
float Enemy::get_pos_y()
{
	return pos_y;
}
void Enemy::set_pos(float x, float y)
{
	pos_x = x;
	pos_y = y;
}
Enemy* Enemy::get_enemy(int i)
{
	return enemy_list[i];
}
//i번째 enemy를 불러옴
vector<Enemy*> Enemy::get_enemy_list()
{
	return enemy_list;
}
//enemy_list를 불러옴
void Enemy::remove_enemy(int i)
{
	enemy_list.erase(enemy_list.begin() + i);
}
//i 번째 enemy를 삭제하는 함수
int Enemy::get_enemy_num()
{
	return enemy_list.size();
}
bool Enemy::isUserNear()
{
	return distance(pos_x, pos_y, User::get_pos_x(), User::get_pos_y()) <= AGGRO_DISTANCE;
}
//유저가 가까운 경우 true를 return한다. 거리 기준은 AGGRO_DISTANCE보다 가까운 경우를 기준으로 함
bool Enemy::isUserCollide() {
	return distance(pos_x, pos_y, User::get_pos_x(), User::get_pos_y()) <= DEATH_DISTANCE;
}
//유저와 충돌한 경우 true를 return한다. 거리 기준은 DEATH_DISTANCE보다 가까운 경우를 기준으로 함.
bool Enemy::isvalidDirection(int dir)
{
	switch (dir)
	{
	case 0:
		//북
		if (Map::getmap(int(pos_x - 0.5), int(pos_y + 0.5)) == 1 || Map::getmap(int(pos_x + 0.5), int(pos_y + 0.5)) == 1)
			return false;
		return true;
	case 1:
		//동
		if (Map::getmap(int(pos_x + 0.5), int(pos_y + 0.5)) == 1 || Map::getmap(int(pos_x + 0.5), int(pos_y - 0.5)) == 1)
			return false;
		return true;
	case 2:
		//남
		if (Map::getmap(int(pos_x - 0.5), int(pos_y - 0.5)) == 1 || Map::getmap(int(pos_x + 0.5), int(pos_y - 0.5)) == 1)
			return false;
		return true;
	case 3:
		//서
		if (Map::getmap(int(pos_x - 0.5), int(pos_y + 0.5)) == 1 || Map::getmap(int(pos_x - 0.5), int(pos_y - 0.5)) == 1)
			return false;
		return true;
	}
}
//vector<Enemy*> Enemy::enemy_list = vector<Enemy*>();


Bullet::Bullet() {
	switch (User::get_direction())
	{
	case 0://북
		pos_x = User::get_pos_x();
		pos_y = User::get_pos_y() + 0.3;
		break;
	case 1://동
		pos_x = User::get_pos_x() + 0.3;
		pos_y = User::get_pos_y();
		break;
	case 2://남
		pos_x = User::get_pos_x();
		pos_y = User::get_pos_y() - 0.3;
		break;
	case 3://서
		pos_x = User::get_pos_x() - 0.3;
		pos_y = User::get_pos_y();
		break;
	}
	direction = User::get_direction();
	velocity = BULLET_VELOCITY;
	bullet_list.push_back(this);
	cout << "BULLET AT : "<< pos_x<<","<<pos_y << '\n';
	cout << "bullet_list size: " << bullet_list.size() << '\n';
	cout << "BULLET_LIST_CHECK :" << bullet_list[0]->get_pos_x() << ' , ' << bullet_list[0]->get_pos_y() << '\n';
}
Bullet::Bullet(float x, float y, int dir) {
	pos_x = x;
	pos_y = y;
	direction = dir;
	velocity = BULLET_VELOCITY;
	bullet_list.push_back(this);
}
int Bullet::move()//enemy랑 부딪히면 enemy죽이고 총알 사라짐, 벽이랑 부딪혀도 사라짐
{
	switch (direction)
	{
	case 0://북
		pos_y += BULLET_VELOCITY;
		break;
	case 1://동
		pos_x += BULLET_VELOCITY;
		break;
	case 2://남
		pos_y -= BULLET_VELOCITY;
		break;
	case 3://서
		pos_x -= BULLET_VELOCITY;
		break;
	}
	cout << "Current position : " << pos_x << "," << pos_y << '\n';
	return check_state();
}
int Bullet::check_state()
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
Bullet* Bullet::get_bullet(int i)
{
	return bullet_list[i];
}
int Bullet::get_bullet_num()
{
	return bullet_list.size();
}
vector<Bullet*> Bullet::get_bullet_list()
{
	return bullet_list;
}
float Bullet::get_pos_x()
{
	return pos_x;
}
float Bullet::get_pos_y()
{
	return pos_y;
}



int User::direction;
float User::pos_x;
float User::pos_y;
int User::item_num;
int Map::map[100][100];
vector<Enemy*> Enemy::enemy_list;
vector<Bullet*> Bullet::bullet_list;