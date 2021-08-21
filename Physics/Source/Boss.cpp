#include "Boss.h"

#include "Player.h"

Boss::Boss()
{
}

Boss::~Boss()
{
}

void Boss::Init()
{
	hp = 1000;
	pos.SetZero();
	prevpos = pos;
	vel.SetZero();
	scale.Set(10.f, 10.f, 1.f);
	normal.SetZero();
	active = true;

	bossFightActive = false;
}

void Boss::Update(double dt)
{
	if ((Player::GetInstance()->GetPos() - pos).Length() <= 50.f)
	{
		bossFightActive = true;
	}
}

void Boss::Render()
{
}

void Boss::Exit()
{
}

void Boss::Movement(double dt)
{
}

void Boss::Collisioncheck(std::vector<Vector3> activeTileList)
{
}

void Boss::SetPos(Vector3 pos)
{
	this->pos = pos;
}

Vector3 Boss::GetPos()
{
	return pos;
}

Vector3 Boss::GetScale()
{
	return scale;
}

bool Boss::GetFightActive()
{
	return bossFightActive;
}

float Boss::GetHP()
{
	return hp;
}

void Boss::SetHP(float hp)
{
	this->hp = hp;
}
