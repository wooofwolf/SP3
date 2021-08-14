#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"

struct GameObject
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CUBE,
		GO_ASTEROID,
		GO_BIG_ASTEROID,
		GO_SHIP,
		GO_BULLET,
		GO_ENEMY,
		GO_ENEMY_BULLET,
		GO_MISSILE,
		GO_POWERUP_BULLET,
		GO_POWERUP_SHIELD,
		GO_POWERUP_MISSILE,
		GO_SHIELD,
		GO_ENEMY_FOLLOWING,
		GO_WALL,
		GO_PILLAR,
		GO_BRICK,
		GO_BRICK_CORNER,
		GO_BUMPER,
		GO_BUMPER_PILLAR,
		GO_FLIPPER,
		GO_TOTAL, //must be last
	};
	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	bool active;
	float mass;
	Vector3 normal;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
};

#endif