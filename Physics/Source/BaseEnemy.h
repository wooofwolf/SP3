#pragma once
#include <iostream>
#include "Entity.h"
#include "Map.h"
class BaseEnemy : public Entity
{
public:
	BaseEnemy();
	~BaseEnemy();
	void Init();
	void Update(double dt);
	void Patrol();
	Vector3 GetEnemyPos();
	Map* map;
protected: 
	enum DIRECTION
	{
		LEFT = 0,
		RIGHT = 1,
		UP = 2,
		DOWN = 3,
		NUM_DIRECTIONS
	};
	int Damage;
	int direction;
	float directionalCD;
};

