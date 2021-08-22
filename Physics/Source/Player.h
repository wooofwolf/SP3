	#pragma once

#include <vector>

#include "Vector3.h"
#include "Entity.h"
#include "SceneBase.h"
#include "SingletonPattern.h"
#include "Guns.h"


class Player : public Entity, public SingletonPattern<Player>
{
	friend SingletonPattern<Player>;
protected:

	float SlowSpeed;
	float m_speed;
	float SprintSpeed;

	int m_worldWidth;
	int m_worldHeight;

	GunComponent* activeGun;

	float energy;

	float cooldownTimer2;
	float cooldownTimer3;

	float dashCooldown;
	float dashTimer;
	bool dash;
public:
	// Constructor
	Player();
	// Destructor
	~Player();

	void Init();
	void Init(int m_worldWidth, int m_worldHeight, GunComponent* activeGun);
	void Update(double dt);
	void Render();
	void Exit();

	void Collisioncheck(std::vector<Vector3> activeTileList);
	void Hazardcheck(std::vector<Vector3> activeTileList);
	void Exitcheck(std::vector<Vector3> activeTileList);


	void Movement(double dt);

	Vector3 GetPos();
	Vector3 GetRotate();
	Vector3 GetScale();

	void SetRotate(Vector3 rotate);
	void SetPos(Vector3 pos);

	float GetPlusSpeed();
	float GetSlowSpeed();
	
	bool power1active;
	bool power2active;

	GunComponent* GetActiveGun();
	void SetActiveGun(GunComponent* activeGun);

	float GetHealth();
	void SetHealth(float hp);
	float GetEnergy();
	void SetEnergy(int energy);
};

