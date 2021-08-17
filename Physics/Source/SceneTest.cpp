#include "SceneTest.h"

#include <sstream>

#include "Application.h"
#include "GL\glew.h"
#include "Map.h"
#include "Guns.h"

SceneTest::SceneTest():map(nullptr)
{
}

SceneTest::~SceneTest()
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

void SceneTest::Init()
{
	SceneBase::Init();


	baseEnemy = new ConcreteBaseEnemy();
	baseEnemy->Init();
	player = Player::GetInstance();
	player->Init();


	
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
	map->Init("Maps/TestMap.csv");

	revolver->nextGun = shotgun;
	revolver->prevGun = minigun;
	revolver->fireRate = 1;
	revolver->fireRateTimer = 0;
	shotgun->nextGun = minigun;
	shotgun->prevGun = revolver;
	shotgun->fireRate = 2;
	shotgun->fireRateTimer = 0;
	minigun->nextGun = revolver;
	minigun->prevGun = shotgun;
	minigun->fireRate = 0.1;
	minigun->fireRateTimer = 0;

	activeTileList.clear();

	for (int i = 0; i < 2; ++i)
	{
		CavemenMelee* CavemanMelee = new CavemenMelee(baseEnemy);
		CavemanMelee->Init();
		CaveManMeleeList.push_back(CavemanMelee);
	}
}

void SceneTest::Update(double dt)
{
	//for (int i = 0; i < entitylist.size(); i++)
	//{
	//	entitylist[i]->pos += go->vel * player.GetGameSpeed() * dt;
	//}

	SceneBase::Update(dt);
	
	//camera->Update(dt);

	player->Update(dt);
	for (CavemenMelee* CaveMelee : CaveManMeleeList)
	{
		CaveMelee->Movement(dt);
	}
	// yes i hard found the values where the outside of the map starts to show
	// theres probably a better way to find these values but i cant be bothered lol -yh
	if ((player->GetPos().x > 90 && player->GetPos().x < 714))
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
	
	for (Projectile* proj : projectileList)
	{
		proj->Update(dt);
	}
}

