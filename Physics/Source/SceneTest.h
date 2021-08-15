#pragma once

#include "SceneBase.h"
#include "Map.h"

class SceneTest : public SceneBase
{
private:
	// Variables
	float m_worldWidth;
	float m_worldHeight;
	Map* map;

public:
	// Constructor
	SceneTest();
	// Destructor
	~SceneTest();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

