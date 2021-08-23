#pragma once

#include "Entity.h"
#include "SingletonPattern.h"
#include "BaseEnemy.h"

class Boss : public Entity, public SingletonPattern<Boss>
{
	friend SingletonPattern<Boss>;
public:
	enum class FSM
	{
		INACTIVE = 0,
		IDLE,
		INVINCIBLE,
		SPAWN_MINIONS,
		ATTACK,
		ATTACK_WAIT,
		TOTAL_STATES
	};

	void Init();
	void Update(double dt);
	void Render();
	void Exit();

	void Movement(double dt);
	void Collisioncheck(std::vector<Vector3> activeTileList);

	void SetPos(Vector3 pos);
	Vector3 GetPos();
	Vector3 GetScale();
	bool GetFightActive();
	float GetHP();
	void SetHP(float hp);
	bool GetInvincible();

	void SpawnEnemies();
	void Attack();
	void SetAttacked(bool attacked);

protected:
	// Constructor
	Boss();
	// Destructor
	virtual ~Boss();

	bool bossFightActive;
	double stateTimer;
	BaseEnemy* baseEnemy;
	bool invincible;
	double invincibleTimer;
	FSM currentFSM;
	bool attacked;
};

class BossAttack : public Entity, public SingletonPattern<BossAttack>
{
	friend SingletonPattern<BossAttack>;
public:
	void Init();
	void Update(double dt);
	void Render();
	void Exit();

	void Movement(double dt);
	void Collisioncheck(std::vector<Vector3> activeTileList);

	void DamagePlayer();

protected:
	// Constructor
	BossAttack();
	// Destructor
	virtual ~BossAttack();

	double targetTimer;
};