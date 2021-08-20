#pragma once

#include "SceneBase.h"
#include "Map.h"
#include "Guns.h"
#include "BaseEnemy.h"
#include "Player.h"
#include "MeshList.h"
#include "Renderer.h"
#include "Projectile.h"
#include "Collectible.h"
#include "SceneGUI.h"
#include "SingletonPattern.h"

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

	BaseEnemy* baseEnemy;
	BaseEnemy* cavemanMelee;
	BaseEnemy* cavemeanRanged;
	BaseEnemy* Knight;
	BaseEnemy* Archer;
	BaseEnemy* Cyborg;
	BaseEnemy* Sniper;

	std::vector<Entity*> EntityList;
	std::vector<CavemenMelee*> CaveManMeleeList;
	std::vector <Vector3> activeTileList;
	std::vector <Vector3> HazardTileList;
	std::vector<Projectile*> projectileList;
	
	std::vector<Collectible*> collectibleList;

	Player* player;

	BaseEnemy* SpawnEnemies();
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