void SceneTest::Render()
{
	activeTileList.clear();

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
			switch (map->GetTile(x, y))
			{
			case Map::TILES::TILE_WALL:
			{
				Vector3 tilePos = Vector3(2 + 4 * x, m_worldHeight - 2 - 4 * y, 1);
				float width = (tilePos - Camera::GetInstance()->position).Length();
				float height = Camera::GetInstance()->position.z - tilePos.z;
				float hypotenuse = sqrtf(width * width + height * height);
				float maxWidth = (Vector3(m_worldWidth / 2 + 2, 0, 1).Dot(Vector3(m_worldHeight / 2 + 2, 0, 1)) - Camera::GetInstance()->position.Length());
				float maxHypotenuse = sqrtf(maxWidth * maxWidth + height * height);

				if (hypotenuse <= maxHypotenuse)
				{
					activeTileList.push_back(tilePos);
					modelStack.PushMatrix();
					modelStack.Translate(2 + 4 * x, m_worldHeight - 2 - 4 * y, 1);
					modelStack.Scale(2, 2, 2);
					RenderMesh(MeshList::GetInstance()->GetMesh(MeshList::GEO_CUBE), true);
					modelStack.PopMatrix();
					break;

				}
			}
			//case Map::TILES::TILE_PLAYER:
			//{
			//	Player::GetInstance()->SetPos(Vector3(10, 90, 1));
			//	break;
			//}
			}
		}
	}
	Player::GetInstance()->Collisioncheck(activeTileList);

	//modelStack.PushMatrix();
	//player->Render();
	//modelStack.PopMatrix();

	//done for now

	Vector3 playerpos;

	if (NearXEdge == true || NearYEdge == true)
	{
		// make code the do pls thank

		playerpos = player->GetPos();
	}
	else
	{
		playerpos.Set(m_worldWidth * 0.5, m_worldHeight * 0.5);
	}

	Vector3 dir = Vector3((x / w) * m_worldWidth - playerpos.x, m_worldHeight - (y / h) * m_worldHeight - playerpos.y, 0);
	player->SetRotate(dir);

	modelStack.PushMatrix();
	{
		modelStack.Translate(player->GetPos().x, player->GetPos().y, 0);
		modelStack.Rotate(Math::RadianToDegree(atan2f(dir.y, dir.x)), 0, 0, 1);
		modelStack.Scale(player->GetScale().x, player->GetScale().y, player->GetScale().z);
		RenderMesh(MeshList::GetInstance()->GetMesh(MeshList::GEO_CUBE_RED), false);
	}
	modelStack.PopMatrix();

	for (std::vector<CavemenMelee*>::iterator it = CaveManMeleeList.begin(); it != CaveManMeleeList.end(); ++it)
	{
		CavemenMelee* Cavemen = (CavemenMelee*)*it;
		if (Cavemen->active != true)
		{
			Cavemen->active = true;
		}
		if (Cavemen->active == true)
		{
			modelStack.PushMatrix();
			modelStack.Translate(Cavemen->GetEnemyPos().x, Cavemen->GetEnemyPos().y, 1);
			modelStack.Scale(10, 10, 10);
			RenderMesh(MeshList::GetInstance()->GetMesh(MeshList::GEO_BASEENEMY), false);
			modelStack.PopMatrix();
		}
	}

	for (CavemenMelee* CaveMelee : CaveManMeleeList)
	{
		CaveMelee->Collisioncheck(activeTileList);
	}
	
	//std::cout << (x / w) * m_worldWidth << ' ' << m_worldHeight - (y / h) * m_worldHeight << std::endl;

	//baseEnemy->active = true;


	/*modelStack.PushMatrix();
	modelStack.Translate(baseEnemy->GetEnemyPos().x, baseEnemy->GetEnemyPos().y, 1);
	modelStack.Scale(10, 10, 10);
	RenderMesh(MeshList::GetInstance()->GetMesh(MeshList::GEO_BASEENEMY), false);
	modelStack.PopMatrix();*/

	
	//modelStack.PushMatrix();
	//modelStack.Translate(2 + 4 * baseEnemy->GetEnemyPos().x, m_worldHeight - 2 - 4 * baseEnemy->GetEnemyPos().y, 1);
	//RenderMesh(MeshList::GetInstance()->GetMesh(MeshList::GEO_BASEENEMY), false);
	//modelStack.PopMatrix();

	std::vector<Projectile*>::iterator it = projectileList.begin();
	for ( ; it != projectileList.end(); )
	{
		Projectile* projectile = (Projectile*)*it;
		projectile->Collisioncheck(activeTileList);
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
				modelStack.PushMatrix();
					modelStack.Translate(projectile->GetPos().x, projectile->GetPos().y, projectile->GetPos().z);
					modelStack.Scale(projectile->GetScale().x, projectile->GetScale().y, projectile->GetScale().z);
					RenderMesh(MeshList::GetInstance()->GetMesh(MeshList::GEO_BALL), false);
				modelStack.PopMatrix();
			}
			++it;
		}
	}

	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(MeshList::GetInstance()->GetMesh(MeshList::GEO_TEXT), ss.str(), Color(0, 1, 0), 3, 0, 3);

	//RenderTextOnScreen(MeshList::GetInstance()->GetMesh(MeshList::GEO_TEXT), "Test", Color(0, 1, 0), 3, 0, 6);
}

void SceneTest::Exit()
{
	SceneBase::Exit();

	delete map;

	delete player;

	delete simpleGun;
	delete revolver;
	delete shotgun;
	delete minigun;

	for (Entity* entity : EntityList)
		delete entity;
	for (BaseEnemy* enemy : CaveManMeleeList)
		delete enemy;
	for (Projectile* proj : projectileList)
		delete proj;
}
