#include "BaseEnemy.h"

#include <MyMath.h>
#include <future>

#include "Application.h"
#include "Map.h"
#include "Player.h" 
#include "SceneBase.h"

BaseEnemy::~BaseEnemy()
{
}

void BaseEnemy::Init()
{
	active = true;
	hp = 0;
	vel.Set(0, 0, 0);
	scale.Set(3, 3, 3);
	normal.Set(0, 1, 0);
	prevpos = pos;
	Math::InitRNG();
	currentFSM = IDLE;
	moving = false;
	dest.SetZero();
	movingTimer = 0;
}

void BaseEnemy::Update(double dt)
{
	/*std::cout << "IM BEING USED" << std::endl;
	switch (currentFSM)
	{
		case IDLE:
		{
			Movement(dt);
			if ((pos- Player::GetInstance()->GetPos()).Length() < 5)
			{
				currentFSM = ATTACK;
			}
			std::cout << "lalala" << std::endl;
			break;
		}
		case ATTACK:
		{
			std::cout << "Kiil me" << std::endl;
			break;
		}
	}*/

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
				pos.x += 10 * dt;
				direction = RIGHT;
				break;
			}
			case RIGHT:
			{
				pos = prevpos;
				pos.x -= 10 * dt;
				direction = LEFT;
				break;
			}
			case UP:
			{
				pos = prevpos;
				pos.y -= 10 * dt;
				direction = DOWN;
				break;
			}
			case DOWN:
			{
				pos = prevpos;
				pos.y += 10 * dt;
				direction = UP;
				break;
			}
		}
		collided = false;
	}
}
void BaseEnemy::Render()
{
}

void BaseEnemy::Movement(double dt)
{
	if (moving)
	{
		if (!pathfinding && !path.empty())
		{
			dest = Vector3(2 + 4 * path.back().x, Map::GetInstance()->m_worldHeight - 2 - 4 * path.back().y, 1);
			path.pop_back();
			pathfinding = true;
		}
		if (pathfinding)
		{
			if (pos == dest)
			{
				path.pop_back();
				pathfinding = false;
			}
			if ((dest - pos).Length() > 0.2f)
				pos += (dest - pos).Normalized() * dt * 10;
			else
			{
				pathfinding = false;
			}
		}
	}
}

void BaseEnemy::UpdateDirection()
{
	// Set the destination to the Player::GetInstance()
	destination = Player::GetInstance()->GetPos();

	// Calculate the direction between enemy2D and Player::GetInstance()2D
	pathDir = destination - pos;

	// Calculate the distance between enemy2D and Player::GetInstance()2D
	float fDistance = (destination-pos).Length();
	if (fDistance >= 0.01f)
	{
		// Calculate direction vector.
		// We need to round the numbers as it is easier to work with whole numbers for movements
		pathDir.x = (float)round(pathDir.x / fDistance);
		pathDir.y = (float)round(pathDir.y / fDistance);
	}
	else
	{
		// Since we are not going anywhere, set this to 0.
		pathDir = 0;
	}
}

void BaseEnemy::UpdatePosition()
{
	// Store the old position
	prevpos = pos;
	//std::cout << "Path Direction: "<<pathDir << std::endl;
	
	// if the Player::GetInstance() is to the left or right of the enemy2D, then jump to attack
	if (pathDir.x < 0)
	{
		// Move left
		const int previousposition = pos.x;
		if (pos.x >= 0)
		{
			pos.x--;
		}
	}
	else if (pathDir.x > 0)
	{
		// Move right
		const int previousposition = pos.x;
		if (pos.x < 200)
		{
			pos.x++;
		}
	}
	// if the Player::GetInstance() is above the enemy2D, then jump to attack
	if (pathDir.y > 0)
	{
		const int previousposition = pos.y;
		if (pos.y >= 0)
		{
			pos.y++;
		}
	}
	if (pathDir.y < 200)
	{
		const int previousposition = pos.y;
		if (pos.y < 200)
		{
			pos.y--;
		}
	}
}

