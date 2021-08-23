#include "SceneLevel4.h"

#include <sstream>

#include "Application.h"
#include "GL\glew.h"
#include "Map.h"
#include "Guns.h"
#include "Projectile.h"
#include "Boss.h"

SceneLevel4::SceneLevel4():map(nullptr)
{
}

SceneLevel4::~SceneLevel4()
{
}
//
//BaseEnemy* SceneTest::SpawnEnemies()
//{
//	for (BaseEnemy* enemies : BasicEnemyList)
//	{
//		if (enemies->GetActive() == false)
//		{
//			enemies->active = true;
//		}
//	}
//	return BasicEnemyList.back();
//}

void SceneLevel4::Init()
{
	type = SceneBase::SCENE_TYPE::SCENE_LEVEL_1;

	SceneBase::Init();

	player = Player::GetInstance();
	player->Init();

	boss = Boss::GetInstance();
	boss->Init();
	//baseEnemy = new ConcreteBaseEnemy();
	//baseEnemy->Init();	
	//EntityList.push_back(player);

	// Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	
	// Test guns
	simpleGun = new ConcreteGunComponent();
	revolver = new Revolver(simpleGun);
	shotgun = new Shotgun(simpleGun);
	minigun = new Minigun(simpleGun);

	player = Player::GetInstance();
	player->Init(m_worldWidth, m_worldHeight, revolver);

	// Test map
	map = Map::GetInstance();
	map->Init("Maps/Level4.csv");

	revolver->nextGun = shotgun;
	revolver->prevGun = minigun;
	revolver->fireRate = 0.5;
	revolver->fireRateTimer = 0;
	shotgun->nextGun = minigun;
	shotgun->prevGun = revolver;
	shotgun->fireRate = 1;		
	shotgun->fireRateTimer = 0;
	minigun->nextGun = revolver;
	minigun->prevGun = shotgun;
	minigun->fireRate = 0.2;
	minigun->fireRateTimer = 0;

	activeTileList.clear();
	HazardTileList.clear();

	for (int y = 0; y < 200; ++y)
	{
		for (int x = 0; x < 200; ++x)
		{
			switch (map->GetTile(x, y).type)
			{
			case Tile::TILE_TYPE::TILE_PICKUP:
			{
				Vector3 Collpos = Vector3(2 + 4 * x, m_worldHeight - 2 - 4 * y, 1);

				Collectible* collectible = new Collectible();

				collectible->active = true;
				collectible->SetPos(Collpos);
				collectible->SetScale(Vector3(2, 2, 2));
				collectibleList.push_back(collectible);
			}
			}
		}
	}


	camera->position.Set(player->GetPos().x - m_worldWidth / 2, player->GetPos().y - m_worldHeight / 2, 2);
	camera->target.Set(player->GetPos().x - m_worldWidth / 2, player->GetPos().y - m_worldHeight / 2, player->GetPos().z);

	sceneGUI = SceneGUI::GetInstance();
	sceneGUI->Init();

}

