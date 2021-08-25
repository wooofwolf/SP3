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
	hp = 1500;
	pos.SetZero();
	prevpos = pos;
	vel.SetZero();
	scale.Set(10.f, 10.f, 1.f);
	normal.SetZero();
	active = true;

	bossFightActive = false;
	stateTimer = 5.0;
	currentFSM = FSM::INACTIVE;
	invincible = false;
	invincibleTimer = 0.0;
	attacked = false;
}

void Boss::Update(double dt)
{
	if (active)
	{
		switch (currentFSM)
		{
		case FSM::INACTIVE:
			if ((Player::GetInstance()->GetPos() - pos).Length() <= 50.f && !bossFightActive && hp == 1500)
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
			if (stateTimer >= 10.0)
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
			if (invincibleTimer >= 10.0)
			{
				invincible = false;
				currentFSM = FSM::IDLE;
			}
			break;
			
		case FSM::ATTACK:
			Attack();
			currentFSM = FSM::ATTACK_WAIT;
			break;

		case FSM::ATTACK_WAIT:
			BossAttack* attack = BossAttack::GetInstance();
			attack->Update(dt);
			if (attacked)
			{
				attack->Exit();
				attack->Destroy();
				stateTimer = 0.0;
				currentFSM = FSM::IDLE;
				attacked = false;
			}
			break;
		}
	}
}

void Boss::Render()
{
	if (active)
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
			scene->RenderMesh(MeshList::GetInstance()->GetMesh(MeshList::GEO_CAVEMAN), false);
			scene->GetModelStack().PopMatrix();
		}

		if (currentFSM == FSM::ATTACK_WAIT)
		{
			BossAttack* attack = BossAttack::GetInstance();
			attack->Render();
		}
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
	int randInt = Math::RandIntMinMax(1, 3);
	int randInt2 = Math::RandIntMinMax(2, 4);
	int randInt3 = Math::RandIntMinMax(2, 4);
	switch (randInt)
	{
	case 1:
		for (int i = 0; i < randInt2; ++i)
		{
			float randAngle = Math::RandFloatMinMax(0.f, Math::TWO_PI);
			Vector3 unitVector = Vector3(cosf(randAngle), sinf(randAngle));
			CavemenMelee* enemy = new CavemenMelee(baseEnemy);
			enemy->Init();
			enemy->SetEnemyPos(pos + (unitVector * Math::RandFloatMinMax(20.f, 50.f)));
			SceneManager::GetInstance()->GetActiveScene()->enemyList.push_back(enemy);
		}
		for (int i = 0; i < randInt3; ++i)
		{
			float randAngle = Math::RandFloatMinMax(0.f, Math::TWO_PI);
			Vector3 unitVector = Vector3(cosf(randAngle), sinf(randAngle));
			CavemenRanged* enemy = new CavemenRanged(baseEnemy);
			enemy->Init();
			enemy->SetEnemyPos(pos + (unitVector * Math::RandFloatMinMax(20.f, 50.f)));
			SceneManager::GetInstance()->GetActiveScene()->enemyList.push_back(enemy);
		}
		break;
	case 2:
		for (int i = 0; i < randInt2; ++i)
		{
			float randAngle = Math::RandFloatMinMax(0.f, Math::TWO_PI);
			Vector3 unitVector = Vector3(cosf(randAngle), sinf(randAngle));
			Knights* enemy = new Knights(baseEnemy);
			enemy->Init();
			enemy->SetEnemyPos(pos + (unitVector * Math::RandFloatMinMax(20.f, 50.f)));
			SceneManager::GetInstance()->GetActiveScene()->enemyList.push_back(enemy);
		}
		for (int i = 0; i < randInt3; ++i)
		{
			float randAngle = Math::RandFloatMinMax(0.f, Math::TWO_PI);
			Vector3 unitVector = Vector3(cosf(randAngle), sinf(randAngle));
			Archer* enemy = new Archer(baseEnemy);
			enemy->Init();
			enemy->SetEnemyPos(pos + (unitVector * Math::RandFloatMinMax(20.f, 50.f)));
			SceneManager::GetInstance()->GetActiveScene()->enemyList.push_back(enemy);
		}
		break;
	case 3:
		for (int i = 0; i < randInt2; ++i)
		{
			float randAngle = Math::RandFloatMinMax(0.f, Math::TWO_PI);
			Vector3 unitVector = Vector3(cosf(randAngle), sinf(randAngle));
			Cyborg* enemy = new Cyborg(baseEnemy);
			enemy->Init();
			enemy->SetEnemyPos(pos + (unitVector * Math::RandFloatMinMax(20.f, 50.f)));
			SceneManager::GetInstance()->GetActiveScene()->enemyList.push_back(enemy);
		}
		for (int i = 0; i < randInt3; ++i)
		{
			float randAngle = Math::RandFloatMinMax(0.f, Math::TWO_PI);
			Vector3 unitVector = Vector3(cosf(randAngle), sinf(randAngle));
			Sniper* enemy = new Sniper(baseEnemy);
			enemy->Init();
			enemy->SetEnemyPos(pos + (unitVector * Math::RandFloatMinMax(20.f, 50.f)));
			SceneManager::GetInstance()->GetActiveScene()->enemyList.push_back(enemy);
		}
		break;
	}
}

bool Boss::GetInvincible()
{
	return invincible;
}

void Boss::Attack()
{
	BossAttack* attack = BossAttack::GetInstance();
	attack->Init();
}

void Boss::SetAttacked(bool attacked)
{
	this->attacked = attacked;
}

BossAttack::BossAttack()
{
	hp = 0;
	pos.SetZero();
	prevpos = pos;
	vel.SetZero();
	scale.Set(15, 15, 1);
	normal.SetZero();
	active = false;
}

BossAttack::~BossAttack()
{
}

void BossAttack::Init()
{
	active = true;
	pos = Player::GetInstance()->GetPos();
	targetTimer = 0.0;
}

void BossAttack::Update(double dt)
{
	if (targetTimer < 3.0)
		Movement(dt);
	if (targetTimer >= 5.0)
	{
		DamagePlayer();
		Boss::GetInstance()->SetAttacked(true);
	}
	targetTimer += dt;
}

void BossAttack::Render()
{
	SceneBase* scene = SceneManager::GetInstance()->GetActiveScene();
	scene->GetModelStack().PushMatrix();
		scene->GetModelStack().Translate(pos.x, pos.y, pos.z);
		scene->GetModelStack().Scale(scale.x, scale.y, scale.z);
		scene->RenderMesh(MeshList::GetInstance()->GetMesh(MeshList::GEO_SPHERE_BLUE), false);
	scene->GetModelStack().PopMatrix();
}

void BossAttack::Exit()
{
}

void BossAttack::Movement(double dt)
{
	pos = Player::GetInstance()->GetPos();
}

void BossAttack::Collisioncheck(std::vector<Vector3> activeTileList)
{
}

void BossAttack::DamagePlayer()
{
	Player* p = Player::GetInstance();
	if ((p->GetPos() - pos).Length() <= 15.f)
		p->SetHealth(p->GetHealth() - 50.f);
}
