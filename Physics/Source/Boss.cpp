#include "Boss.h"

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
}

void Boss::Update(double dt)
{
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
