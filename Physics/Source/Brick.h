#ifndef BRICK_H
#define BRICK_H

#include <vector>

#include "GameObject.h"
#include "SceneAssignment2.h"

struct Brick : public GameObject
{
	Brick* sameBrick[5];
	SceneAssignment2::SIDE side;
	bool isPowerup;

	Brick();
	~Brick();

	void insertBrick(Brick* brick);
	void removeAllBricks();
};

struct PowerupBrick : public Brick
{
	enum POWERUP_TYPE
	{
		PU_NONE = 0,
		PU_MULTIPLY,
		PU_INCREASE_GRAV
	};

	PowerupBrick();
	PowerupBrick(POWERUP_TYPE type);
	~PowerupBrick();

	POWERUP_TYPE powerupType;

	void multiplyBall(GameObject* ball, std::vector<GameObject*>& goList);
	void increaseOppGrav(Vector3& gravity);
};

#endif