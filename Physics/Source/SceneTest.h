#pragma once

#include "SceneBase.h"
#include "Map.h"
#include "Guns.h"

class SceneTest : public SceneBase
{
private:
	// Variables
	float m_worldWidth;
	float m_worldHeight;
	Map* map;
	GunComponent* simpleGun;
	GunComponent* revolver;
	GunComponent* shotgun;
	GunComponent* minigun;
	GunComponent* activeGun;

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

