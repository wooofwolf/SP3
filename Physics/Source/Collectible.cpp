#include "Collectible.h"

#include "Player.h"

Collectible::Collectible()
{

}

Collectible::~Collectible()
{

}

void Collectible::Init()
{

}

void Collectible::Update(double dt)
{

}

void Collectible::Render()
{

}

void Collectible::Exit()
{

}

void Collectible::Movement(double dt)
{

}

void Collectible::Collisioncheck(std::vector<Vector3> activeTileList)
{
	if (active == true)
	{
		if ((pos - Player::GetInstance()->GetPos()).Length() < scale.x + 2)
		{
			active = false;
			std::cout << "PICKUP DETECTED" << std::endl;
		}
	}
}

Vector3 Collectible::GetPos()
{
	return pos;
}

Vector3 Collectible::GetScale()
{
	return scale;
}

void Collectible::SetPos(Vector3 pos)
{
	this->pos = pos;
}

void Collectible::SetScale(Vector3 scale)
{
	this->scale = scale;
}