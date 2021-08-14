#include "SceneAssignment2.h"
#include "GL\glew.h"
#include "Application.h"
#include "Brick.h"
#include "Flipper.h"
#include "MeshBuilder.h"
#include <sstream>

bool SceneAssignment2::gameStarted = false;

SceneAssignment2::SceneAssignment2()
{
}

SceneAssignment2::~SceneAssignment2()
{
}

void SceneAssignment2::Init()
{
	//among us
	SceneBase::Init();
	
	stopwatch = new StopWatch();
	timeAfterStarted = 0;
	m_respawnL = m_respawnR = 0.f;
	bricksLeftL = bricksLeftR = 0;

	// Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	m_speed = 1.f;
	m_gravity.Set(0, -9.8f, 0); //init gravity as 9.8ms-2 downwards
	m_gravity2.Set(0, -9.8f, 0);
	m_gravL = m_gravR = 0.f;
	gravIncreasedL = gravIncreasedR = false;

	leftWins = rightWins = false;

	Math::InitRNG();

	//Exercise 1: initialize m_objectCount
	m_objectCount = 0;

	m_ghost = new GameObject(GameObject::GO_BALL);

	// Init flippers
	flipperL = new Flipper();
	m_goList.push_back(flipperL);
	flipperL->active = true;
	flipperL->type = GameObject::GO_FLIPPER;
	flipperL->pos.Set(30.f, m_worldHeight / 4.5);
	flipperL->scale.Set(2, 12, 1);
	flipperL->mass = 0.003f;
	flipperL->momentOfInertia = flipperL->mass * flipperL->scale.x * flipperL->scale.x;
	flipperL->normal.Set(0, 1, 0);

	flipperR = new Flipper();
	m_goList.push_back(flipperR);
	flipperR->active = true;
	flipperR->type = GameObject::GO_FLIPPER;
	flipperR->pos.Set(m_worldWidth / 2 - 30.f, m_worldHeight / 4.5);
	flipperR->scale.Set(2, 12, 1);
	flipperR->mass = 0.003f;
	flipperR->momentOfInertia = flipperL->mass * flipperL->scale.x * flipperL->scale.x;
	flipperR->normal.Set(0, 1, 0);

	flipperL2 = new Flipper();
	m_goList.push_back(flipperL2);
	flipperL2->active = true;
	flipperL2->type = GameObject::GO_FLIPPER;
	flipperL2->pos.Set(m_worldWidth / 2 + 30.f, m_worldHeight / 4.5);
	flipperL2->scale.Set(2, 12, 1);
	flipperL2->mass = 0.003f;
	flipperL2->momentOfInertia = flipperL2->mass * flipperL2->scale.x * flipperL2->scale.x;
	flipperL2->normal.Set(0, 1, 0);

	flipperR2 = new Flipper();
	m_goList.push_back(flipperR2);
	flipperR2->active = true;
	flipperR2->type = GameObject::GO_FLIPPER;
	flipperR2->pos.Set(m_worldWidth - 30.f, m_worldHeight / 4.5);
	flipperR2->scale.Set(2, 12, 1);
	flipperR2->mass = 0.003f;
	flipperR2->momentOfInertia = flipperR2->mass * flipperR2->scale.x * flipperR2->scale.x;
	flipperR2->normal.Set(0, 1, 0);
	
	// BORDERS
	MakeOneWall(1.5f, m_worldHeight / 2, 90.f, 100.f, 3.f);
	MakeOneWall(m_worldWidth - 1.5f, m_worldHeight / 2, 90.f, 100.f, 3.f);
	
	MakeThickWall(m_worldWidth / 2 - 7.5f, m_worldHeight / 2 + 40.f , 90.f, 80.f, 1.f);
	MakeOneWall(m_worldWidth / 2, m_worldHeight / 2, 90.f, 100.f, 2.f);
	MakeThickWall(m_worldWidth / 2 + 7.5f, m_worldHeight / 2 + 40.f, 90.f, 80.f, 1.f);

	MakeOneWall(m_worldWidth / 2, m_worldHeight - 1.5f, 0.f, m_worldWidth, 3.f);

	MakeBumper(m_worldWidth / 2 - 5.f, m_worldHeight / 2 - 10.f, -45.f, 15.f, 2.f);
	MakeBumper(m_worldWidth / 2 + 5.f, m_worldHeight / 2 - 10.f, 45.f, 15.f, 2.f);

	MakeThickWall(m_worldWidth / 2 + 15.f, m_worldHeight / 4, 10.f, 30.f, 2.f);
	MakeThickWall(m_worldWidth - 15.f, m_worldHeight / 4, -10.f, 30.f, 2.f);
	MakeThickWall(m_worldWidth / 2 - 15.f, m_worldHeight / 4, -10.f, 30.f, 2.f);
	MakeThickWall(15.f, m_worldHeight / 4, 10.f, 30.f, 2.f);

	MakePillarBumper((30.f + (m_worldWidth / 2 - 30.f)) / 2, m_worldHeight / 2 + 10.f, 3.f);
	MakeThinWall(30.f, m_worldHeight / 2, 75.f, 2.f, 15.f);
	MakeThinWall(m_worldWidth / 2 - 30.f, m_worldHeight / 2, 105.f, 2.f, 15.f);
	MakePillarBumper((m_worldWidth - 30.f + (m_worldWidth / 2 + 30.f)) / 2, m_worldHeight / 2 + 10.f, 3.f);
	MakeThinWall(m_worldWidth - 30.f, m_worldHeight / 2, 105.f, 2.f, 15.f);
	MakeThinWall(m_worldWidth / 2 + 30.f, m_worldHeight / 2, 75.f, 2.f, 15.f);

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			MakeBrick(12.f + j * 10.f, m_worldHeight - 17.f - i * 7.f, 0, 3, 2, LEFT);
			++bricksLeftL;
			MakeBrick(m_worldWidth - 12.f - j * 10.f, m_worldHeight - 17.f - i * 7.f, 0, 3, 2, RIGHT);
			++bricksLeftR;
		}
	}

	MakeMultiplyBrick(32.f, m_worldHeight - 10.f, 0, 3, 2, LEFT);
	MakeMultiplyBrick(m_worldWidth - 52.f, m_worldHeight - 10.f, 0, 3, 2, RIGHT);
	MakeGravBrick(52.f, m_worldHeight - 10.f, 0, 3, 2, LEFT);
	MakeGravBrick(m_worldWidth - 32.f, m_worldHeight - 10.f, 0, 3, 2, RIGHT);
	bricksLeftL += 2;
	bricksLeftR += 2;
}

