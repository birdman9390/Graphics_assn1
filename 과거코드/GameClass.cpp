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
	isDead = false;
	item01_num = 0;
	item02_num = 0;
}
User::User(float x, float y) {
	pos_x = x;
	pos_y = y;
	direction = 2;
	isDead = false;
	item01_num = 0;
	item02_num = 0;
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
int User::get_item_num(int item_type)
{
	if (item_type == 1)
		return item01_num;
	if (item_type == 2)
		return item02_num;
}
//아이템 개수를 return

void User::increase_item_num(int item_type)
{
	if (item_type == 1)
		item01_num++;
	if (item_type == 2)
		item02_num++;
}
//유저가 아이템을 먹었을때 아이템개수를 증가시키는 함수
void User::decrease_item_num(int item_type)
{
	if (item_type == 1)
		item01_num--;
	if (item_type == 2)
		item02_num--;
}
//유저가 아이템을 사용했을 때 아이템개수를 감소시키는 함수
bool User::isvalidDirection(int dir)
{
	switch (dir)
	{
	case 0:
		//북
		if (Map::getmap(int(pos_x - 0.5), int(pos_y + 0.5)) == 1 || Map::getmap(int(pos_x + 0.49), int(pos_y + 0.5)) == 1)
			return false;
		return true;
	case 1:
		//동
		if (Map::getmap(int(pos_x + 0.5), int(pos_y + 0.499)) == 1 || Map::getmap(int(pos_x + 0.5), int(pos_y - 0.5)) == 1)
			return false;
		return true;
	case 2:
		//남
		if (Map::getmap(int(pos_x - 0.5), int(pos_y - 0.501)) == 1 || Map::getmap(int(pos_x + 0.499), int(pos_y - 0.501)) == 1)
			return false;
		return true;
	case 3:
		//서
		if (Map::getmap(int(pos_x - 0.501), int(pos_y + 0.499)) == 1 || Map::getmap(int(pos_x - 0.501), int(pos_y - 0.5)) == 1)
			return false;
		return true;
	}
}
//dir 방향으로 현재 움직일수 있는지 확인해서 움직일수 있으면 true를 return한다

int User::itemCheck() {
	if (Map::getmap(int(pos_x), int(pos_y)) == 4) {
		cout << "item01" << endl;
		Map::setmap(int(pos_x), int(pos_y), 0);
		increase_item_num(1);
		return 1;
	}
	else if (Map::getmap(int(pos_x), int(pos_y)) == 5) {
		cout << "item02" << endl;
		Map::setmap(int(pos_x), int(pos_y), 0);
		increase_item_num(2);
		return 2;
	}
	else
		return 0;
}

void User::set_isDead() {
	isDead = true;
}
bool User::get_isDead() {
	return isDead;
}
void User::set_clear() {
	isClear = true;
}
bool User::get_clear() {
	return isClear;
}


Enemy::Enemy() {
	direction = 2;
	enemy_list.push_back(this);
}
Enemy::Enemy(float x, float y)
{
	direction = 2;
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
	enemy_list.erase(enemy_list.begin() + i);//???delete 안써도 되는 걸까.

	int enemy_num = enemy_list.size();
	cout << enemy_num << " enemy left..." << endl;
	if (enemy_num == 0)
		User::set_clear();
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
		if (Map::getmap(int(pos_x - 0.5), int(pos_y + 0.5)) == 1 || Map::getmap(int(pos_x + 0.49), int(pos_y + 0.5)) == 1)
			return false;
		return true;
	case 1:
		//동
		if (Map::getmap(int(pos_x + 0.5), int(pos_y + 0.499)) == 1 || Map::getmap(int(pos_x + 0.5), int(pos_y - 0.5)) == 1)
			return false;
		return true;
	case 2:
		//남
		if (Map::getmap(int(pos_x - 0.5), int(pos_y - 0.501)) == 1 || Map::getmap(int(pos_x + 0.499), int(pos_y - 0.501)) == 1)
			return false;
		return true;
	case 3:
		//서
		if (Map::getmap(int(pos_x - 0.501), int(pos_y + 0.499)) == 1 || Map::getmap(int(pos_x - 0.501), int(pos_y - 0.5)) == 1)
			return false;
		return true;
	case 4:
		return true;
	}
}

bool Enemy::istickzero()
{
	return tick == 0;
}
void Enemy::increasetick()
{
	tick = (tick + 1) % 10;
}

void Enemy::moveAll()
{
	tick = (tick + 1) % 10;
	if (tick == 0)
	{
		for (int i = 0; i < get_enemy_num(); i++)
		{
			if (enemy_list[i]->isUserNear()) {
				enemy_list[i]->setdirectionToUser();
			}
			else
				enemy_list[i]->setdirection(rand() % 5);
		}
	}
	for (int i = 0; i < get_enemy_num(); i++)
	{
		switch (enemy_list[i]->getdirection())
		{
		case 0://북
			if (enemy_list[i]->isvalidDirection(0))
				enemy_list[i]->add_to_y(0.05); break;
			break;
		case 1://동
			if (enemy_list[i]->isvalidDirection(1))
				enemy_list[i]->add_to_x(0.05); break;
			break;
		case 2://남
			if (enemy_list[i]->isvalidDirection(2))
				enemy_list[i]->add_to_y(-0.05); break;
			break;
		case 3://서
			if (enemy_list[i]->isvalidDirection(3))
				enemy_list[i]->add_to_x(-0.05); break;
			break;
		case 4:
			break;
		}
	}

}
void Enemy::add_to_x(float i)
{
	pos_x += i;
}
void Enemy::add_to_y(float i)
{
	pos_y += i;
}
void Enemy::setdirection(int dir)
{
	direction = dir;
}
int Enemy::getdirection()
{
	return direction;
}

