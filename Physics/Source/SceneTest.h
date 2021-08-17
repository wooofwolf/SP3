#pragma once

#include "SceneBase.h"
#include "Map.h"
#include "Guns.h"

#include "BaseEnemy.h"
#include "Player.h"

#include "MeshList.h"

#include "Renderer.h"

class SceneTest : public SceneBase
{
private:
	// Variables
	float m_worldWidth;
	float m_worldHeight;
	Map* map;

public:
	Renderer* renderer;

	MeshList* meshlist;
	
	GunComponent* simpleGun;
	GunComponent* revolver;
	GunComponent* shotgun;
	GunComponent* minigun;
	GunComponent* activeGun;

	BaseEnemy* baseEnemy;
	BaseEnemy* cavemanMelee;
	BaseEnemy* cavemeanRanged;
	BaseEnemy* Knight;
	BaseEnemy* Archer;
	BaseEnemy* Cyborg;
	BaseEnemy* Sniper;

	std::vector<Entity*> EntityList;
	std::vector<BaseEnemy*> BasicEnemyList;
	std::vector <Vector3> activeTileList;

	Player* player;
public:
	// Constructor
	SceneTest();
	// Destructor
	~SceneTest();

	BaseEnemy* SpawnEnemies();
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

