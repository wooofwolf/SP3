#pragma once

#include "Entity.h"
#include <stdlib.h>
#include "BaseEnemy.h"
struct Projectile: public Entity
{
	enum class PROJECTILE_TYPE
	{
		PROJECTILE_NONE = 0,
		PROJECTILE_REVOLVER,
		PROJECTILE_SHOTGUN,
		PROJECTILE_MINIGUN,
		PROJECTILE_ABILITY_3,
		PROJECTILE_ABILITY_3_EXPLOSION,
		PROJECTILE_SPEARS,
		PROJECTILE_ARROWS,
		PROJECTILE_SNIPERBULLET,
		PROJECTILE_TOTAL
	};

	PROJECTILE_TYPE type;
	int damage;
	double detonateTimer;
	double explosionTimer;

	Projectile();
	Projectile(PROJECTILE_TYPE type);
	~Projectile();

	void Init();
	void Init(int shotgunID);
	void Init(BaseEnemy* enemy);
	void EnemyShoot(PROJECTILE_TYPE type , BaseEnemy* enemy, double dt);
	void Update(double dt);
	void Render();
	void Exit();
	void Movement(double dt);
	void Collisioncheck(std::vector <Vector3> activeTileList);
	void SetVelo(Vector3 vel);
	Vector3 GetPos();
	void SetPos(Vector3 pos);
	Vector3 GetScale();
	void SetScale(Vector3 scale);


};