void SceneLevel4::Update(double dt)
{
	//for (int i = 0; i < entitylist.size(); i++)
	//{
	//	entitylist[i]->pos += go->vel * player.GetGameSpeed() * dt;
	//}

	static bool bZPressed = false;
	if (!bZPressed && Application::IsKeyPressed('Z'))
	{
		SceneManager::GetInstance()->SetActiveScene("TestScene");
		bZPressed = true;
	}
	else if (bZPressed && !Application::IsKeyPressed('Z'))
		bZPressed = false;

	SceneBase::Update(dt);
	
	//camera->Update(dt);

	player->Update(dt);
	// yes i hard found the values where the outside of the map starts to show
	// theres probably a better way to find these values but i cant be bothered lol -yh

	if ((player->GetPos().x > 89 && player->GetPos().x < 712))
	{
		NearXEdge = false;

		camera->position.x = player->GetPos().x - m_worldWidth / 2;
		camera->target.x = player->GetPos().x - m_worldWidth / 2;
		//camera->position.Set(player->GetPos().x - m_worldWidth / 2, player->GetPos().y - m_worldHeight / 2, 2);
		//camera->target.Set(player->GetPos().x - m_worldWidth / 2, player->GetPos().y - m_worldHeight / 2, player->GetPos().z);
	}
	else
	{
		NearXEdge = true;
	}
	if ((player->GetPos().y < 50 && player->GetPos().y > -653))
	{
		NearYEdge = false;

		camera->position.y = player->GetPos().y - m_worldHeight / 2;
		camera->target.y = player->GetPos().y - m_worldHeight / 2;
		//camera->position.Set(player->GetPos().x - m_worldWidth / 2, player->GetPos().y - m_worldHeight / 2, 2);
		//camera->target.Set(player->GetPos().x - m_worldWidth / 2, player->GetPos().y - m_worldHeight / 2, player->GetPos().z);
	}
	else
	{
		NearYEdge = true;
	}

	//baseEnemy->Update(dt);
	revolver->UpdateFireRateTimer(dt);
	shotgun->UpdateFireRateTimer(dt);
	minigun->UpdateFireRateTimer(dt);
	static bool bQButtonState = false;
	if (!bQButtonState && Application::IsKeyPressed('Q'))
	{
		player->SetActiveGun(player->GetActiveGun()->prevGun);
		bQButtonState = true;
	}
	else if (bQButtonState && !Application::IsKeyPressed('Q'))
		bQButtonState = false;

	static bool bEButtonState = false;
	if (!bEButtonState && Application::IsKeyPressed('E'))
	{
		player->SetActiveGun(player->GetActiveGun()->nextGun);
		bEButtonState = true;
	}
	else if (bEButtonState && !Application::IsKeyPressed('E'))
		bEButtonState = false;

	static bool bFireButtonState = false;
	if (!bFireButtonState && Application::IsMousePressed(0))
	{
		if (player->GetActiveGun() == revolver || player->GetActiveGun() == minigun)
		{
			Projectile* proj = player->GetActiveGun()->FireGun();
			if (proj != nullptr)
			{
				proj->Init();
				projectileList.push_back(proj);
			}
		}
		else
		{
			if (player->GetActiveGun()->fireRateTimer >= player->GetActiveGun()->fireRate)
			{
				for (int i = 1; i < 6; ++i)
				{
					Projectile* proj = player->GetActiveGun()->FireGun();
					if (proj != nullptr)
					{
						proj->Init(i);
						projectileList.push_back(proj);
					}
				}
				player->GetActiveGun()->fireRateTimer = 0;
			}
		}

		if (player->GetActiveGun() != minigun)
			bFireButtonState = true;
	}
	else if (bFireButtonState && !Application::IsMousePressed(0))
		bFireButtonState = false;

	//if (Application::IsMousePressed(1))
	//{
	//	player->SetHealth(player->GetHealth() - dt);
	//}
	
	for (Projectile* proj : projectileList)
	{
		proj->Update(dt);
	}

	for (BaseEnemy* enemy : enemyList)
	{
		enemy->Update(dt);
	}

	Player::GetInstance()->Collisioncheck(activeTileList);
	Player::GetInstance()->Hazardcheck(HazardTileList);
	Player::GetInstance()->Exitcheck(ExitTileList);

	for (Collectible* coll : collectibleList)
	{
		coll->Collisioncheck(activeTileList);
	}

	boss->Update(dt);

	sceneGUI->Update(dt);
}