GameObject* SceneAssignment2::FetchGO()
{
	//Exercise 2a: implement FetchGO()
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		//Exercise 2b: increase object count every time an object is set to active
		if (!go->active)
		{
			++m_objectCount;
			go->active = true;
			return go;
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		GameObject* go = new GameObject(GameObject::GO_BALL);
		m_goList.push_back(go);
	}

	//Exercise 2b: increase object count every time an object is set to active
	++m_objectCount;
	m_goList.back()->active = true;
	return m_goList.back();
}

void SceneAssignment2::ReturnGO(GameObject *go)
{
	//Exercise 3: implement ReturnGO()
	go->active = false;
	--m_objectCount;
}

void SceneAssignment2::MakeThinWall(float x, float y, float angleDeg, float thickness, float height)
{
	GameObject* wall = FetchGO();
	wall->type = GameObject::GO_WALL;
	wall->pos.Set(x, y, 0);
	wall->normal.Set(cosf(Math::DegreeToRadian(angleDeg)), sinf(Math::DegreeToRadian(angleDeg)));
	wall->scale.Set(thickness, height, 1.f);
	wall->vel.Set(0, 0, 0);

	Vector3 np(wall->normal.y, -wall->normal.x, 0);	// Vector tangent to wall
	Vector3 pillarPos[2] = { wall->pos + np * (height / 2.f), wall->pos - np * (height / 2.f) };

	for (int i = 0; i < 2; ++i)
	{
		GameObject* pillar = FetchGO();
		pillar->type = GameObject::GO_PILLAR;
		pillar->pos = pillarPos[i];
		pillar->scale.Set(thickness * 0.5f, thickness * 0.5f, thickness * 0.5f);
		pillar->vel.Set(0, 0, 0);
	}
}

void SceneAssignment2::MakeThickWall(float x, float y, float angleDeg, float thickness, float height)
{
	GameObject* wall = FetchGO();
	wall->type = GameObject::GO_WALL;
	wall->pos.Set(x, y, 0);
	wall->normal.Set(sinf(Math::DegreeToRadian(angleDeg)), cosf(Math::DegreeToRadian(angleDeg)));
	wall->scale.Set(height, thickness, 1.f);
	wall->vel.Set(0, 0, 0);

	wall = FetchGO();
	wall->type = GameObject::GO_WALL;
	wall->pos.Set(x, y, 0);
	wall->normal.Set(sinf(Math::DegreeToRadian(angleDeg + 90)), cosf(Math::DegreeToRadian(angleDeg + 90)));
	wall->scale.Set(thickness, height, 1.f);
	wall->vel.Set(0, 0, 0);

	Vector3 n(wall->normal);
	Vector3 np(wall->normal.y, -wall->normal.x, 0);//vector tangent to wall
	Vector3 pillarPos[4] = { wall->pos + np * (height / 2.f) + n * (thickness / 2.f), wall->pos - np * (height / 2.f) + n * (thickness / 2.f) ,wall->pos + np * (height / 2.f) - n * (thickness / 2.f), wall->pos - np * (height / 2.f) - n * (thickness / 2.f) };

	for (int i = 0; i < 4; ++i)
	{
		GameObject* pillar = FetchGO();
		pillar->type = GameObject::GO_PILLAR;
		pillar->pos = pillarPos[i];
		//size of pillar is determined by wall thickness 
		pillar->scale.Set(0.001f, 0.001f, 0.001f);
		pillar->vel.Set(0, 0, 0);
	}
}

