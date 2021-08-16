#include "BaseEnemy.h"
#include <MyMath.h>
BaseEnemy::~BaseEnemy()
{
}

void BaseEnemy::Init()
{
	vel = 0.7;
	active = true;
	pos = Vector3(10, 90, 0);
}

void BaseEnemy::Update(double dt)
{
	if (!active)
		return;
	Movement();
}

void BaseEnemy::Render()
{
}

void BaseEnemy::Movement()
{
	directionalCD = Math::RandIntMinMax(4, 7);
	direction = Math::RandIntMinMax(1, 4);
	if (directionalCD > 0)
	{
		directionalCD--;
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

void ConcreteBaseEnemy::Attack() const
{
	std::cout << "ConcreteBaseEnemy Attack!!" << std::endl;
}

EnemyDecorator::EnemyDecorator(BaseEnemy* baseEnemy): baseEnemy(baseEnemy)
{
}

void EnemyDecorator::Attack() const
{
	std::cout << "EnemyDecorator Attack!!" << std::endl;
}

CavemenMelee::CavemenMelee(BaseEnemy* baseEnemy) : EnemyDecorator(baseEnemy)
{
}

void CavemenMelee::Attack() const
{
	std::cout << "Cavemen Melee Attack!!" << std::endl;
}

CavemenRanged::CavemenRanged(BaseEnemy* baseEnemy) : EnemyDecorator(baseEnemy)
{
}

void CavemenRanged::Attack() const
{
	std::cout << "Cavemen Ranged Attack!!" << std::endl;
}

Knights::Knights(BaseEnemy* baseEnemy) : EnemyDecorator(baseEnemy)
{
}

void Knights::Attack() const
{
	std::cout << "Knights Attack!!" << std::endl;
}

Archer::Archer(BaseEnemy* baseEnemy) : EnemyDecorator(baseEnemy)
{
}

void Archer::Attack() const
{
	std::cout << "Archer Attack!!" << std::endl;
}

Cyborg::Cyborg(BaseEnemy* baseEnemy) : EnemyDecorator(baseEnemy)
{
}

void Cyborg::Attack() const
{
	std::cout << "Cyborg Attack!!" << std::endl;
}

Sniper::Sniper(BaseEnemy* baseEnemy) : EnemyDecorator(baseEnemy)
{
}

void Sniper::Attack() const
{
	std::cout << "Sniper Attack!!" << std::endl;
}