void Enemy::setdirectionToUser() {
	float user_x = User::get_pos_x();
	float user_y = User::get_pos_y();
	float x_distance = pow(pos_x - user_x, 2);
	float y_distance = pow(pos_y - user_y, 2);

	if (x_distance > y_distance) {//x, y축 중 먼거리가 가까워져야함.
		if (pos_x < user_x)
			setdirection(1);//동으로 설정
		else
			setdirection(3);//서
	}
	else {
		if (pos_y < user_y)
			setdirection(0);//북
		else
			setdirection(2);//남
	}

}


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

void Bullet::remove_bullet(int i)
{
	bullet_list.erase(bullet_list.begin() + i);
}


int User::direction;
float User::pos_x;
float User::pos_y;
int User::item01_num;
int User::item02_num;
bool User::isDead;
bool User::isClear;
int Map::map[50][50];
int Enemy::tick = 0;
vector<Enemy*> Enemy::enemy_list;
vector<Bullet*> Bullet::bullet_list;


/*#include "GameClass.h"
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
		if (Map::getmap(int(pos_x - 0.5), int(pos_y + 0.5)) == 1 || Map::getmap(int(pos_x + 0.49), int(pos_y + 0.5)) == 1)
			return false;
		return true;
	case 1:
		//동
		if (Map::getmap(int(pos_x + 0.5), int(pos_y + 0.499)) == 1 || Map::getmap(int(pos_x + 0.5), int(pos_y - 0.5)) == 1)
			return false;
		return true;
	case 2:
		//남
		if (Map::getmap(int(pos_x - 0.5), int(pos_y - 0.501)) == 1 || Map::getmap(int(pos_x + 0.499), int(pos_y - 0.501)) == 1)
			return false;
		return true;
	case 3:
		//서
		if (Map::getmap(int(pos_x - 0.501), int(pos_y + 0.499)) == 1 || Map::getmap(int(pos_x - 0.501), int(pos_y - 0.5)) == 1)
			return false;
		return true;
	}
}
//dir 방향으로 현재 움직일수 있는지 확인해서 움직일수 있으면 true를 return한다






Enemy::Enemy() {
	direction = 2;
	enemy_list.push_back(this);
}
Enemy::Enemy(float x, float y)
{
	direction = 2;
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
		if (Map::getmap(int(pos_x - 0.5), int(pos_y + 0.5)) == 1 || Map::getmap(int(pos_x + 0.49), int(pos_y + 0.5)) == 1)
			return false;
		return true;
	case 1:
		//동
		if (Map::getmap(int(pos_x + 0.5), int(pos_y + 0.499)) == 1 || Map::getmap(int(pos_x + 0.5), int(pos_y - 0.5)) == 1)
			return false;
		return true;
	case 2:
		//남
		if (Map::getmap(int(pos_x - 0.5), int(pos_y - 0.501)) == 1 || Map::getmap(int(pos_x + 0.499), int(pos_y - 0.501)) == 1)
			return false;
		return true;
	case 3:
		//서
		if (Map::getmap(int(pos_x - 0.501), int(pos_y + 0.499)) == 1 || Map::getmap(int(pos_x - 0.501), int(pos_y - 0.5)) == 1)
			return false;
		return true;
	case 4:
		return true;
	}
}
//vector<Enemy*> Enemy::enemy_list = vector<Enemy*>();

bool Enemy::istickzero()
{
	return tick == 0;
}
void Enemy::increasetick()
{
	tick = (tick + 1) % 10;
}

void Enemy::moveAll()
{
	tick = (tick + 1) % 10;
	if (tick == 0)
	{
		for (int i = 0; i < get_enemy_num(); i++)
		{
			enemy_list[i]->setdirection(rand() % 5);
		}
	}
	for (int i = 0; i < get_enemy_num(); i++)
	{
		switch (enemy_list[i]->getdirection())
		{
		case 0://북
			if(enemy_list[i]->isvalidDirection(0))
				enemy_list[i]->add_to_y(0.05); break;
			break;
		case 1://동
			if (enemy_list[i]->isvalidDirection(1))
				enemy_list[i]->add_to_x(0.05); break;
			break;
		case 2://남
			if (enemy_list[i]->isvalidDirection(2))
				enemy_list[i]->add_to_y(-0.05); break;
			break;
		case 3://서
			if (enemy_list[i]->isvalidDirection(3))
				enemy_list[i]->add_to_x(-0.05); break;
			break;
		case 4:
			break;
		}
	}

}
void Enemy::add_to_x(float i)
{
	pos_x += i;
}
void Enemy::add_to_y(float i)
{
	pos_y += i;
}
void Enemy::setdirection(int dir)
{
	direction = dir;
}
int Enemy::getdirection()
{
	return direction;
}



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
	cout << "BULLET POSITION: " << pos_x << " , " << pos_y << '\n';
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
//				cout << "DEATH_DISTANCE : " << distance(pos_x, pos_y, Enemy::get_enemy(i)->get_pos_x(), Enemy::get_enemy(i)->get_pos_y()) << '\n';
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

void Bullet::remove_bullet(int i)
{
	bullet_list.erase(bullet_list.begin() + i);
}


int User::direction;
float User::pos_x;
float User::pos_y;
int User::item_num;
int Map::map[50][50];
int Enemy::tick = 0;
vector<Enemy*> Enemy::enemy_list;
vector<Bullet*> Bullet::bullet_list;*/