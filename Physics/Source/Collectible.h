#pragma once

#include "Entity.h"

class Collectible : public Entity
{
public:
	Collectible();
	~Collectible();

	void Init();
	void Update(double dt);
	void Render();
	void Exit();

	void Movement(double dt);
	void Collisioncheck(std::vector<Vector3> activeTileList);

	Vector3 GetPos();
	Vector3 GetScale();

	void SetPos(Vector3 pos);
	void SetScale(Vector3 scale);
};

