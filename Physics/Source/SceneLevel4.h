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


class SceneLevel4 : public SceneBase, public SingletonPattern<SceneLevel4>
{
	friend SingletonPattern<SceneLevel4>;
protected:
	// Variables
	Map* map;

	bool NearXEdge = false;
	bool NearYEdge = false;

	bool musicstart;

	SceneGUI* sceneGUI;

	// Constructor
	SceneLevel4();
	// Destructor
	virtual ~SceneLevel4();

public:
	//Renderer* renderer;

	MeshList* meshlist;
	
	GunComponent* simpleGun;
	GunComponent* revolver;
	GunComponent* shotgun;
	GunComponent* minigun;

	SoundController* soundController;

	std::vector <Vector3> activeTileList;
	std::vector <Vector3> HazardTileList;
	std::vector <Vector3> ExitTileList;
	//std::vector<Projectile*> projectileList;
	//std::vector<BaseEnemy*> enemyList;
	std::vector<Collectible*> collectibleList;

	Player* player;
	Boss* boss;

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