void SceneAssignment2::MakeBrick(float x, float y, float angleDeg, float thickness, float height, SIDE side)
{
	Brick* brick = new Brick();
	m_goList.push_back(brick);
	brick->active = true;
	brick->type = GameObject::GO_BRICK;
	brick->pos.Set(x, y, 0);
	brick->normal.Set(sinf(Math::DegreeToRadian(angleDeg)), cosf(Math::DegreeToRadian(angleDeg)));
	brick->scale.Set(height, thickness, 1.f);
	brick->vel.Set(0, 0, 0);
	brick->isPowerup = false;
	brick->side = side;

	Brick* brick2 = new Brick();
	m_goList.push_back(brick2);
	brick2->active = true;
	brick2->type = GameObject::GO_BRICK;
	brick2->pos.Set(x, y, 0);
	brick2->normal.Set(sinf(Math::DegreeToRadian(angleDeg + 90)), cosf(Math::DegreeToRadian(angleDeg + 90)));
	brick2->scale.Set(thickness, height, 1.f);
	brick2->vel.Set(0, 0, 0);
	brick2->isPowerup = false;
	brick2->side = side;

	brick->insertBrick(brick2);
	brick2->insertBrick(brick);

	Vector3 n(brick2->normal);
	Vector3 np(brick2->normal.y, -brick2->normal.x, 0);//vector tangent to wall
	Vector3 pillarPos[4] = { brick2->pos + np * (height / 2.f) + n * (thickness / 2.f), brick2->pos - np * (height / 2.f) + n * (thickness / 2.f) ,brick2->pos + np * (height / 2.f) - n * (thickness / 2.f), brick2->pos - np * (height / 2.f) - n * (thickness / 2.f) };

	Brick* pillar[4] = {nullptr};
	for (int i = 0; i < 4; ++i)
	{
		pillar[i] = new Brick();
		m_goList.push_back(pillar[i]);
		pillar[i]->active = true;
		pillar[i]->type = GameObject::GO_BRICK_CORNER;
		pillar[i]->pos = pillarPos[i];
		//size of pillar is determined by wall thickness 
		pillar[i]->scale.Set(0.001f, 0.001f, 0.001f);
		pillar[i]->vel.Set(0, 0, 0);
		pillar[i]->isPowerup = false;
		pillar[i]->side = side;
		brick->insertBrick(pillar[i]);
		brick2->insertBrick(pillar[i]);
	}
	for (int i = 0; i < 4; ++i)
	{
		pillar[i]->insertBrick(brick);
		pillar[i]->insertBrick(brick2);
		for (int j = 0; j < 4; ++j)
		{
			if (pillar[i] != pillar[j])
				pillar[i]->insertBrick(pillar[j]);
		}
	}
}

void SceneAssignment2::MakeMultiplyBrick(float x, float y, float angleDeg, float thickness, float height, SIDE side)
{
	PowerupBrick* brick = new PowerupBrick();
	m_goList.push_back(brick);
	brick->active = true;
	brick->type = GameObject::GO_BRICK;
	brick->pos.Set(x, y, 0);
	brick->normal.Set(sinf(Math::DegreeToRadian(angleDeg)), cosf(Math::DegreeToRadian(angleDeg)));
	brick->scale.Set(height, thickness, 1.f);
	brick->vel.Set(0, 0, 0);
	brick->isPowerup = true;
	brick->powerupType = PowerupBrick::PU_MULTIPLY;
	brick->side = side;

	PowerupBrick* brick2 = new PowerupBrick();
	m_goList.push_back(brick2);
	brick2->active = true;
	brick2->type = GameObject::GO_BRICK;
	brick2->pos.Set(x, y, 0);
	brick2->normal.Set(sinf(Math::DegreeToRadian(angleDeg + 90)), cosf(Math::DegreeToRadian(angleDeg + 90)));
	brick2->scale.Set(thickness, height, 1.f);
	brick2->vel.Set(0, 0, 0);
	brick2->isPowerup = true;
	brick2->powerupType = PowerupBrick::PU_MULTIPLY;
	brick2->side = side;

	brick->insertBrick(brick2);
	brick2->insertBrick(brick);

	Vector3 n(brick2->normal);
	Vector3 np(brick2->normal.y, -brick2->normal.x, 0);//vector tangent to wall
	Vector3 pillarPos[4] = { brick2->pos + np * (height / 2.f) + n * (thickness / 2.f), brick2->pos - np * (height / 2.f) + n * (thickness / 2.f) ,brick2->pos + np * (height / 2.f) - n * (thickness / 2.f), brick2->pos - np * (height / 2.f) - n * (thickness / 2.f) };

	PowerupBrick* pillar[4] = { nullptr };
	for (int i = 0; i < 4; ++i)
	{
		pillar[i] = new PowerupBrick();
		m_goList.push_back(pillar[i]);
		pillar[i]->active = true;
		pillar[i]->type = GameObject::GO_BRICK_CORNER;
		pillar[i]->pos = pillarPos[i];
		//size of pillar is determined by wall thickness 
		pillar[i]->scale.Set(0.001f, 0.001f, 0.001f);
		pillar[i]->vel.Set(0, 0, 0);
		pillar[i]->powerupType = PowerupBrick::PU_MULTIPLY;
		pillar[i]->isPowerup = true;
		pillar[i]->side = side;
		brick->insertBrick(pillar[i]);
		brick2->insertBrick(pillar[i]);
	}
	for (int i = 0; i < 4; ++i)
	{
		pillar[i]->insertBrick(brick);
		pillar[i]->insertBrick(brick2);
		for (int j = 0; j < 4; ++j)
		{
			if (pillar[i] != pillar[j])
				pillar[i]->insertBrick(pillar[j]);
		}
	}
}

