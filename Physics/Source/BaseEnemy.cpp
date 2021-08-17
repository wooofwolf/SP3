#include "BaseEnemy.h"
#include <MyMath.h>
BaseEnemy::~BaseEnemy()
{
}

void BaseEnemy::Init()
{
	active = false;
	hp = 0;
	pos.Set(50, 60, 1);
	vel.Set(0, 0, 0);
	normal.Set(0, 1, 0);
	prevpos = pos;
}

void BaseEnemy::Update(double dt)
{
	Movement(dt);
}

void BaseEnemy::CheckCollidedSide(DIRECTION direction , double dt)
{
	if (collided == true)
	{
		switch (direction)
		{
		case LEFT:
		{
			pos = prevpos;
			pos.x -= 10 * dt;	
		}
		case RIGHT:
		{
			pos = prevpos;
			pos.x += 10 * dt;
		}
		case UP:
		{
			pos = prevpos;
			pos.y += 10 * dt;
		}
		case DOWN:
		{
			pos = prevpos;
			pos.y -= 10 * dt;
		}
		}
	}
}

void BaseEnemy::Render()
{
}

void BaseEnemy::Movement(double dt)
{
	prevpos = pos;
	if (directionalCD > 0)
	{
		directionalCD-= dt;
		if (direction == 1)
		{
			pos.x += 10 * dt;
			CheckCollidedSide(LEFT, dt);
		}
		else if (direction == 2)
		{
			pos.y += 10 * dt;
			CheckCollidedSide(UP, dt);
		}
		else if (direction == 3)
		{
			pos.x += -10 * dt;
			CheckCollidedSide(RIGHT, dt);
		}
		else if (direction == 4)
		{
			pos.y += -10 * dt;
			CheckCollidedSide(DOWN, dt);
		}
	}
	else if (directionalCD <= 0)
	{
		direction = Math::RandIntMinMax(1, 4);
		directionalCD = 3;
	}
}

Vector3 BaseEnemy::GetEnemyPos()
{
	return pos;
}

bool BaseEnemy::GetActive()
{
	if (active == true)
	{
		return true;
	}
	else
		return false;
}

void BaseEnemy::Collisioncheck(std::vector<Vector3> activeTileList)
{
	for (Vector3 tilePos : activeTileList)
	{
		if ((pos - tilePos).Length() < scale.x + 2)
		{
			collided = true;
		}
	}
}

void BaseEnemy::Exit()
{
}

void ConcreteBaseEnemy::Attack() const
{
	std::cout << "ConcreteBaseEnemy Attack!!" << std::endl;
}

void ConcreteBaseEnemy::Update(double dt)
{
}

void ConcreteBaseEnemy::Init()
{
}

EnemyDecorator::EnemyDecorator(BaseEnemy* baseEnemy): baseEnemy(baseEnemy)
{
}

void EnemyDecorator::Attack() const
{
	std::cout << "EnemyDecorator Attack!!" << std::endl;
}

void EnemyDecorator::Update(double dt)
{
}

void EnemyDecorator::Init()
{
}

CavemenMelee::CavemenMelee(BaseEnemy* baseEnemy) : EnemyDecorator(baseEnemy)
{
}

void CavemenMelee::Attack() const
{
	std::cout << "Cavemen Melee Attack!!" << std::endl;
}

void CavemenMelee::Update(double dt)
{
	prevpos.Set(pos.x,pos.y,pos.z);
	if (directionalCD > 0)
	{
		baseEnemy->directionalCD-= dt;
		if (direction == 1)
		{
			pos.x += 10 * dt;
			CheckCollidedSide(LEFT, dt);
		}
		else if (direction == 2)
		{
			pos.y += 10 * dt;
			CheckCollidedSide(UP, dt);
		}
		else if (direction == 3)
		{
			pos.x += -10 * dt;
			CheckCollidedSide(RIGHT, dt);
		}
		else if (direction == 4)
		{
			pos.y += -10 * dt;
			CheckCollidedSide(DOWN, dt);
		}
	}
	else if (directionalCD <= 0)
	{
		direction = Math::RandIntMinMax(1, 4);
		directionalCD = 3;
	}
}

void CavemenMelee::Init()
{
	pos.Set(50, 60, 1);
	hp = 10;

}

CavemenRanged::CavemenRanged(BaseEnemy* baseEnemy) : EnemyDecorator(baseEnemy)
{
}

void CavemenRanged::Attack() const
{
	std::cout << "Cavemen Ranged Attack!!" << std::endl;
}

void CavemenRanged::Update(double dt)
{
}

void CavemenRanged::Init()
{
}

Knights::Knights(BaseEnemy* baseEnemy) : EnemyDecorator(baseEnemy)
{
}

void Knights::Attack() const
{
	std::cout << "Knights Attack!!" << std::endl;
}

void Knights::Update(double dt)
{
}

void Knights::Init()
{
}

Archer::Archer(BaseEnemy* baseEnemy) : EnemyDecorator(baseEnemy)
{
}

void Archer::Attack() const
{
	std::cout << "Archer Attack!!" << std::endl;
}

void Archer::Update(double dt)
{
}

void Archer::Init()
{
}

Cyborg::Cyborg(BaseEnemy* baseEnemy) : EnemyDecorator(baseEnemy)
{
}

void Cyborg::Attack() const
{
	std::cout << "Cyborg Attack!!" << std::endl;
}

void Cyborg::Update(double dt)
{
}

void Cyborg::Init()
{
}

Sniper::Sniper(BaseEnemy* baseEnemy) : EnemyDecorator(baseEnemy)
{
}

void Sniper::Attack() const
{
	std::cout << "Sniper Attack!!" << std::endl;
}

void Sniper::Update(double dt)
{
}

void Sniper::Init()
{
}
