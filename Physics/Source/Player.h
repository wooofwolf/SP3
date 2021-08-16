#pragma once

#include "Vector3.h"
#include "Entity.h"
#include "SceneBase.h"
#include "SingletonPattern.h"

class Player : public Entity, public SingletonPattern<Player>
{
	friend SingletonPattern<Player>;
protected:

	MS modelStack;

	SceneBase* base;

	float m_speed;
	float SprintSpeed;

public:
	// Constructor
	Player();
	// Destructor
	~Player();

	void Init();
	void Update(double dt);
	void Render();

	void Movement();

	Vector3 GetPos();
	Vector3 GetRotate();
	Vector3 GetScale();

	float GetGameSpeed();
};

