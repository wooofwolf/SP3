#ifndef SCENE_ASSIGNMENT_2_H
#define SCENE_ASSIGNMENT_2_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Flipper.h"
#include "timer.h"

class SceneAssignment2 : public SceneBase
{

public:
	enum SIDE
	{
		NONE = 0,
		LEFT,
		RIGHT
	};

	SceneAssignment2();
	~SceneAssignment2();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

	GameObject* FetchGO();
	void ReturnGO(GameObject *go);

	void MakeThinWall(float x, float y, float angleDeg, float thickness, float height);			// A wall with 2 pillars
	void MakeThickWall(float x, float y, float angleDeg, float thickness, float height);		// 2 walls with 4 pillars
	void MakeBrick(float x, float y, float angleDeg, float thickness, float height, SIDE side);
	void MakeMultiplyBrick(float x, float y, float angleDeg, float thickness, float height, SIDE side);
	void MakeGravBrick(float x, float y, float angleDeg, float thickness, float height, SIDE side);
	void MakeBumper(float x, float y, float angleDeg, float thickness, float height);
	void MakePillarBumper(float x, float y, float thickness);
	void MakeOneWall(float x, float y, float angleDeg, float thickness, float height);

	// Collision codes
	bool CheckCollision(GameObject* go1, GameObject* go2, float dt);
	void CollisionResponse(GameObject* go1, GameObject* go2);

	void RespawnBall(SIDE side);

	static bool gameStarted;
protected:

	//Physics
	std::vector<GameObject *> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	GameObject *m_ghost;
	int m_objectCount;
	Vector3 m_gravity, m_gravity2;
	Flipper* flipperL, * flipperR;
	Flipper* flipperL2, * flipperR2;
	GameObject* m_booster, * m_booster2;

	double timeAfterStarted;
	StopWatch* stopwatch;
	float m_respawnL, m_respawnR;
	int bricksLeftL, bricksLeftR;
	bool gravIncreasedL, gravIncreasedR;
	float m_gravL, m_gravR;

	bool leftWins, rightWins;

	//Auditing
	float m1, m2;
	Vector3 u1, u2, v1, v2;
};

#endif