#pragma once

#include "Vector3.h"
#include "Entity.h"

#include "SceneBase.h"

class Player : public Entity
{
protected:

	MS modelStack;

	SceneBase* base;

public:
	// Constructor
	Player();
	// Destructor
	~Player();

	void Init();
	void Update(double dt);
	void Render();

	void Movement();

	Vector3 getPos();
	Vector3 getRotate();
	Vector3 getScale();

	void setRotate(int x, int y);
};

