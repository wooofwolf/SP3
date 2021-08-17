#pragma once
#include <iostream>
#include "Entity.h"
#include "Vector3.h"
#include <vector>
#include "Map.h"
class BaseEnemy : protected Entity
{
public:
	std::vector<BaseEnemy*> BasicEnemyList;
	~BaseEnemy();
	virtual void Init();
	void Render();
	void Movement(double dt);
	Vector3 GetEnemyPos();
	bool GetActive();
	bool active;
	void Collisioncheck(std::vector <Vector3> activeTileList);
	void Exit();
	int direction;
	float directionalCD;
	virtual void Update(double dt);
	bool collided;
protected: 
	enum DIRECTION
	{
		LEFT = 0,
		RIGHT = 1,
		UP = 2,
		DOWN = 3,
		NUM_DIRECTIONS
	};
	void CheckCollidedSide(DIRECTION direction , double dt);
	virtual void Attack() const = 0;
};

class ConcreteBaseEnemy : public BaseEnemy
{
public:
	void Attack() const override;
	void Update(double dt) override;
	void Init() override; 
};

class EnemyDecorator : public BaseEnemy
{
protected:
	BaseEnemy* baseEnemy;

public:
	EnemyDecorator(BaseEnemy* baseEnemy);

	void Attack() const override;
	void Update(double dt) override;
	void Init() override;
};

class CavemenMelee:public EnemyDecorator
{
public:
	CavemenMelee(BaseEnemy* baseEnemy);

	void Attack() const override;
	void Update(double dt) override;
	void Init() override;
};
class CavemenRanged :public EnemyDecorator
{
public:
	CavemenRanged(BaseEnemy* baseEnemy);

	void Attack() const override;
	void Update(double dt) override;
	void Init() override;
};
class Knights : public EnemyDecorator
{
public:
	Knights(BaseEnemy* baseEnemy);

	void Attack() const override;
	void Update(double dt) override;
	void Init() override;
};
class Archer : public EnemyDecorator
{
public:
	Archer(BaseEnemy* baseEnemy);

	void Attack() const override;
	void Update(double dt) override;
	void Init() override;
};
class Cyborg : public EnemyDecorator
{
public:
	Cyborg(BaseEnemy* baseEnemy);

	void Attack() const override;
	void Update(double dt) override;
	void Init() override;
};

class Sniper : public EnemyDecorator
{
public:
	Sniper(BaseEnemy* baseEnemy);

	void Attack() const override;
	void Update(double dt) override;
	void Init() override;
};

