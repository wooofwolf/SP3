#pragma once
#include <iostream>
#include "Entity.h"
class BaseEnemy : protected Entity
{
public:
	~BaseEnemy();
	void Init();
	void Update(double dt);
	void Render();
	void Movement();
	Vector3 GetEnemyPos();
	bool GetActive();
	bool active;
protected: 
	enum DIRECTION
	{
		LEFT = 0,
		RIGHT = 1,
		UP = 2,
		DOWN = 3,
		NUM_DIRECTIONS
	};
	int direction;
	float directionalCD;
	virtual void Attack() const = 0;
};

class ConcreteBaseEnemy : public BaseEnemy
{
public:
	void Attack() const override;
};

class EnemyDecorator : public BaseEnemy
{
protected:
	BaseEnemy* baseEnemy;

public:
	EnemyDecorator(BaseEnemy* baseEnemy);

	void Attack() const override;
};

class CavemenMelee:public EnemyDecorator
{
public:
	CavemenMelee(BaseEnemy* baseEnemy);

	void Attack() const override;
};
class CavemenRanged :public EnemyDecorator
{
public:
	CavemenRanged(BaseEnemy* baseEnemy);

	void Attack() const override;
};
class Knights : public EnemyDecorator
{
public:
	Knights(BaseEnemy* baseEnemy);

	void Attack() const override;
};
class Archer : public EnemyDecorator
{
public:
	Archer(BaseEnemy* baseEnemy);

	void Attack() const override;
};
class Cyborg : public EnemyDecorator
{
public:
	Cyborg(BaseEnemy* baseEnemy);

	void Attack() const override;
};

class Sniper : public EnemyDecorator
{
public:
	Sniper(BaseEnemy* baseEnemy);

	void Attack() const override;
};

