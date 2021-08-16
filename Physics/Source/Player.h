#pragma once

#include "Vector3.h"
#include "Entity.h"

#include "SceneBase.h"

class Player : public Entity
{
protected:

	MS modelStack;

public:
	// Constructor
	Player();
	// Destructor
	~Player();

	void Init();
	void Update();
	void Render();

	void Movement();
};

