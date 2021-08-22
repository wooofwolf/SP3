#include "Boss.h"

#include "Player.h"
#include "BaseEnemy.h"
#include "SceneManager.h"

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

	bossFightActive = false;
	spawnTimer = 0.0;
}

void Boss::Update(double dt)
{
	if (active)
	{
		if ((Player::GetInstance()->GetPos() - pos).Length() <= 50.f && !bossFightActive)
		{
			bossFightActive = true;
		}
		if (bossFightActive)
		{
			if (hp <= 0.f)
			{
				active = false;
				bossFightActive = false;
				return;
			}

			spawnTimer += dt;
			if (spawnTimer >= 15.0)
			{
				SpawnEnemies();
				spawnTimer = 0.0;
			}
		}
	}
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

bool Boss::GetFightActive()
{
	return bossFightActive;
}

float Boss::GetHP()
{
	return hp;
}

void Boss::SetHP(float hp)
{
	this->hp = hp;
}

void Boss::SpawnEnemies()
{
	int randInt = Math::RandIntMinMax(5, 10);
	for (int i = 0; i < randInt; ++i)
	{
		float randAngle = Math::RandFloatMinMax(0.f, Math::TWO_PI);
		Vector3 unitVector = Vector3(cosf(randAngle), sinf(randAngle));
		CavemenMelee* enemy = new CavemenMelee(baseEnemy);
		enemy->Init();
		enemy->SetEnemyPos(pos + (unitVector * Math::RandFloatMinMax(30.f, 60.f)));
		SceneManager::GetInstance()->GetActiveScene()->enemyList.push_back(enemy);
	}
}
