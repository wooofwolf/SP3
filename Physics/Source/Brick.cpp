#include "Brick.h"

Brick::Brick() : sameBrick{ nullptr }, side(SceneAssignment2::NONE),isPowerup(false)
{
}

Brick::~Brick()
{
}

void Brick::insertBrick(Brick* brick)
{
	for (int i = 0; i < 5; ++i)
	{
		if (sameBrick[i] == nullptr)
		{
			sameBrick[i] = brick;
			return;
		}
	}

	std::cout << "sameBrick is full." << std::endl;
}

void Brick::removeAllBricks()
{
	for (int i = 0; i < 5; ++i)
	{
		sameBrick[i]->active = false;
	}
}

PowerupBrick::PowerupBrick() : powerupType(PU_NONE)
{
}

PowerupBrick::PowerupBrick(POWERUP_TYPE type) : powerupType(type)
{
}

PowerupBrick::~PowerupBrick()
{
}

void PowerupBrick::multiplyBall(GameObject* ball, std::vector<GameObject*>& goList)
{
	GameObject* newBall = new GameObject(GameObject::GO_BALL);
	goList.push_back(newBall);
	newBall->active = true;
	newBall->pos = ball->pos;
	newBall->scale.Set(2, 2, 2);
	newBall->mass = 8.f;
}

void PowerupBrick::increaseOppGrav(Vector3& gravity)
{
	gravity.Set(0, -17.6f, 0);
}
