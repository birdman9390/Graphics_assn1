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
	//initializing Map! �Ѽ� is doing this!
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
//private �ٷ�� �Լ��� �� �˾Ƽ� �����
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
//������ ������ return
void User::set_item_num(int n)
{
	item_num = n;
}
//������ ������ n���� �����ִ� �Լ�
void User::increase_item_num()
{
	item_num++;
}
//������ �������� �Ծ����� �����۰����� ������Ű�� �Լ�
void User::decrease_item_num()
{
	item_num--;
}
//������ �������� ������� �� �����۰����� ���ҽ�Ű�� �Լ�
bool User::isvalidDirection(int dir)
{
	switch (dir)
	{
	case 0:
		//��
		if (Map::getmap(int(pos_x - 0.5), int(pos_y + 0.5)) == 1 || Map::getmap(int(pos_x + 0.5), int(pos_y + 0.5)) == 1)
			return false;
		return true;
	case 1:
		//��
		if (Map::getmap(int(pos_x + 0.5), int(pos_y + 0.5)) == 1 || Map::getmap(int(pos_x + 0.5), int(pos_y - 0.5)) == 1)
			return false;
		return true;
	case 2:
		//��
		if (Map::getmap(int(pos_x - 0.5), int(pos_y - 0.5)) == 1 || Map::getmap(int(pos_x + 0.5), int(pos_y - 0.5)) == 1)
			return false;
		return true;
	case 3:
		//��
		if (Map::getmap(int(pos_x - 0.5), int(pos_y + 0.5)) == 1 || Map::getmap(int(pos_x - 0.5), int(pos_y - 0.5)) == 1)
			return false;
		return true;
	}
}
//dir �������� ���� �����ϼ� �ִ��� Ȯ���ؼ� �����ϼ� ������ true�� return�Ѵ�






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
}//is_aggro ���õȰſ� ���� ���긦 �Ѵ�. �÷��̾�� ��������� �÷��̾ ���δ�! �Ѿ˿� ���� �״°� �Ѿ�Ŭ������ �־�����Ŵ� ���� ���ʿ�. 
 //�����δ� �������ؾ���. �������� ������. �ð����� �����ϰ� ����������. ������ �����ڰ� �˾Ƽ� ����!
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
//i��° enemy�� �ҷ���
vector<Enemy*> Enemy::get_enemy_list()
{
	return enemy_list;
}
//enemy_list�� �ҷ���
void Enemy::remove_enemy(int i)
{
	enemy_list.erase(enemy_list.begin() + i);
}
//i ��° enemy�� �����ϴ� �Լ�
int Enemy::get_enemy_num()
{
	return enemy_list.size();
}
bool Enemy::isUserNear()
{
	return distance(pos_x, pos_y, User::get_pos_x(), User::get_pos_y()) <= AGGRO_DISTANCE;
}
//������ ����� ��� true�� return�Ѵ�. �Ÿ� ������ AGGRO_DISTANCE���� ����� ��츦 �������� ��
bool Enemy::isUserCollide() {
	return distance(pos_x, pos_y, User::get_pos_x(), User::get_pos_y()) <= DEATH_DISTANCE;
}
//������ �浹�� ��� true�� return�Ѵ�. �Ÿ� ������ DEATH_DISTANCE���� ����� ��츦 �������� ��.
bool Enemy::isvalidDirection(int dir)
{
	switch (dir)
	{
	case 0:
		//��
		if (Map::getmap(int(pos_x - 0.5), int(pos_y + 0.5)) == 1 || Map::getmap(int(pos_x + 0.5), int(pos_y + 0.5)) == 1)
			return false;
		return true;
	case 1:
		//��
		if (Map::getmap(int(pos_x + 0.5), int(pos_y + 0.5)) == 1 || Map::getmap(int(pos_x + 0.5), int(pos_y - 0.5)) == 1)
			return false;
		return true;
	case 2:
		//��
		if (Map::getmap(int(pos_x - 0.5), int(pos_y - 0.5)) == 1 || Map::getmap(int(pos_x + 0.5), int(pos_y - 0.5)) == 1)
			return false;
		return true;
	case 3:
		//��
		if (Map::getmap(int(pos_x - 0.5), int(pos_y + 0.5)) == 1 || Map::getmap(int(pos_x - 0.5), int(pos_y - 0.5)) == 1)
			return false;
		return true;
	}
}
//vector<Enemy*> Enemy::enemy_list = vector<Enemy*>();


Bullet::Bullet() {
	switch (User::get_direction())
	{
	case 0://��
		pos_x = User::get_pos_x();
		pos_y = User::get_pos_y() + 0.3;
		break;
	case 1://��
		pos_x = User::get_pos_x() + 0.3;
		pos_y = User::get_pos_y();
		break;
	case 2://��
		pos_x = User::get_pos_x();
		pos_y = User::get_pos_y() - 0.3;
		break;
	case 3://��
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
int Bullet::move()//enemy�� �ε����� enemy���̰� �Ѿ� �����, ���̶� �ε����� �����
{
	switch (direction)
	{
	case 0://��
		pos_y += BULLET_VELOCITY;
		break;
	case 1://��
		pos_x += BULLET_VELOCITY;
		break;
	case 2://��
		pos_y -= BULLET_VELOCITY;
		break;
	case 3://��
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
}//�Ϲ����� ��Ȳ���� -1�� return, ���� �ε������ -2�� return, enemy�� �ε������� enemy�� index�� return
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