#pragma once

#include "SceneBase.h"
#include "Map.h"
#include "Guns.h"
#include "Player.h"
#include "MeshList.h"
#include "Renderer.h"
#include "Projectile.h"
#include "Collectible.h"
#include "SceneGUI.h"
#include "SingletonPattern.h"
#include "Boss.h"

class SceneTest : public SceneBase, public SingletonPattern<SceneTest>
{
	friend SingletonPattern<SceneTest>;
protected:
	// Variables
	float m_worldWidth;
	float m_worldHeight;
	Map* map;

	bool NearXEdge = false;
	bool NearYEdge = false;

	SceneGUI* sceneGUI;

	// Constructor
	SceneTest();
	// Destructor
	virtual ~SceneTest();

public:
	//Renderer* renderer;

	MeshList* meshlist;
	
	GunComponent* simpleGun;
	GunComponent* revolver;
	GunComponent* shotgun;
	GunComponent* minigun;

	std::vector <Vector3> activeTileList;
	std::vector <Vector3> HazardTileList;
	std::vector<Projectile*> projectileList;
	std::vector<BaseEnemy*> enemyList;
	std::vector<Collectible*> collectibleList;

	Player* player;
	Boss* boss;

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