Vector3 BaseEnemy::GetEnemyPos()
{
	return pos;
}

void BaseEnemy::SetEnemyPos(Vector3 pos)
{
	this->pos = pos;
}

float BaseEnemy::GetEnemyHP()
{
	return hp;
}

void BaseEnemy::SetEnemyHP(int hp)
{
	this->hp = hp;
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

Vector3 BaseEnemy::GetScale()
{
	return scale;	
}

Vector3 BaseEnemy::GetNormal()
{
	return normal;
}

void BaseEnemy::Collisioncheck(std::vector<Vector3> activeTileList)
{
	for (Vector3 tilePos : activeTileList)
	{
		if ((pos - tilePos).Length() < scale.x + 5)
		{
			collided = true;
		}
	}
}

void BaseEnemy::Exit()
{
}
void ConcreteBaseEnemy::Update(double dt)
{

}

void ConcreteBaseEnemy::Init()
{
	scale.Set(3, 3, 3);
}

EnemyDecorator::EnemyDecorator(BaseEnemy* baseEnemy): baseEnemy(baseEnemy)
{
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

void CavemenMelee::Attack()
{
	Player::GetInstance()->SetHealth(Player::GetInstance()->GetHealth() - 5);
	std::cout << "Cavemen Melee Attack!!" << std::endl;
}

void CavemenMelee::Update(double dt)
{
	if (hp <= 0)
	{
		active = false;
	}
	if (!moving)
	{
		stasisTimer += dt;
		std::cout << stasisTimer << std::endl;
	}
	if (stasisTimer >= 5)
	{
		moving = false;
	}
	//Vector3(2 + 4 * x, m_worldHeight - 2 - 4 * y, 1)
	//Vector3 unOffsetPlayerPos = ((Player::GetInstance()->GetPos().x - 2) / 4, (Player::GetInstance()->GetPos().y - map->m_worldHeight + 2) / -4);
	if (attacked)
	{
		attackCD = 5;
		attacked = false;
	}
	attackCD -= dt;
	//std::cout << attackCD << std::endl;
	//std::cout << "IM BEING USED" << std::endl;
	if (moving)
	{
		switch (currentFSM)
		{
		case IDLE:
		{
			//std::cout << "IDLING" << std::endl;
			//Movement(dt);
			if ((pos - Player::GetInstance()->GetPos()).Length() < 50 && attackCD <= 0)
			{
				currentFSM = PATHFIND;
			}
			/*std::cout << "lalala" << std::endl;*/
			break;
		}
		case ATTACK:
		{
			Attack();
			attacked = true;
			currentFSM = IDLE;
			break;
		}
		case PATHFIND:
		{
			path.clear();
			Map* map = Map::GetInstance();
			Player* p = Player::GetInstance();
			std::future<void> future = std::async(std::launch::async, &Map::AStarSearch, map, pos, p->GetPos(), &path);
			currentFSM = MOVE;
			break;
		}
		case MOVE:
		{
			Movement(dt);
			if ((pos - Player::GetInstance()->GetPos()).Length() < 10)
			{
				currentFSM = ATTACK;
			}
			else if (path.empty())
			{
				currentFSM = IDLE;
			}
			break;
		}
		}
	}

}

void CavemenMelee::Init()
{
	active = true;
	hp = 50;
	currentFSM = IDLE;
	attackCD = 0;
	bPathFind = true;
	attacked = false;
	pathfinding = false;
	dest.SetZero();
	movingTimer = 0;
}

CavemenRanged::CavemenRanged(BaseEnemy* baseEnemy) : EnemyDecorator(baseEnemy)
{
}

void CavemenRanged::Update(double dt)
{

	if (hp <= 0)
	{
		active = false;
	}
	if (!moving)
	{
		stasisTimer += dt;
		std::cout << stasisTimer << std::endl;
	}
	if (stasisTimer >= 5)
	{
		moving = false;
	}
	//Vector3(2 + 4 * x, m_worldHeight - 2 - 4 * y, 1);
	//Vector3 unOffsetPlayerPos = ((Player::GetInstance()->GetPos().x - 2) / 4, (Player::GetInstance()->GetPos().y - map->m_worldHeight + 2) / -4);
	if (attacked)
	{
		attackCD = 5;
		attacked = false;
	}
	attackCD -= dt;
	//std::cout << attackCD << std::endl;
	//std::cout << "IM BEING USED" << std::endl;
	if (moving)
	{
		switch (currentFSM)
		{
		case IDLE:
		{
			if ((pos - Player::GetInstance()->GetPos()).Length() < 75)
			{
				Vector3 shootingDir = (Player::GetInstance()->GetPos() - pos).Normalized();
				normal.Set(shootingDir.x, shootingDir.y, shootingDir.z);
				//std::cout << "IDLING" << std::endl;
				//Movement(dt);
				if (attackCD <= 0)
				{
					currentFSM = MOVE;
				}
			}
			/*std::cout << "lalala" << std::endl;*/
			break;
		}
		case ATTACK:
		{
			std::cout << "FUCK" << std::endl;
			Projectile* proj = new Projectile();
			proj->EnemyShoot(Projectile::PROJECTILE_TYPE::PROJECTILE_SPEARS,this);
			currentFSM = IDLE;
			attacked = true;
			break;
		}
		case MOVE:
		{
			Vector3 shootingDir = (Player::GetInstance()->GetPos() - pos).Normalized();
			normal.Set(shootingDir.x, shootingDir.y, shootingDir.z);
			//Map* map = Map::GetInstance();
			//Player* p = Player::GetInstance();
			//std::future<void> future = std::async(std::launch::async, &Map::AStarSearch, map, pos, p->GetPos(), &path);
		/*	Movement(dt);*/
			if ((pos - Player::GetInstance()->GetPos()).Length() < 50)
			{
				currentFSM = ATTACK;
			}
			break;
		}
		}
	}

}

void CavemenRanged::Init()
{
	active = true;
	hp = 30;
	currentFSM = IDLE;
	attackCD = 0;
	bPathFind = true;
	attacked = false;
}

Knights::Knights(BaseEnemy* baseEnemy) : EnemyDecorator(baseEnemy)
{

}

void Knights::Attack()
{
	Player::GetInstance()->SetHealth(Player::GetInstance()->GetHealth() - 15);
	std::cout << "Knights Attack!!" << std::endl;
}

void Knights::Update(double dt)
{
	if (hp <= 0)
	{
		active = false;
	}
	if (!moving)
	{
		stasisTimer += dt;
		std::cout << stasisTimer << std::endl;
	}
	if (stasisTimer >= 5)
	{
		moving = false;
	}
	//Vector3(2 + 4 * x, m_worldHeight - 2 - 4 * y, 1)
	//Vector3 unOffsetPlayerPos = ((Player::GetInstance()->GetPos().x - 2) / 4, (Player::GetInstance()->GetPos().y - map->m_worldHeight + 2) / -4);
	if (attacked)
	{
		attackCD = 5;
		attacked = false;
	}
	attackCD -= dt;
	//std::cout << attackCD << std::endl;
	//std::cout << "IM BEING USED" << std::endl;
	if (moving)
	{
		switch (currentFSM)
		{
		case IDLE:
		{
			//std::cout << "IDLING" << std::endl;
			//Movement(dt);
			if ((pos - Player::GetInstance()->GetPos()).Length() < 50 && attackCD <= 0)
			{
				currentFSM = PATHFIND;
			}
			/*std::cout << "lalala" << std::endl;*/
			break;
		}
		case ATTACK:
		{
			Attack();
			attacked = true;
			currentFSM = IDLE;
			break;
		}
		case PATHFIND:
		{

			Map* map = Map::GetInstance();
			Player* p = Player::GetInstance();
			std::future<void> future = std::async(std::launch::async, &Map::AStarSearch, map, pos, p->GetPos(), &path);
			currentFSM = MOVE;
			break;
		}
		case MOVE:
		{
			Movement(dt);
			if ((pos - Player::GetInstance()->GetPos()).Length() < 10)
			{
				currentFSM = ATTACK;
			}
			break;
		}
		}
	}
}

void Knights::Init()
{
	active = true;
	hp = 80;
	currentFSM = IDLE;
	attackCD = 0;
	bPathFind = true;
	attacked = false;
}

Archer::Archer(BaseEnemy* baseEnemy) : EnemyDecorator(baseEnemy)
{
}

void Archer::Update(double dt)
{
	if (hp <= 0)
	{
		active = false;
	}
	if (!moving)
	{
		stasisTimer += dt;
		std::cout << stasisTimer << std::endl;
	}
	if (stasisTimer >= 5)
	{
		moving = false;
	}
	//Vector3(2 + 4 * x, m_worldHeight - 2 - 4 * y, 1);
	//Vector3 unOffsetPlayerPos = ((Player::GetInstance()->GetPos().x - 2) / 4, (Player::GetInstance()->GetPos().y - map->m_worldHeight + 2) / -4);
	if (attacked)
	{
		attackCD = 5;
		attacked = false;
	}
	attackCD -= dt;
	//std::cout << attackCD << std::endl;
	//std::cout << "IM BEING USED" << std::endl;
	if (moving)
	{
		switch (currentFSM)
		{
		case IDLE:
		{
			if ((pos - Player::GetInstance()->GetPos()).Length() < 75)
			{
				Vector3 shootingDir = (Player::GetInstance()->GetPos() - pos).Normalized();
				normal.Set(shootingDir.x, shootingDir.y, shootingDir.z);
				//std::cout << "IDLING" << std::endl;
				//Movement(dt);
				if (attackCD <= 0)
				{
					currentFSM = MOVE;
				}
			}
			/*std::cout << "lalala" << std::endl;*/
			break;
		}
		case ATTACK:
		{
			std::cout << "FUCK" << std::endl;
			Projectile* proj = new Projectile();
			proj->EnemyShoot(Projectile::PROJECTILE_TYPE::PROJECTILE_ARROWS, this);
			currentFSM = IDLE;
			attacked = true;
			break;
		}
		case MOVE:
		{
			Vector3 shootingDir = (Player::GetInstance()->GetPos() - pos).Normalized();
			normal.Set(shootingDir.x, shootingDir.y, shootingDir.z);
			//Map* map = Map::GetInstance();
			//Player* p = Player::GetInstance();
			//std::future<void> future = std::async(std::launch::async, &Map::AStarSearch, map, pos, p->GetPos(), &path);
		/*	Movement(dt);*/
			if ((pos - Player::GetInstance()->GetPos()).Length() < 50)
			{
				currentFSM = ATTACK;
			}
			break;
		}
		}
	}
}

void Archer::Init()
{
	active = true;
	hp = 60;
	currentFSM = IDLE;
	attackCD = 0;
	bPathFind = true;
	attacked = false;
}

Cyborg::Cyborg(BaseEnemy* baseEnemy) : EnemyDecorator(baseEnemy)
{

}

void Cyborg::Attack()
{
	Player::GetInstance()->SetHealth(Player::GetInstance()->GetHealth() - 25);
	std::cout << "Cyborg Attack!!" << std::endl;
}

void Cyborg::Update(double dt)
{
	if (hp <= 0)
	{
		active = false;
	}
	if (!moving)
	{
		stasisTimer += dt;
		std::cout << stasisTimer << std::endl;
	}
	if (stasisTimer >= 5)
	{
		moving = false;
	}
	//Vector3(2 + 4 * x, m_worldHeight - 2 - 4 * y, 1)
	//Vector3 unOffsetPlayerPos = ((Player::GetInstance()->GetPos().x - 2) / 4, (Player::GetInstance()->GetPos().y - map->m_worldHeight + 2) / -4);
	if (attacked)
	{
		attackCD = 5;
		attacked = false;
	}
	attackCD -= dt;
	//std::cout << attackCD << std::endl;
	//std::cout << "IM BEING USED" << std::endl;
	if (moving)
	{
		switch (currentFSM)
		{
		case IDLE:
		{
			//std::cout << "IDLING" << std::endl;
			//Movement(dt);
			if ((pos - Player::GetInstance()->GetPos()).Length() < 50 && attackCD <= 0)
			{
				currentFSM = PATHFIND;
			}
			/*std::cout << "lalala" << std::endl;*/
			break;
		}
		case ATTACK:
		{
			Attack();
			attacked = true;
			currentFSM = IDLE;
			break;
		}
		case PATHFIND:
		{

			Map* map = Map::GetInstance();
			Player* p = Player::GetInstance();
			std::future<void> future = std::async(std::launch::async, &Map::AStarSearch, map, pos, p->GetPos(), &path);
			currentFSM = MOVE;
			break;
		}
		case MOVE:
		{
			Movement(dt);
			if ((pos - Player::GetInstance()->GetPos()).Length() < 10)
			{
				currentFSM = ATTACK;
			}
			break;
		}
		}
	}
}

void Cyborg::Init()
{
	active = true;
	hp = 110;
	currentFSM = IDLE;
	attackCD = 0;
	bPathFind = true;
	attacked = false;
}

Sniper::Sniper(BaseEnemy* baseEnemy) : EnemyDecorator(baseEnemy)
{
}

void Sniper::Update(double dt)
{
	if (hp <= 0)
	{
		active = false;
	}
	if (!moving)
	{
		stasisTimer += dt;
		std::cout << stasisTimer << std::endl;
	}
	if (stasisTimer >= 5)
	{
		moving = false;
	}
	//Vector3(2 + 4 * x, m_worldHeight - 2 - 4 * y, 1);
	//Vector3 unOffsetPlayerPos = ((Player::GetInstance()->GetPos().x - 2) / 4, (Player::GetInstance()->GetPos().y - map->m_worldHeight + 2) / -4);
	if (attacked)
	{
		attackCD = 5;
		attacked = false;
	}
	attackCD -= dt;
	//std::cout << attackCD << std::endl;
	//std::cout << "IM BEING USED" << std::endl;
	if (moving)
	{
		switch (currentFSM)
		{
		case IDLE:
		{
			if ((pos - Player::GetInstance()->GetPos()).Length() < 75)
			{
				Vector3 shootingDir = (Player::GetInstance()->GetPos() - pos).Normalized();
				normal.Set(shootingDir.x, shootingDir.y, shootingDir.z);
				//std::cout << "IDLING" << std::endl;
				//Movement(dt);
				if (attackCD <= 0)
				{
					currentFSM = MOVE;
				}
			}
			/*std::cout << "lalala" << std::endl;*/
			break;
		}
		case ATTACK:
		{
			std::cout << "FUCK" << std::endl;
			Projectile* proj = new Projectile();
			proj->EnemyShoot(Projectile::PROJECTILE_TYPE::PROJECTILE_SNIPERBULLET, this);	
			currentFSM = IDLE;
			attacked = true;
			break;
		}
		case MOVE:
		{
			Vector3 shootingDir = (Player::GetInstance()->GetPos() - pos).Normalized();
			normal.Set(shootingDir.x, shootingDir.y, shootingDir.z);
			//Map* map = Map::GetInstance();
			//Player* p = Player::GetInstance();
			//std::future<void> future = std::async(std::launch::async, &Map::AStarSearch, map, pos, p->GetPos(), &path);
		/*	Movement(dt);*/
			if ((pos - Player::GetInstance()->GetPos()).Length() < 50)
			{
				currentFSM = ATTACK;
			}
			break;
		}
		}
	}
}

void Sniper::Init()
{
	active = true;
	hp = 80;
	currentFSM = IDLE;
	attackCD = 0;
	bPathFind = true;
	attacked = false;
}
