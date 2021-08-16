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

void SceneTest::Init()
{
	SceneBase::Init();

	player = Player::GetInstance();
	player->Init();
	baseEnemy = new ConcreteBaseEnemy();
	baseEnemy->Init();
	//EntityList.push_back(player);

	// Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	
	player = Player::GetInstance();
	player->Init(m_worldWidth, m_worldHeight);

	// Test map
	map = Map::GetInstance();

	// Test guns
	simpleGun = new ConcreteGunComponent();
	revolver = new Revolver(simpleGun);
	shotgun = new Shotgun(simpleGun);
	minigun = new Minigun(simpleGun);
	activeGun = revolver;

	revolver->nextGun = shotgun;
	revolver->prevGun = minigun;
	shotgun->nextGun = minigun;
	shotgun->prevGun = revolver;
	minigun->nextGun = revolver;
	minigun->prevGun = shotgun;
}

void SceneTest::Update(double dt)
{
	//for (int i = 0; i < entitylist.size(); i++)
	//{
	//	entitylist[i]->pos += go->vel * player.GetGameSpeed() * dt;
	//}

	SceneBase::Update(dt);
	player->Update(dt);
	baseEnemy->Update(dt);
	static bool bQButtonState = false;
	if (!bQButtonState && Application::IsKeyPressed('Q'))
	{
		activeGun = activeGun->prevGun;
		bQButtonState = true;
	}
	else if (bQButtonState && !Application::IsKeyPressed('Q'))
		bQButtonState = false;

	static bool bEButtonState = false;
	if (!bEButtonState && Application::IsKeyPressed('E'))
	{
		activeGun = activeGun->nextGun;
		bEButtonState = true;
	}
	else if (bEButtonState && !Application::IsKeyPressed('E'))
		bEButtonState = false;

	static bool bFireButtonState = false;
	if (!bFireButtonState && Application::IsMousePressed(0))
	{
		activeGun->FireGun();
		bFireButtonState = true;
	}
	else if (bFireButtonState && !Application::IsMousePressed(0))
		bFireButtonState = false;
}

void SceneTest::Render()
{
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
	map->ReadFile("Maps/TestMap.csv");
	for (int y = 0; y < 200; ++y)
	{
		for (int x = 0; x < 200; ++x)
		{
			switch (map->GetTile(x, y))
			{
			case Map::TILES::TILE_WALL:
				Vector3 tilePos = Vector3(2 + 4 * x, m_worldHeight - 2 - 4 * y, 1);
				float width = (tilePos - Camera::GetInstance()->position).Length();
				float height = Camera::GetInstance()->position.z - tilePos.z;
				float hypotenuse = sqrtf(width * width + height * height);
				float maxWidth = (Vector3(m_worldWidth + 2, m_worldHeight + 2, 1) - Camera::GetInstance()->position).Length();
				float maxHypotenuse = sqrtf(maxWidth * maxWidth + height * height);

				if (hypotenuse <= maxHypotenuse)
				{
					modelStack.PushMatrix();
					modelStack.Translate(2 + 4 * x, m_worldHeight - 2 - 4 * y, 1);
					modelStack.Scale(2, 2, 2);
					RenderMesh(MeshList::GetInstance()->GetMesh(MeshList::GEO_CUBE), true);
					modelStack.PopMatrix();
					break;
				}
			}
		}
	}

	modelStack.PushMatrix();
	{
		modelStack.Translate(player->GetPos().x, player->GetPos().y, 0);
		modelStack.Rotate(Math::RadianToDegree(atan2f(m_worldHeight - (y / h) * m_worldHeight - player->GetPos().y, (x / w) * m_worldWidth - player->GetPos().x)), 0, 0, 1);
		modelStack.Scale(player->GetScale().x, player->GetScale().y * 0.5, player->GetScale().z);
		RenderMesh(MeshList::GetInstance()->GetMesh(MeshList::GEO_CUBE_RED), false);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(10,90,0);
	RenderMesh(MeshList::GetInstance()->GetMesh(MeshList::GEO_BASEENEMY), true);
	modelStack.PopMatrix();

	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(MeshList::GetInstance()->GetMesh(MeshList::GEO_TEXT), ss.str(), Color(0, 1, 0), 3, 0, 3);
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
}