void SceneAssignment2::MakeGravBrick(float x, float y, float angleDeg, float thickness, float height, SIDE side)
{
	PowerupBrick* brick = new PowerupBrick();
	m_goList.push_back(brick);
	brick->active = true;
	brick->type = GameObject::GO_BRICK;
	brick->pos.Set(x, y, 0);
	brick->normal.Set(sinf(Math::DegreeToRadian(angleDeg)), cosf(Math::DegreeToRadian(angleDeg)));
	brick->scale.Set(height, thickness, 1.f);
	brick->vel.Set(0, 0, 0);
	brick->isPowerup = true;
	brick->powerupType = PowerupBrick::PU_INCREASE_GRAV;
	brick->side = side;

	PowerupBrick* brick2 = new PowerupBrick();
	m_goList.push_back(brick2);
	brick2->active = true;
	brick2->type = GameObject::GO_BRICK;
	brick2->pos.Set(x, y, 0);
	brick2->normal.Set(sinf(Math::DegreeToRadian(angleDeg + 90)), cosf(Math::DegreeToRadian(angleDeg + 90)));
	brick2->scale.Set(thickness, height, 1.f);
	brick2->vel.Set(0, 0, 0);
	brick2->isPowerup = true;
	brick2->powerupType = PowerupBrick::PU_INCREASE_GRAV;
	brick2->side = side;

	brick->insertBrick(brick2);
	brick2->insertBrick(brick);

	Vector3 n(brick2->normal);
	Vector3 np(brick2->normal.y, -brick2->normal.x, 0);//vector tangent to wall
	Vector3 pillarPos[4] = { brick2->pos + np * (height / 2.f) + n * (thickness / 2.f), brick2->pos - np * (height / 2.f) + n * (thickness / 2.f) ,brick2->pos + np * (height / 2.f) - n * (thickness / 2.f), brick2->pos - np * (height / 2.f) - n * (thickness / 2.f) };

	PowerupBrick* pillar[4] = { nullptr };
	for (int i = 0; i < 4; ++i)
	{
		pillar[i] = new PowerupBrick();
		m_goList.push_back(pillar[i]);
		pillar[i]->active = true;
		pillar[i]->type = GameObject::GO_BRICK_CORNER;
		pillar[i]->pos = pillarPos[i];
		//size of pillar is determined by wall thickness 
		pillar[i]->scale.Set(0.001f, 0.001f, 0.001f);
		pillar[i]->vel.Set(0, 0, 0);
		pillar[i]->powerupType = PowerupBrick::PU_INCREASE_GRAV;
		pillar[i]->isPowerup = true;
		pillar[i]->side = side;
		brick->insertBrick(pillar[i]);
		brick2->insertBrick(pillar[i]);
	}
	for (int i = 0; i < 4; ++i)
	{
		pillar[i]->insertBrick(brick);
		pillar[i]->insertBrick(brick2);
		for (int j = 0; j < 4; ++j)
		{
			if (pillar[i] != pillar[j])
				pillar[i]->insertBrick(pillar[j]);
		}
	}
}

void SceneAssignment2::MakeBumper(float x, float y, float angleDeg, float thickness, float height)
{
	GameObject* wall = FetchGO();
	wall->type = GameObject::GO_BUMPER;
	wall->pos.Set(x, y, 0);
	wall->normal.Set(sinf(Math::DegreeToRadian(angleDeg)), cosf(Math::DegreeToRadian(angleDeg)));
	wall->scale.Set(height, thickness, 1.f);
	wall->vel.Set(0, 0, 0);

	wall = FetchGO();
	wall->type = GameObject::GO_BUMPER;
	wall->pos.Set(x, y, 0);
	wall->normal.Set(sinf(Math::DegreeToRadian(angleDeg + 90)), cosf(Math::DegreeToRadian(angleDeg + 90)));
	wall->scale.Set(thickness, height, 1.f);
	wall->vel.Set(0, 0, 0);

	Vector3 n(wall->normal);
	Vector3 np(wall->normal.y, -wall->normal.x, 0);//vector tangent to wall
	Vector3 pillarPos[4] = { wall->pos + np * (height / 2.f) + n * (thickness / 2.f), wall->pos - np * (height / 2.f) + n * (thickness / 2.f) ,wall->pos + np * (height / 2.f) - n * (thickness / 2.f), wall->pos - np * (height / 2.f) - n * (thickness / 2.f) };

	for (int i = 0; i < 4; ++i)
	{
		GameObject* pillar = FetchGO();
		pillar->type = GameObject::GO_BUMPER_PILLAR;
		pillar->pos = pillarPos[i];
		//size of pillar is determined by wall thickness 
		pillar->scale.Set(0.001f, 0.001f, 0.001f);
		pillar->vel.Set(0, 0, 0);
	}
}

void SceneAssignment2::MakePillarBumper(float x, float y, float thickness)
{
	GameObject* pillar = FetchGO();
	pillar->type = GameObject::GO_BUMPER_PILLAR;
	pillar->pos.Set(x, y);
	pillar->scale.Set(thickness, thickness);
	pillar->vel.SetZero();
}

void SceneAssignment2::MakeOneWall(float x, float y, float angleDeg, float thickness, float height)
{
	GameObject* wall = FetchGO();
	wall->type = GameObject::GO_WALL;
	wall->pos.Set(x, y, 0);
	wall->normal.Set(sinf(Math::DegreeToRadian(angleDeg)), cosf(Math::DegreeToRadian(angleDeg)));
	wall->scale.Set(height, thickness, 1.f);
	wall->vel.Set(0, 0, 0);
}

