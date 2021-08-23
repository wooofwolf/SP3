#pragma once

#include "SceneBase.h"
#include "Map.h"
#include "Guns.h"
#include "Player.h"
#include "MeshList.h"
#include "Renderer.h"
//#include "Projectile.h"
#include "Collectible.h"
#include "SceneGUI.h"
#include "SingletonPattern.h"

class SceneLevel1 : public SceneBase, public SingletonPattern<SceneLevel1>
{
	friend SingletonPattern<SceneLevel1>;
protected:
	// Variables
	float m_worldWidth;
	float m_worldHeight;
	Map* map;

	bool NearXEdge = false;
	bool NearYEdge = false;

	SceneGUI* sceneGUI;

	// Constructor
	SceneLevel1();
	// Destructor
	virtual ~SceneLevel1();

public:
	//Renderer* renderer;

	MeshList* meshlist;
	
	GunComponent* simpleGun;
	GunComponent* revolver;
	GunComponent* shotgun;
	GunComponent* minigun;

	std::vector <Vector3> activeTileList;
	std::vector <Vector3> HazardTileList;
	std::vector <Vector3> ExitTileList;
	//std::vector<Projectile*> projectileList;
	//std::vector<BaseEnemy*> enemyList;
	std::vector<Collectible*> collectibleList;

	Player* player;

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

