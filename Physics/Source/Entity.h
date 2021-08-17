#pragma once

#include "Vector3.h"

class Entity
{
protected:
	// Variables
	int hp;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	Vector3 normal;

public:
	bool active;
	// Constructor
	Entity();
	// Destructor
	~Entity();

	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;

	virtual void Movement() = 0;
};

