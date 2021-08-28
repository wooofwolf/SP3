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
#include "Boss.h"
#include "SoundController.h"


class SceneLevel2 : public SceneBase, public SingletonPattern<SceneLevel2>
{
	friend SingletonPattern<SceneLevel2>;
protected:
	// Variables
	float m_worldWidth;
	float m_worldHeight;
	Map* map;

	bool NearXEdge = false;
	bool NearYEdge = false;

	SceneGUI* sceneGUI;

	// Constructor
	SceneLevel2();
	// Destructor
	virtual ~SceneLevel2();

public:
	//Renderer* renderer;

	MeshList* meshlist;
	
	GunComponent* component;
	GunComponent* simpleGun;
	GunComponent* revolver;
	GunComponent* shotgun;
	GunComponent* minigun;

	SoundController* soundController;

	std::vector <Vector3> activeTileList;
	std::vector <Vector3> HazardTileList;
	std::vector <Vector3> ExitTileList;

	/*std::vector<Projectile*> projectileList;
	std::vector<BaseEnemy*> enemyList;*/
	std::vector<Collectible*> collectibleList;

	Player* player;
	Boss* boss;

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