bool SceneAssignment2::CheckCollision(GameObject* go1, GameObject* go2, float dt)
{
	if (go1->type == GameObject::GO_BALL && 
		(go2->type == GameObject::GO_BALL || go2->type == GameObject::GO_PILLAR || go2->type == GameObject::GO_BRICK_CORNER || go2->type == GameObject::GO_BUMPER_PILLAR))
	{
		// Prevent internal collision
		Vector3 relVel = go1->vel - go2->vel;
		Vector3 collisionNormal = go2->pos - go1->pos;
		if (relVel.Dot(collisionNormal) <= 0)
			return false;

		return (go1->pos - go2->pos).LengthSquared() <= (go1->scale.x + go2->scale.x) * (go1->scale.x + go2->scale.x);
	}
	else if (go1->type == GameObject::GO_BALL && (go2->type == GameObject::GO_WALL || go2->type == GameObject::GO_BRICK || go2->type == GameObject::GO_BUMPER))
	{
		Vector3 N = go2->normal;
		// Flip the wall normal if it's needed
		if ((go2->pos - go1->pos).Dot(N) < 0)
			N = -N;

		if (N.Dot(go1->vel) < 0)
			return false;

		Vector3 NP = Vector3(-N.y, N.x, 0);
		Vector3 w0_b1 = go2->pos - go1->pos;
		float r = go1->scale.x;
		float h_2 = go2->scale.x * 0.5f;
		float l_2 = go2->scale.y * 0.5f;

		if ((abs((w0_b1).Dot(N)) < r + h_2) && abs((w0_b1).Dot(NP)) < l_2)
			return true;
	}
	else if (go1->type == GameObject::GO_BALL && go2->type == GameObject::GO_FLIPPER)
	{
		if (go2 == flipperL || go2 == flipperL2)
		{
			Vector3 N = go2->normal;

			if ((go2->pos - go1->pos).Dot(N) < 0)
			{
				N = -N;
			}

			if (N.Dot(go1->vel) < 0)
				return false;

			Vector3 NP = Vector3(-N.y, N.x, 0);
			Vector3 w0_b1 = (go2->pos + NP * go2->scale.y * 0.5f) - go1->pos;
			float r = go1->scale.x;
			float h_2 = go2->scale.x * 0.5f;
			float l_2 = go2->scale.y * 0.5f;

			if (abs((w0_b1).Dot(N)) < r + h_2 &&
				abs((w0_b1).Dot(NP)) < l_2)
			{
				return true;
			}
		}
		else
		{
			Vector3 N = go2->normal;

			if ((go2->pos - go1->pos).Dot(N) < 0)
			{
				N = -N;
			}

			if (N.Dot(go1->vel) < 0)
				return false;

			Vector3 NP = Vector3(-N.y, N.x, 0);
			Vector3 w0_b1 = (go2->pos + NP * go2->scale.y * -0.5f) - go1->pos;
			float r = go1->scale.x;
			float h_2 = go2->scale.x * 0.5f;
			float l_2 = go2->scale.y * 0.5f;

			if (abs((w0_b1).Dot(N)) < r + h_2 &&
				abs((w0_b1).Dot(NP)) < l_2)
			{
				return true;
			}
		}
	}
	return false;
}

void SceneAssignment2::CollisionResponse(GameObject* go1, GameObject* go2)
{
	if (go1->type == GameObject::GO_BALL && go2->type == GameObject::GO_BALL)
	{
		//Exercise 8b: store values in auditing variables
		m1 = go1->mass;
		m2 = go2->mass;
		u1 = go1->vel;
		u2 = go2->vel;

		//Exercise 10: handle collision using momentum swap instead
		Vector3 collisionNormal = go1->pos - go2->pos;
		float distSq = collisionNormal.LengthSquared();
		float dp = (u1 - u2).Dot(go1->pos - go2->pos);
		v1 = u1 - ((2 * m2) / (m1 + m2)) * (dp / distSq) * collisionNormal;
		v2 = u2 - ((2 * m1) / (m1 + m2)) * (dp / distSq) * -collisionNormal;

		go1->vel = v1 * 0.9f;
		go2->vel = v2 * 0.9f;
	}
	else if (go1->type == GameObject::GO_BALL && go2->type == GameObject::GO_WALL)
	{
		go1->vel -= 2 * go1->vel.Dot(go2->normal) * go2->normal;
		go1->vel *= 0.9f;
	}
	else if (go1->type == GameObject::GO_BALL && go2->type == GameObject::GO_FLIPPER)
	{
		Flipper* flipper = static_cast<Flipper*>(go2);
		if (flipper->angularVelocity == 0.f)
		{
			go1->vel -= 2 * go1->vel.Dot(go2->normal) * flipper->normal;
			go1->vel *= 0.9f;
		}
		else
		{
			float rotationalEnergy = 0.5f * flipper->momentOfInertia * flipper->angularVelocity * flipper->angularVelocity;
			go1->vel.y = 200.f * sqrt(rotationalEnergy * 2 / go1->mass);
		}
	}
	else if (go1->type == GameObject::GO_BALL && go2->type == GameObject::GO_PILLAR)
	{
		Vector3 normal = (go1->pos - go2->pos).Normalized();
		go1->vel -= 2 * go1->vel.Dot(normal) * normal;
		go1->vel *= 0.9f;
	}
	else if (go1->type == GameObject::GO_BALL && go2->type == GameObject::GO_BRICK)
	{
		go1->vel -= 2 * go1->vel.Dot(go2->normal) * go2->normal;
		go1->vel *= 0.9f;

		Brick* brick = static_cast<Brick*>(go2);
		brick->removeAllBricks();
		go2->active = false;
		if (go1->pos.x < m_worldWidth / 2)
			--bricksLeftL;
		else
			--bricksLeftR;

		if (brick->isPowerup)
		{
			PowerupBrick* powerup = static_cast<PowerupBrick*>(go2);
			if (powerup->powerupType == PowerupBrick::PU_MULTIPLY)
				powerup->multiplyBall(go1, m_goList);
			else
			{
				if (powerup->side == LEFT)
				{
					powerup->increaseOppGrav(m_gravity2);
					gravIncreasedL = true;
				}
				else
				{
					powerup->increaseOppGrav(m_gravity);
					gravIncreasedR = true;
				}
			}
		}
	}
	else if (go1->type == GameObject::GO_BALL && go2->type == GameObject::GO_BRICK_CORNER)
	{
		Vector3 normal = (go1->pos - go2->pos).Normalized();
		go1->vel -= 2 * go1->vel.Dot(normal) * normal;
		go1->vel *= 0.9f;

		Brick* brick = static_cast<Brick*>(go2);
		brick->removeAllBricks();
		go2->active = false;

		if (go1->pos.x < m_worldWidth / 2)
			--bricksLeftL;
		else
			--bricksLeftR;

		if (brick->isPowerup)
		{
			PowerupBrick* powerup = static_cast<PowerupBrick*>(go2);
			if (powerup->powerupType == PowerupBrick::PU_MULTIPLY)
				powerup->multiplyBall(go1, m_goList);
			else
			{
				if (powerup->side == LEFT)
				{
					powerup->increaseOppGrav(m_gravity2);
					gravIncreasedL = true;
				}
				else
				{
					powerup->increaseOppGrav(m_gravity);
					gravIncreasedR = true;
				}
			}
		}
	}
	else if (go1->type == GameObject::GO_BALL && go2->type == GameObject::GO_BUMPER)
	{
		go1->vel -= 2 * go1->vel.Dot(go2->normal) * go2->normal;
		go1->vel *= Math::RandFloatMinMax(1.f, 1.2f);
	}
	else if (go1->type == GameObject::GO_BALL && go2->type == GameObject::GO_BUMPER_PILLAR)
	{
		Vector3 normal = (go1->pos - go2->pos).Normalized();
		go1->vel -= 2 * go1->vel.Dot(normal) * normal;
		go1->vel *= Math::RandFloatMinMax(1.f, 1.2f);
	}
}

