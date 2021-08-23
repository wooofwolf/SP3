#pragma once
#include <iostream>
#include "Entity.h"
#include "Vector3.h"
#include <vector>

#include <stack>

class BaseEnemy : public Entity
{
protected: 
	enum DIRECTION
	{
		LEFT = 0,
		RIGHT = 1,
		UP = 2,
		DOWN = 3,
		NUM_DIRECTIONS
	};
	enum FSM
	{
		ATTACK = 0,
		IDLE = 1,
		MOVE = 2,
		PATHFIND = 3,
		NUM_FSM
	};
	void CheckCollidedSide(DIRECTION direction , double dt);
	void Attack();
public:
	std::vector<BaseEnemy*> BasicEnemyList;
	~BaseEnemy();
	virtual void Init();

	void Render();
	Vector3 GetEnemyPos();
	void SetEnemyPos(Vector3 pos);
	float GetEnemyHP();
	void SetEnemyHP(int hp);
	bool GetActive();

	Vector3 GetScale();
	bool active;
	void Collisioncheck(std::vector <Vector3> activeTileList);
	void Exit();

	bool moving = true;

	float stasisTimer;
	float directionalCD;
	virtual void Update(double dt);
	bool collided;
	//bool changeDir;
	void Movement(double dt);
	DIRECTION direction;
	FSM currentFSM;
	Vector3 destination;
	Vector3 pathDir;
	int damage;
	float FSMcounter;
	float FSMMaxCounter = 60;
	void UpdateDirection();
	void UpdatePosition();
	bool bPathFind;
	float attackCD;
	bool attacked;
	std::stack<Vector3> path;
	bool pathfinding;
	double movingTimer;
	Vector3 dest;
};

class ConcreteBaseEnemy : public BaseEnemy
{
public:
	void Update(double dt) override;
	void Init() override; 
};

class EnemyDecorator : public BaseEnemy
{
protected:
	BaseEnemy* baseEnemy;

public:
	EnemyDecorator(BaseEnemy* baseEnemy);
	void Update(double dt) override;
	void Init() override;
};

class CavemenMelee:public EnemyDecorator
{
public:
	CavemenMelee(BaseEnemy* baseEnemy);

	void Attack();
	void Update(double dt) override;
	void Init() override;


};
class CavemenRanged :public EnemyDecorator
{
public:
	CavemenRanged(BaseEnemy* baseEnemy);

	void Update(double dt) override;
	void Init() override;
};
class Knights : public EnemyDecorator
{
public:
	Knights(BaseEnemy* baseEnemy);

	void Attack();
	void Update(double dt) override;
	void Init() override;
};
class Archer : public EnemyDecorator
{
public:
	Archer(BaseEnemy* baseEnemy);
	void Update(double dt) override;
	void Init() override;
};
class Cyborg : public EnemyDecorator
{
public:
	Cyborg(BaseEnemy* baseEnemy);

	void Attack();
	void Update(double dt) override;
	void Init() override;
};

class Sniper : public EnemyDecorator
{
public:
	Sniper(BaseEnemy* baseEnemy);

	void Update(double dt) override;
	void Init() override;
};

