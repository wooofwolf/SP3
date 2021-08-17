#include "Projectile.h"

#include "Player.h"

Projectile::Projectile()
{
}

Projectile::Projectile(PROJECTILE_TYPE type) : type(type)
{
}

Projectile::~Projectile()
{
}

void Projectile::Init()
{
	hp = 1;
	prevpos = pos;
	scale.Set(1.f, 1.f, 1.f);
	normal = Player::GetInstance()->GetRotate().Normalized();
	pos = Player::GetInstance()->GetPos() + normal * 2;
	active = true;

	switch (type)
	{
	case PROJECTILE_TYPE::PROJECTILE_REVOLVER:
		vel = normal * 50;
		damage = 10;
		break;
	}
}

void Projectile::Update(double dt)
{
	if (active)
	{
		prevpos = pos;

		pos += vel * dt;
	}
}

void Projectile::Render()
{
}

void Projectile::Exit()
{
}

void Projectile::Movement()
{
}

void Projectile::Collisioncheck(std::vector<Vector3> activeTileList)
{
	for (Vector3 tilepos : activeTileList)
	{
		if ((pos - tilepos).Length() < scale.x + 2)
		{
			active = false;
		}
	}
}

Vector3 Projectile::GetPos()
{
	return pos;
}

Vector3 Projectile::GetScale()
{
	return scale;
}