void SceneAssignment2::RespawnBall(SIDE side)
{
	if (side == LEFT)
	{
		GameObject* ball = FetchGO();
		ball->type = GameObject::GO_BALL;
		ball->pos.Set(m_worldWidth / 2 - 3.5f, m_worldHeight - 5.f);
		ball->mass = 8.f;
		ball->scale.Set(2, 2, 2);
		ball->vel.SetZero();
	}
	else
	{
		GameObject* ball = FetchGO();
		ball->type = GameObject::GO_BALL;
		ball->pos.Set(m_worldWidth / 2 + 3.5f, m_worldHeight - 5.f);
		ball->mass = 8.f;
		ball->scale.Set(2, 2, 2);
		ball->vel.SetZero();
	}
}

void SceneAssignment2::Update(double dt)
{
	SceneBase::Update(dt);

	if (gameStarted && timeAfterStarted == 0)
	{
		stopwatch->startTimer();
	}
	if (timeAfterStarted > 3 && stopwatch)
	{
		RespawnBall(LEFT);
		RespawnBall(RIGHT);

		delete stopwatch;
		stopwatch = NULL;
	}

	if (bricksLeftL == 0)
		leftWins = true;
	if (bricksLeftR == 0)
		rightWins = true;

	flipperL->force.SetZero();
	flipperL->torque.SetZero();
	flipperR->force.SetZero();
	flipperR->torque.SetZero();
	flipperL2->force.SetZero();
	flipperL2->torque.SetZero();
	flipperR2->force.SetZero();
	flipperR2->torque.SetZero();
	if (Application::IsKeyPressed(VK_SPACE))
	{
		flipperL->force += 10 * flipperL->normal;
		flipperL->torque = Vector3(1, -1, 0).Cross(Vector3(0, 2, 0));
		flipperR->force += 10 * flipperR->normal;
		flipperR->torque = Vector3(-1, -1, 0).Cross(Vector3(0, 2, 0));
	}
	else
	{
		flipperL->force += 10 * flipperL->normal;
		flipperL->torque = Vector3(-1, -1, 0).Cross(Vector3(0, 2, 0));
		flipperR->force += 10 * flipperR->normal;
		flipperR->torque = Vector3(1, -1, 0).Cross(Vector3(0, 2, 0));
	}
	if (Application::IsKeyPressed(VK_SHIFT))
	{
		flipperL2->force += 10 * flipperL2->normal;
		flipperL2->torque = Vector3(1, -1, 0).Cross(Vector3(0, 2, 0));
		flipperR2->force += 10 * flipperR2->normal;
		flipperR2->torque = Vector3(-1, -1, 0).Cross(Vector3(0, 2, 0));
	}
	else
	{
		flipperL2->force += 10 * flipperL2->normal;
		flipperL2->torque = Vector3(-1, -1, 0).Cross(Vector3(0, 2, 0));
		flipperR2->force += 10 * flipperR2->normal;
		flipperR2->torque = Vector3(1, -1, 0).Cross(Vector3(0, 2, 0));
	}

	//Physics Simulation Section
	float alpha = flipperL->torque.z / flipperL->momentOfInertia;
	if (alpha == 0 && (flipperL->angularVelocity > 0 || flipperL->angularVelocity < 0))
		alpha = flipperL->angularVelocity * -1;
	flipperL->angularVelocity += alpha * m_speed * dt;
	float angle = flipperL->angularVelocity * m_speed * dt;
	flipperL->normal.Set(flipperL->normal.x * cosf(angle) - flipperL->normal.y * sinf(angle), flipperL->normal.x * sinf(angle) + flipperL->normal.y * cosf(angle), 0);
	if (flipperL->normal.x < -cosf(Math::DegreeToRadian(60)) && flipperL->normal.y < sinf(Math::DegreeToRadian(60)))
	{
		flipperL->normal.Set(-cosf(Math::DegreeToRadian(60)), sinf(Math::DegreeToRadian(60)));
		flipperL->angularVelocity = 0.f;
	}
	else if (flipperL->normal.x > cosf(Math::DegreeToRadian(60)) && flipperL->normal.y < sinf(Math::DegreeToRadian(60)))
	{
		flipperL->normal.Set(cosf(Math::DegreeToRadian(60)), sinf(Math::DegreeToRadian(60)));
		flipperL->angularVelocity = 0.f;
	}

	float alpha2 = flipperR->torque.z / flipperR->momentOfInertia;
	if (alpha2 == 0 && (flipperR->angularVelocity > 0 || flipperR->angularVelocity < 0))
		alpha2 = flipperR->angularVelocity * -1;
	flipperR->angularVelocity += alpha2 * m_speed * dt;
	float angle2 = flipperR->angularVelocity * m_speed * dt;
	flipperR->normal.Set(flipperR->normal.x * cosf(angle2) - flipperR->normal.y * sinf(angle2), flipperR->normal.x * sinf(angle2) + flipperR->normal.y * cosf(angle2), 0);
	if (flipperR->normal.x < -cosf(Math::DegreeToRadian(60)) && flipperR->normal.y < sinf(Math::DegreeToRadian(60)))
	{
		flipperR->normal.Set(-cosf(Math::DegreeToRadian(60)), sinf(Math::DegreeToRadian(60)));
		flipperR->angularVelocity = 0.f;
	}
	else if (flipperR->normal.x > cosf(Math::DegreeToRadian(60)) && flipperR->normal.y < sinf(Math::DegreeToRadian(60)))
	{
		flipperR->normal.Set(cosf(Math::DegreeToRadian(60)), sinf(Math::DegreeToRadian(60)));
		flipperR->angularVelocity = 0.f;
	}

	float alpha3 = flipperL2->torque.z / flipperL2->momentOfInertia;
	if (alpha3 == 0 && (flipperL2->angularVelocity > 0 || flipperL2->angularVelocity < 0))
		alpha3 = flipperL2->angularVelocity * -1;
	flipperL2->angularVelocity += alpha3 * m_speed * dt;
	float angle3 = flipperL2->angularVelocity * m_speed * dt;
	flipperL2->normal.Set(flipperL2->normal.x * cosf(angle3) - flipperL2->normal.y * sinf(angle3), flipperL2->normal.x * sinf(angle3) + flipperL2->normal.y * cosf(angle3), 0);
	if (flipperL2->normal.x < -cosf(Math::DegreeToRadian(60)) && flipperL2->normal.y < sinf(Math::DegreeToRadian(60)))
	{
		flipperL2->normal.Set(-cosf(Math::DegreeToRadian(60)), sinf(Math::DegreeToRadian(60)));
		flipperL2->angularVelocity = 0.f;
	}
	else if (flipperL2->normal.x > cosf(Math::DegreeToRadian(60)) && flipperL2->normal.y < sinf(Math::DegreeToRadian(60)))
	{
		flipperL2->normal.Set(cosf(Math::DegreeToRadian(60)), sinf(Math::DegreeToRadian(60)));
		flipperL2->angularVelocity = 0.f;
	}

	float alpha4 = flipperR2->torque.z / flipperR2->momentOfInertia;
	if (alpha4 == 0 && (flipperR2->angularVelocity > 0 || flipperR2->angularVelocity < 0))
		alpha4 = flipperR2->angularVelocity * -1;
	flipperR2->angularVelocity += alpha4 * m_speed * dt;
	float angle4 = flipperR2->angularVelocity * m_speed * dt;
	flipperR2->normal.Set(flipperR2->normal.x * cosf(angle4) - flipperR2->normal.y * sinf(angle4), flipperR2->normal.x * sinf(angle4) + flipperR2->normal.y * cosf(angle4), 0);
	if (flipperR2->normal.x < -cosf(Math::DegreeToRadian(60)) && flipperR2->normal.y < sinf(Math::DegreeToRadian(60)))
	{
		flipperR2->normal.Set(-cosf(Math::DegreeToRadian(60)), sinf(Math::DegreeToRadian(60)));
		flipperR2->angularVelocity = 0.f;
	}
	else if (flipperR2->normal.x > cosf(Math::DegreeToRadian(60)) && flipperR2->normal.y < sinf(Math::DegreeToRadian(60)))
	{
		flipperR2->normal.Set(cosf(Math::DegreeToRadian(60)), sinf(Math::DegreeToRadian(60)));
		flipperR2->angularVelocity = 0.f;
	}

	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			if (go->type == GameObject::GO_BALL)
			{
				if (go->pos.x < m_worldWidth / 2)
					go->vel += m_gravity * 4.f * m_speed * dt;
				else
					go->vel += m_gravity2 * 4.f * m_speed * dt;

				if (go->pos.y < 5.f)
				{
					go->active = false;
					if (go->pos.x < m_worldWidth / 2)	// Left ball
						RespawnBall(LEFT);
					else
						RespawnBall(RIGHT);
				}
			}
			go->pos += go->vel * m_speed * dt;
			//Exercise 7: handle out of bound game objects
			if ((go->pos.x < go->scale.x && go->vel.x < 0) || go->pos.x > m_worldWidth - go->scale.x && go->vel.x > 0)
				go->vel.x = -go->vel.x;
			if ((go->pos.y < go->scale.y && go->vel.y < 0) || go->pos.y > m_worldHeight - go->scale.y && go->vel.y > 0)
				go->vel.y = -go->vel.y;
			for (std::vector<GameObject*>::iterator it2 = it + 1; it2 != m_goList.end(); ++it2)
			{
				GameObject* go2 = (GameObject*)*it2;
				if (go2->active)
				{
					// Ensure that ball is always the first ptr
					GameObject* ball = go;
					GameObject* other = go2;
					if (ball->type != GameObject::GO_BALL)
					{
						ball = go2;
						other = go;
					}

					if (CheckCollision(ball, other, dt))
					{
						CollisionResponse(ball, other);
					}
				}
			}
		}
	}

	if (stopwatch)
		timeAfterStarted += stopwatch->getElapsedTime();

	if (gravIncreasedL)
	{
		m_gravL += dt;
		if (m_gravL > 10.f)
		{
			gravIncreasedL = false;
			m_gravity2.Set(0, -9.8f, 0);
		}
	}
	if (gravIncreasedR)
	{
		m_gravR += dt;
		if (m_gravR > 10.f)
		{
			gravIncreasedR = false;
			m_gravity.Set(0, -9.8f, 0);
		}
	}
}


