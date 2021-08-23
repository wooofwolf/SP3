#include "Boss.h"

#include "Player.h"
#include "BaseEnemy.h"
#include "SceneManager.h"
#include "MeshList.h"

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
	stateTimer = 10.0;
	currentFSM = FSM::INACTIVE;
	invincible = false;
	invincibleTimer = 0.0;
}

void Boss::Update(double dt)
{
	if (active)
	{
		switch (currentFSM)
		{
		case FSM::INACTIVE:
			if ((Player::GetInstance()->GetPos() - pos).Length() <= 50.f && !bossFightActive && hp == 1000)
			{
				bossFightActive = true;
				currentFSM = FSM::IDLE;
			}
			break;

		case FSM::IDLE:
			// Kill boss
			if (hp <= 0.f)
			{
				active = false;
				bossFightActive = false;
				currentFSM = FSM::INACTIVE;
				break;
			}

			stateTimer += dt;
			if (stateTimer >= 15.0)
			{
				// Random state
				currentFSM = static_cast<FSM>(Math::RandIntMinMax(3, 4));
			}
			break;

		case FSM::SPAWN_MINIONS:
			SpawnEnemies();
			stateTimer = 0.0;
			invincibleTimer = 0.0;
			currentFSM = FSM::INVINCIBLE;
			break;

		case FSM::INVINCIBLE:
			invincible = true;
			invincibleTimer += dt;
			if (invincibleTimer >= 15.0)
			{
				invincible = false;
				currentFSM = FSM::IDLE;
			}
			break;
			
		case FSM::ATTACK:
			Attack();
			stateTimer = 0.0;
			currentFSM = FSM::IDLE;
			break;
		}
	}
}

void Boss::Render()
{
	SceneBase* scene = SceneManager::GetInstance()->GetActiveScene();
	float width = (pos - Camera::GetInstance()->position).Length();
	float height = Camera::GetInstance()->position.z - pos.z;
	float hypotenuse = sqrtf(width * width + height * height);
	float maxWidth = (Vector3(scene->GetWorldWidth() / 2 + 2, 0, 1).Dot(Vector3(scene->GetWorldHeight() / 2 + 2, 0, 1)) - Camera::GetInstance()->position.Length());
	float maxHypotenuse = sqrtf(maxWidth * maxWidth + height * height);

	if (hypotenuse <= maxHypotenuse)
	{
		scene->GetModelStack().PushMatrix();
		scene->GetModelStack().Translate(pos.x, pos.y, pos.z);
		scene->GetModelStack().Scale(scale.x, scale.y, scale.z);
		scene->RenderMesh(MeshList::GetInstance()->GetMesh(MeshList::GEO_BASEENEMY), false);
		scene->GetModelStack().PopMatrix();
	}
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
		enemy->SetEnemyPos(pos + (unitVector * Math::RandFloatMinMax(40.f, 80.f)));
		SceneManager::GetInstance()->GetActiveScene()->enemyList.push_back(enemy);
	}
}

bool Boss::GetInvincible()
{
	return invincible;
}

void Boss::Attack()
{

}