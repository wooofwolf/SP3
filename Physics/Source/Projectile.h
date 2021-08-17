#pragma once

#include "Entity.h"

struct Projectile: public Entity
{
	enum class PROJECTILE_TYPE
	{
		PROJECTILE_NONE = 0,
		PROJECTILE_REVOLVER,
		PROJECTILE_SHOTGUN,
		PROJECTILE_MINIGUN,
		PROJECTILE_TOTAL
	};

	PROJECTILE_TYPE type;
	int damage;

	Projectile();
	Projectile(PROJECTILE_TYPE type);
	~Projectile();

	void Init();
	void Init(int shotgunID);
	void Update(double dt);
	void Render();
	void Exit();
	void Movement();
	void Collisioncheck(std::vector <Vector3> activeTileList);

	Vector3 GetPos();
	Vector3 GetScale();
};

