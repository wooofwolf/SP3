#include "Projectile.h"
#include "BaseEnemy.h"
#include "Player.h"
#include "Map.h"
#include "SceneManager.h"
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
	detonateTimer = 0;
	explosionTimer = 0;
	active = true;
	BaseEnemy* enemy;
	switch (type)
	{
	case PROJECTILE_TYPE::PROJECTILE_REVOLVER:
		vel = normal * 200;
		damage = 15;
		break;
	case PROJECTILE_TYPE::PROJECTILE_SHOTGUN:
		vel = normal * 130;
		damage = 10;
		break;
	case PROJECTILE_TYPE::PROJECTILE_MINIGUN:
		vel = normal * 150;
		damage = 5;
		break;
	case PROJECTILE_TYPE::PROJECTILE_ABILITY_3:
		vel = normal * 100;
		damage = 10;
		break;
	case PROJECTILE_TYPE::PROJECTILE_ABILITY_3_EXPLOSION:
		vel.SetZero();
		damage = 10;
		scale.Set(10.f, 10.f, 1.f);
		break;
	}
}

void Projectile::Update(double dt)
{
	if (active)
	{
		prevpos = pos;

		if (type == PROJECTILE_TYPE::PROJECTILE_ABILITY_3)
		{
			vel *= 0.9f;
			detonateTimer += dt;
		}
		else if (type == PROJECTILE_TYPE::PROJECTILE_ABILITY_3_EXPLOSION)
		{
			vel.SetZero();
			explosionTimer += dt;
		}

		pos += vel * dt * Player::GetInstance()->GetPlusSpeed();
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
	vel = normal * 130;
	damage = 10;
}

void Projectile::Init(BaseEnemy* enemy)
{
	scale.Set(1, 1, 1);
	hp = 1;
	active = true;
	switch (type)
	{
	case PROJECTILE_TYPE::PROJECTILE_SPEARS:
		damage = 10;
		pos.Set(enemy->GetEnemyPos().x, enemy->GetEnemyPos().y, enemy->GetEnemyPos().z);
		normal.Set(enemy->GetNormal().x, enemy->GetNormal().y, enemy->GetNormal().z);
		vel = normal * 100;
		break;
	case PROJECTILE_TYPE::PROJECTILE_ARROWS:
		damage = 15;
		pos.Set(enemy->GetEnemyPos().x, enemy->GetEnemyPos().y, enemy->GetEnemyPos().z);
		normal.Set(enemy->GetNormal().x, enemy->GetNormal().y, enemy->GetNormal().z);
		vel = normal * 125;
		break;
	case PROJECTILE_TYPE::PROJECTILE_SNIPERBULLET:
		damage = 20;
		pos.Set(enemy->GetEnemyPos().x, enemy->GetEnemyPos().y, enemy->GetEnemyPos().z);
		normal.Set(enemy->GetNormal().x, enemy->GetNormal().y, enemy->GetNormal().z);
		vel = normal * 140;
		break;
	}
}

void Projectile::EnemyShoot(PROJECTILE_TYPE type , BaseEnemy* enemy, double dt)
{
	if (enemy->moving)
	{
		std::cout << Player::GetInstance()->GetPos() << std::endl;
		Projectile* proj = new Projectile(type);
		enemy->active = true;
		proj->Init(enemy);
		proj->active = true;
		SceneManager::GetInstance()->GetActiveScene()->projectileList.push_back(proj);
	}
	else
	{
		enemy->moving = true;
	}
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
			if (type == PROJECTILE_TYPE::PROJECTILE_ABILITY_3 || type == PROJECTILE_TYPE::PROJECTILE_ABILITY_3_EXPLOSION)
			{
				vel.SetZero();
				pos = prevpos;
			}
			else
				active = false;
		}
	}
}

void Projectile::SetVelo(Vector3 vel)
{
	this->vel = vel;
}

Vector3 Projectile::GetPos()
{
	return pos;
}

Vector3 Projectile::GetScale()
{
	return scale;
}

void Projectile::SetPos(Vector3 pos)
{
	this->pos = pos;
}

void Projectile::SetScale(Vector3 scale)
{
	this->scale = scale;
}