void SceneLevel4::Render()
{
	activeTileList.clear();
	HazardTileList.clear();
	ExitTileList.clear();

	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera->position.x, camera->position.y, camera->position.z,
		camera->target.x, camera->target.y, camera->target.z,
		camera->up.x, camera->up.y, camera->up.z
	);


	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
	//map->RenderMap("Maps/TestMap.csv");
	for (int y = 0; y < 200; ++y)
	{
		for (int x = 0; x < 200; ++x)
		{
			switch (map->GetTile(x, y).type)
			{
			case Tile::TILE_TYPE::TILE_WALL:
			{
				Vector3 tilePos = Vector3(2 + 4 * x, m_worldHeight - 2 - 4 * y, 0);
				float width = (tilePos - Camera::GetInstance()->position).Length();
				float height = Camera::GetInstance()->position.z - tilePos.z;
				float hypotenuse = sqrtf(width * width + height * height);
				float maxWidth = (Vector3(m_worldWidth / 2 + 2, 0, 1).Dot(Vector3(m_worldHeight / 2 + 2, 0, 1)) - Camera::GetInstance()->position.Length());
				float maxHypotenuse = sqrtf(maxWidth * maxWidth + height * height);

				// Occlusion Rendering
				if (hypotenuse <= maxHypotenuse)
				{
					activeTileList.push_back(tilePos);
					modelStack.PushMatrix();
					modelStack.Translate(2 + 4 * x, m_worldHeight - 2 - 4 * y, 0);
					modelStack.Scale(2, 2, 2);
					RenderMesh(MeshList::GetInstance()->GetMesh(MeshList::GEO_CUBE), true);
					modelStack.PopMatrix();
					break;
				}
			}
			case Tile::TILE_TYPE::TILE_HAZARD:
			{
				Vector3 hazardPos = Vector3(2 + 4 * x, m_worldHeight - 2 - 4 * y, 0);
				float width = (hazardPos - Camera::GetInstance()->position).Length();
				float height = Camera::GetInstance()->position.z - hazardPos.z;
				float hypotenuse = sqrtf(width * width + height * height);
				float maxWidth = (Vector3(m_worldWidth / 2 + 2, 0, 1).Dot(Vector3(m_worldHeight / 2 + 2, 0, 1)) - Camera::GetInstance()->position.Length());
				float maxHypotenuse = sqrtf(maxWidth * maxWidth + height * height);

				// Occlusion Rendering
				if (hypotenuse <= maxHypotenuse)
				{
					HazardTileList.push_back(hazardPos);
					modelStack.PushMatrix();
					modelStack.Translate(2 + 4 * x, m_worldHeight - 2 - 4 * y, 0);
					modelStack.Scale(2, 2, 2);
					RenderMesh(MeshList::GetInstance()->GetMesh(MeshList::GEO_HAZARD), true);
					modelStack.PopMatrix();
					break;
				}
			}
			case Tile::TILE_TYPE::TILE_EXIT:
			{
				Vector3 ExitPos = Vector3(2 + 4 * x, m_worldHeight - 2 - 4 * y, 0);
				float width = (ExitPos - Camera::GetInstance()->position).Length();
				float height = Camera::GetInstance()->position.z - ExitPos.z;
				float hypotenuse = sqrtf(width * width + height * height);
				float maxWidth = (Vector3(m_worldWidth / 2 + 2, 0, 1).Dot(Vector3(m_worldHeight / 2 + 2, 0, 1)) - Camera::GetInstance()->position.Length());
				float maxHypotenuse = sqrtf(maxWidth * maxWidth + height * height);

				// Occlusion Rendering
				if (hypotenuse <= maxHypotenuse)
				{
					ExitTileList.push_back(ExitPos);
					modelStack.PushMatrix();
					modelStack.Translate(2 + 4 * x, m_worldHeight - 2 - 4 * y, 0);
					modelStack.Scale(2, 2, 2);
					RenderMesh(MeshList::GetInstance()->GetMesh(MeshList::GEO_EXIT), true);
					modelStack.PopMatrix();
					break;
				}
			}
			}
		}
	}


	Vector3 playerpos;

	if (NearXEdge == true || NearYEdge == true)
	{
		// make code the do pls thank

		playerpos = player->GetPos() - camera->position;
		//playerpos.Set((w * player->GetPos().x) / m_worldWidth, h - (h * player->GetPos().y) / m_worldHeight);
	}
	else
	{
		playerpos.Set(m_worldWidth * 0.5, m_worldHeight * 0.5);
	}

	//std::cout << player->GetPos().x << ' ' << player->GetPos().y << std::endl;

	float mousePosX = (x / w) * m_worldWidth;
	float mousePosY = m_worldHeight - (y / h) * m_worldHeight;

	Vector3 dir = Vector3(mousePosX - playerpos.x, mousePosY - playerpos.y, 0);
	player->SetRotate(dir);

	modelStack.PushMatrix();
	{
		modelStack.Translate(player->GetPos().x, player->GetPos().y, 1);
		modelStack.Rotate(Math::RadianToDegree(atan2f(dir.y, dir.x)), 0, 0, 1);
		modelStack.Scale(player->GetScale().x, player->GetScale().y, player->GetScale().z);
		RenderMesh(MeshList::GetInstance()->GetMesh(MeshList::GEO_CUBE_RED), false);
	}
	modelStack.PopMatrix();

	std::vector<Projectile*>::iterator it = projectileList.begin();
	for (; it != projectileList.end(); )
	{
		Projectile* projectile = (Projectile*)*it;
		projectile->Collisioncheck(activeTileList);

		for (BaseEnemy* enemy : enemyList)
		{
			if (projectile->active && (projectile->GetPos() - enemy->GetEnemyPos()).Length() < projectile->GetScale().x + 2)
			{
				projectile->active = false;
				enemy->SetEnemyHP(enemy->GetEnemyHP() - projectile->damage);
			}
		}
		if (projectile->active && (projectile->GetPos() - boss->GetPos()).Length() < projectile->GetScale().x + boss->GetScale().x && boss->GetFightActive())
		{
			projectile->active = false;
			boss->SetHP(boss->GetHP() - projectile->damage);
		}
		if (projectile->active && projectile->type == Projectile::PROJECTILE_TYPE::PROJECTILE_ABILITY_3 && projectile->detonateTimer >= 3)
		{
			projectile->type = Projectile::PROJECTILE_TYPE::PROJECTILE_ABILITY_3_EXPLOSION;
			projectile->SetScale(Vector3(20.f, 20.f, 1.f));
		}

		if (projectile->active && projectile->type == Projectile::PROJECTILE_TYPE::PROJECTILE_ABILITY_3_EXPLOSION && projectile->explosionTimer >= 0.1)
		{
			projectile->active = false;
		}

		if (!projectile->active)
		{
			it = projectileList.erase(it);
		}
		else
		{
			float width = (projectile->GetPos() - Camera::GetInstance()->position).Length();
			float height = Camera::GetInstance()->position.z - projectile->GetPos().z;
			float hypotenuse = sqrtf(width * width + height * height);
			float maxWidth = (Vector3(m_worldWidth / 2 + 2, 0, 1).Dot(Vector3(m_worldHeight / 2 + 2, 0, 1)) - Camera::GetInstance()->position.Length());
			float maxHypotenuse = sqrtf(maxWidth * maxWidth + height * height);

			if (hypotenuse <= maxHypotenuse)
			{
				if (projectile->type == Projectile::PROJECTILE_TYPE::PROJECTILE_ABILITY_3_EXPLOSION)
				{
					modelStack.PushMatrix();
					modelStack.Translate(projectile->GetPos().x, projectile->GetPos().y, 2);
					modelStack.Scale(projectile->GetScale().x, projectile->GetScale().y, projectile->GetScale().z);
					RenderMesh(MeshList::GetInstance()->GetMesh(MeshList::GEO_SPHERE_BLUE), false);
					modelStack.PopMatrix();
				}
				else
				{
					modelStack.PushMatrix();
					modelStack.Translate(projectile->GetPos().x, projectile->GetPos().y, 2);
					modelStack.Scale(projectile->GetScale().x, projectile->GetScale().y, projectile->GetScale().z);
					RenderMesh(MeshList::GetInstance()->GetMesh(MeshList::GEO_BALL), false);
					modelStack.PopMatrix();
				}
			}
			++it;
		}
	}
	
	std::vector<BaseEnemy*>::iterator it2 = enemyList.begin();
	for (; it2 != enemyList.end(); )
	{
		BaseEnemy* enemy = (BaseEnemy*)*it2;

		if (!enemy->active)
			it2 = enemyList.erase(it2);
		else
		{
			float width = (enemy->GetEnemyPos() - Camera::GetInstance()->position).Length();
			float height = Camera::GetInstance()->position.z - enemy->GetEnemyPos().z;
			float hypotenuse = sqrtf(width * width + height * height);
			float maxWidth = (Vector3(m_worldWidth / 2 + 2, 0, 1).Dot(Vector3(m_worldHeight / 2 + 2, 0, 1)) - Camera::GetInstance()->position.Length());
			float maxHypotenuse = sqrtf(maxWidth * maxWidth + height * height);

			if (hypotenuse <= maxHypotenuse)
			{
				modelStack.PushMatrix();
				modelStack.Translate(enemy->GetEnemyPos().x, enemy->GetEnemyPos().y, enemy->GetEnemyPos().z);
				modelStack.Scale(3, 3, 3);
				RenderMesh(MeshList::GetInstance()->GetMesh(MeshList::GEO_BASEENEMY), false);
				modelStack.PopMatrix();
			}
			it2++;
		}
	}

	std::vector<Collectible*>::iterator it3 = collectibleList.begin();

	for (; it3 != collectibleList.end(); )
	{
		Collectible* collectible = (Collectible*)*it3;

		if (!collectible->active)
		{
			it3 = collectibleList.erase(it3);
		}
		else
		{
			float width = (collectible->GetPos() - Camera::GetInstance()->position).Length();
			float height = Camera::GetInstance()->position.z - collectible->GetPos().z;
			float hypotenuse = sqrtf(width * width + height * height);
			float maxWidth = (Vector3(m_worldWidth / 2 + 2, 0, 1).Dot(Vector3(m_worldHeight / 2 + 2, 0, 1)) - Camera::GetInstance()->position.Length());
			float maxHypotenuse = sqrtf(maxWidth * maxWidth + height * height);

			if (hypotenuse <= maxHypotenuse)
			{
				modelStack.PushMatrix();
				modelStack.Translate(collectible->GetPos().x, collectible->GetPos().y, collectible->GetPos().z);
				modelStack.Scale(collectible->GetScale().x, collectible->GetScale().y, collectible->GetScale().z);
				RenderMesh(MeshList::GetInstance()->GetMesh(MeshList::GEO_CUBE_GREEN), false);
				modelStack.PopMatrix();
			}
			it3++;
		}
	}

	if (boss->active)
	{
		float width = (boss->GetPos() - Camera::GetInstance()->position).Length();
		float height = Camera::GetInstance()->position.z - boss->GetPos().z;
		float hypotenuse = sqrtf(width * width + height * height);
		float maxWidth = (Vector3(m_worldWidth / 2 + 2, 0, 1).Dot(Vector3(m_worldHeight / 2 + 2, 0, 1)) - Camera::GetInstance()->position.Length());
		float maxHypotenuse = sqrtf(maxWidth * maxWidth + height * height);

		if (hypotenuse <= maxHypotenuse)
		{
			modelStack.PushMatrix();
			modelStack.Translate(boss->GetPos().x, boss->GetPos().y, boss->GetPos().z);
			modelStack.Scale(boss->GetScale().x, boss->GetScale().y, boss->GetScale().z);
			RenderMesh(MeshList::GetInstance()->GetMesh(MeshList::GEO_BASEENEMY), false);
			modelStack.PopMatrix();
		}
	}

	sceneGUI->Render();
}

void SceneLevel4::Exit()
{
	SceneBase::Exit();

	map->Destroy();
	map = nullptr;

	player->Destroy();
	player = nullptr;

	delete simpleGun;
	delete revolver;
	delete shotgun;
	delete minigun;

	for (Projectile* proj : projectileList)
		delete proj;
	
	for (Collectible* coll : collectibleList)
		delete coll;

	collectibleList.clear();

	for (BaseEnemy* enemy : enemyList)
		delete enemy;

	enemyList.clear();

	sceneGUI->Exit();
}