void SceneAssignment2::RenderGO(GameObject *go)
{
	switch(go->type)
	{
	case GameObject::GO_BALL:
		//Exercise 4: render a sphere using scale and pos
		modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_BALL], true);
		modelStack.PopMatrix();
		//Exercise 11: think of a way to give balls different colors
		break;
	case GameObject::GO_WALL:
		modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Rotate(Math::RadianToDegree(atan2f(go->normal.y, go->normal.x)), 0, 0, 1);
			modelStack.Scale(go->scale.x * 0.5f, go->scale.y * 0.5f, go->scale.z);
			RenderMesh(meshList[GEO_CUBE], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_PILLAR:
		modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			modelStack.Rotate(90, 1, 0, 0);
			RenderMesh(meshList[GEO_CYLINDER], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BRICK:
		if (static_cast<Brick*>(go)->isPowerup)
		{
			delete meshList[GEO_CUBE];
			if (static_cast<PowerupBrick*>(go)->powerupType == PowerupBrick::PU_MULTIPLY)
				meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(0.266f, 0.333f, 0.353f), 2.f);
			else
				meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(0, 1.f, 0), 2.f);
		}
		modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Rotate(Math::RadianToDegree(atan2f(go->normal.y, go->normal.x)), 0, 0, 1);
			modelStack.Scale(go->scale.x * 0.5f, go->scale.y * 0.5f, go->scale.z * 0.5f);
			RenderMesh(meshList[GEO_CUBE], true);
		modelStack.PopMatrix();
		delete meshList[GEO_CUBE];
		meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 1, 1), 2.f);
		break;
	case GameObject::GO_BRICK_CORNER:
		modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			modelStack.Rotate(90, 1, 0, 0);
			RenderMesh(meshList[GEO_CYLINDER], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BUMPER:
		modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Rotate(Math::RadianToDegree(atan2f(go->normal.y, go->normal.x)), 0, 0, 1);
			modelStack.Scale(go->scale.x * 0.5f, go->scale.y * 0.5f, go->scale.z * 0.5f);
			RenderMesh(meshList[GEO_CUBE], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_FLIPPER:
		modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Rotate(Math::RadianToDegree(atan2f(go->normal.y, go->normal.x)), 0, 0, 1);
			if (go == flipperL || go == flipperL2)
				modelStack.Translate(0, 0.5f * -go->scale.y, 0);
			else
				modelStack.Translate(0, 0.5f * go->scale.y, 0);
			modelStack.Scale(go->scale.x * 0.5f, go->scale.y * 0.5f, go->scale.z * 0.5f);
			RenderMesh(meshList[GEO_CUBE], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BUMPER_PILLAR:
		modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			modelStack.Rotate(90, 1, 0, 0);
			RenderMesh(meshList[GEO_CYLINDER], true);
		modelStack.PopMatrix();
		break;
	}
}

void SceneAssignment2::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);
	
	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
						camera.position.x, camera.position.y, camera.position.z,
						camera.target.x, camera.target.y, camera.target.z,
						camera.up.x, camera.up.y, camera.up.z
					);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
	
	RenderMesh(meshList[GEO_AXES], false);

	if (m_ghost->active)
	{
		modelStack.PushMatrix();
			modelStack.Translate(m_ghost->pos.x, m_ghost->pos.y, m_ghost->pos.z);
			modelStack.Scale(m_ghost->scale.x, m_ghost->scale.y, m_ghost->scale.z);
			RenderMesh(meshList[GEO_BALL], false);
		modelStack.PopMatrix();
	}

	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			RenderGO(go);
		}
	}

	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

	ss.str("");
	ss.precision(2);
	ss << "Bricks Left: " << bricksLeftL;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 2, 5, 58);

	ss.str("");
	ss.precision(2);
	ss << "Bricks Left: " << bricksLeftR;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 2, 47, 58);

	ss.str("");
	ss.precision(2);
	ss << "Press 'Esc' to quit.";

	if (leftWins)
	{
		modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, 3.f);
			modelStack.Scale(1.f, .5f, .5f);
			RenderMesh(meshList[GEO_WIN_1], true);
		modelStack.PopMatrix();

		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 12, 20);
	}
	else if (rightWins)
	{
		modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, 3.f);
			modelStack.Scale(1.f, .5f, .5f);
			RenderMesh(meshList[GEO_WIN_2], true);
		modelStack.PopMatrix();

		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 12, 20);
	}
}

void SceneAssignment2::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if(m_ghost)
	{
		delete m_ghost;
		m_ghost = NULL;
	}
}
