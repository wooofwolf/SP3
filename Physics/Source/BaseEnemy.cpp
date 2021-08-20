#include "BaseEnemy.h"
#include <MyMath.h>
#include "Application.h"
#include "Map.h"
BaseEnemy::~BaseEnemy()
{
}

void BaseEnemy::Init()
{
	active = true;
	hp = 0;
	vel.Set(0, 0, 0);
	normal.Set(0, 1, 0);
	prevpos = pos;
	Math::InitRNG();
	currentFSM = IDLE;
	player = Player::GetInstance();
}

void BaseEnemy::Update(double dt)
{
	/*std::cout << "IM BEING USED" << std::endl;
	switch (currentFSM)
	{
		case IDLE:
		{
			Movement(dt);
			if ((pos- player->GetPos()).Length() < 5)
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
	prevpos = pos;
	if (directionalCD > 0)
	{
		directionalCD-= dt;
		
		switch (direction)
		{
			case LEFT:
			{
				pos.x -= 10 * dt * Player::GetInstance()->GetSlowSpeed();
				CheckCollidedSide(LEFT, dt);
				break;
			}
			case UP:
			{
				pos.y += 10 * dt * Player::GetInstance()->GetSlowSpeed();
				CheckCollidedSide(UP, dt);
				break;
			}
			case RIGHT:
			{
				pos.x += 10 * dt * Player::GetInstance()->GetSlowSpeed();
				CheckCollidedSide(RIGHT, dt);
				break;
			}
			case DOWN:
			{
				pos.y -= 10 * dt * Player::GetInstance()->GetSlowSpeed();
				CheckCollidedSide(DOWN, dt);
				break;
			}
		}
	}
	else if (directionalCD <= 0)
	{
		this->direction = static_cast<DIRECTION>(Math::RandIntMinMax(0, 3));
		directionalCD = 3;
	}
	/*if (Application::IsKeyPressed(0x25))
	{
		pos.x -= 10 * dt;
		CheckCollidedSide(LEFT, dt);
	}
	else if (Application::IsKeyPressed(0x27))
	{
		pos.x += 10 * dt;
		CheckCollidedSide(RIGHT, dt);
	}
	else if (Application::IsKeyPressed(0x26))
	{
		pos.y += 10 * dt;
		CheckCollidedSide(UP, dt);
	}
	else if (Application::IsKeyPressed(0x28))
	{
		pos.y -= 10 * dt;
		CheckCollidedSide(DOWN, dt);
	}*/
}

void BaseEnemy::UpdateDirection()
{
	// Set the destination to the player
	destination = player->GetPos();

	// Calculate the direction between enemy2D and player2D
	pathDir = destination - pos;

	// Calculate the distance between enemy2D and player2D
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
	std::cout << "Path Direction: "<<pathDir << std::endl;
	
	// if the player is to the left or right of the enemy2D, then jump to attack
	if (pathDir.x < 0)
	{
		// Move left
		const int previousposition = pos.x;
		if (pos.x >= 0)
		{
			pos.x--;
		}
		//// Constraint the enemy2D's position within the screen boundary
		//CheckCollidedSide(LEFT,dt);
		//// Find a feasible position for the enemy2D's current position
		//if (CheckCollidedSide(LEFT,dt) == false)
		//{
		//	FlipHorizontalDirection();
		//	i32vec2Index = i32vec2OldIndex;
		//	i32vec2NumMicroSteps.x = 0;
		//}
		//// Check if enemy2D is in mid-air, such as walking off a platform
		//if (IsMidAir() == true)
		//{
		//	cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);
		//}
		////CS: Play the "left" animation
		//animatedSprites->PlayAnimation("left", -1, 1.0f);
		// Interact with the Player
	}
	else if (pathDir.x > 0)
	{
		// Move right
		const int previousposition = pos.x;
		if (pos.x < 200)
		{
			pos.x++;
		}
		//// Constraint the enemy2D's position within the screen boundary
		//Constraint(RIGHT);
		//// Find a feasible position for the enemy2D's current position
		//if (CheckPosition(RIGHT) == false)
		//{
		//	FlipHorizontalDirection();
		//	i32vec2Index = i32vec2OldIndex;
		//	i32vec2NumMicroSteps.x = 0;
		//}
		//// Check if enemy2D is in mid-air, such as walking off a platform
		//if (IsMidAir() == true)
		//{
		//	cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);
		//}
		////CS: Play the "right" animation
		//animatedSprites->PlayAnimation("right", -1, 1.0f);
		// Interact with the Player
	}
	// if the player is above the enemy2D, then jump to attack
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
		if ((pos - tilePos).Length() < scale.x + 5)
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
	//Vector3(2 + 4 * x, m_worldHeight - 2 - 4 * y, 1)
	Map* map = Map::GetInstance();
	//Vector3 unOffsetPlayerPos = ((player->GetPos().x - 2) / 4, (player->GetPos().y - map->m_worldHeight + 2) / -4);
	if (attacked)
	{
		attackCD = 5;
		attacked = false;
	}
	attackCD -= dt;
	//std::cout << attackCD << std::endl;
	//std::cout << "IM BEING USED" << std::endl;
	switch (currentFSM)		
	{
		case IDLE:
		{
			//std::cout << "IDLING" << std::endl;
			Movement(dt);
			if ((pos - player->GetPos()).Length() < 25 && attackCD <= 0)
			{
				currentFSM = ATTACK;
				bPathFind = true;
			}
			/*std::cout << "lalala" << std::endl;*/
			break;
		}
		case ATTACK:
		{
			// Check if enemy has reached the destination from the last pathfind
			// Else if never reach the last pathfind, then recalculate the path
			if (bPathFind == true)
			{
				//std::cout << "ATTACK" << std::endl;
				/*std::cout << "Kill me" << std::endl;*/
				auto path = map->PathFind(pos, player->GetPos(), heuristic::euclidean);
				/*		std::cout << "=== Printing out the path ===" << std::endl;*/
						// Calculate new destination
				bool bFirstPosition = true;
				for (const auto& coord : path)
				{
					if (bFirstPosition == true)
					{
						// Set a destination
						destination = coord;
						// Calculate the direction between enemy2D and this destination
						pathDir = destination - pos;
						bFirstPosition = false;
						}
					else
					{
						if ((coord - destination) == pathDir)
						{
							// Set a destination
							destination = coord;
						}
						else
							break;
					}
				}
				bPathFind = false;
			}
			if (pos == destination && player->GetPos() != pos)
			{
				bPathFind = true;
			}
			UpdatePosition();
			if ((player->GetPos() - pos).Length() < 5)
			{
				Attack();
				attacked = true;
			}
			
			if ((pos - player->GetPos()).Length() < 25)
			{
				currentFSM = IDLE;
			}
			break;
		}
	}
}

void CavemenMelee::Init()
{
	active = true;
	hp = 10;
	currentFSM = IDLE;
	player = Player::GetInstance();
	attackCD = 0;
	bPathFind = true;
	attacked = false;
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
