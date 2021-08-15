#pragma once

#include "SceneBase.h"

class SceneTest : public SceneBase
{
private:
	// Variables
	float m_worldWidth;
	float m_worldHeight;

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

