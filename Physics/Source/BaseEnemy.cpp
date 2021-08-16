#include "BaseEnemy.h"
#include <MyMath.h>
BaseEnemy::BaseEnemy()
	: Damage(0)
	, map(NULL)
{
}

BaseEnemy::~BaseEnemy()
{
}

void BaseEnemy::Init()
{
	Damage = 5;
	vel = 0.7;
	active = true;
	pos = Vector3(10, 90, 0);
}

void BaseEnemy::Update(double dt)
{
	if (!active)
		return;
}

void BaseEnemy::Patrol()
{
	directionalCD = Math::RandIntMinMax(4, 7);
	direction = Math::RandIntMinMax(1, 4);
	if (directionalCD > 0)
	{
		directionalCD --;
		if (direction == 1)
		{
			pos.x += 1;
		}
		else if (direction == 2)
		{
			pos.y += 1;
		}
		else if (direction == 3)
		{
			pos.x -= 1;
		}
		else if (direction == 4)
		{
			pos.y -= 1;
		}
	}
	else if (directionalCD <= 0)
	{
		direction = Math::RandIntMinMax(1, 4);
		directionalCD = Math::RandIntMinMax(4, 7);
	}
}

Vector3 BaseEnemy::GetEnemyPos()
{
	return pos;
}
