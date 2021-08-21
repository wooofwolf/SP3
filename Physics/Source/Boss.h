#pragma once

#include "Entity.h"
#include "SingletonPattern.h"

class Boss : public Entity, public SingletonPattern<Boss>
{
	friend SingletonPattern<Boss>;
protected:
	// Constructor
	Boss();
	// Destructor
	virtual ~Boss();

	bool bossFightActive;

public:
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
};

