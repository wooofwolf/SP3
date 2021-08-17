#pragma once

#include <vector>

#include "Vector3.h"
#include "Entity.h"
#include "SceneBase.h"
#include "SingletonPattern.h"
#include "Map.h"


class Player : public Entity, public SingletonPattern<Player>
{
	friend SingletonPattern<Player>;
protected:

	MS modelStack;

	SceneBase* base;

	float m_speed;
	float SprintSpeed;

	int m_worldWidth;
	int m_worldHeight;

public:
	// Constructor
	Player();
	// Destructor
	~Player();

	void Init();
	void Init(int m_worldWidth, int m_worldHeight);
	void Update(double dt);
	void Render();
	void Exit();

	void Collisioncheck(std::vector<Vector3> activeTileList);

	void Movement();

	Vector3 GetPos();
	Vector3 GetRotate();
	Vector3 GetScale();

	void SetPos(Vector3 pos);

	float GetGameSpeed();
};

