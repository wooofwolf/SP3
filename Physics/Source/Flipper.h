#ifndef FLIPPER_H
#define FLIPPER_H

#include "GameObject.h"
#include "Vector3.h"

struct Flipper : public GameObject
{
	float momentOfInertia;
	float angularVelocity;
	Vector3 force;
	Vector3 torque;

	Flipper();
	~Flipper();
};

#endif