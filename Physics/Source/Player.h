#pragma once

#include "Vector3.h"
#include "Entity.h"

class Player : public Entity
{
private:

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

