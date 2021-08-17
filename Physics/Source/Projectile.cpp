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
	case PROJECTILE_TYPE::PROJECTILE_SHOTGUN:
		vel = normal * 30;
		damage = 10;
		break;
	case PROJECTILE_TYPE::PROJECTILE_MINIGUN:
		vel = normal * 40;
		damage = 5;
		break;
	}
}

void Projectile::Update(double dt)
{
	if (active)
	{
		prevpos = pos;

		pos += vel * dt * Player::GetInstance()->GetGameSpeed();
	}
}

void Projectile::Render()
{
}

void Projectile::Exit()
{
}

void Projectile::Init(int shotgunID)
{
	hp = 1;
	prevpos = pos;
	scale.Set(1.f, 1.f, 1.f);
	normal = Player::GetInstance()->GetRotate().Normalized();
	if (shotgunID == 1 || shotgunID == 2)
	{
		normal.Set(normal.x * cos(Math::DegreeToRadian(shotgunID * 10)) - normal.y * sin(Math::DegreeToRadian(shotgunID * 10)),
			normal.x * sin(Math::DegreeToRadian(shotgunID * 10)) + normal.y * cos(Math::DegreeToRadian(shotgunID * 10)));
	}
	else if (shotgunID == 4 || shotgunID == 5)
	{
		normal.Set(normal.x * cos(Math::DegreeToRadian((shotgunID - 3) * -10)) - normal.y * sin(Math::DegreeToRadian((shotgunID - 3) * -10)),
			normal.x * sin(Math::DegreeToRadian((shotgunID - 3) * -10)) + normal.y * cos(Math::DegreeToRadian((shotgunID - 3) * -10)));
	}
	pos = Player::GetInstance()->GetPos() + normal * 2;
	active = true;
	vel = normal * 30;
	damage = 10;
}

void Projectile::Movement(double dt)
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