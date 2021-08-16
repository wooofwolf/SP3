#include "SceneTest.h"

#include <sstream>

#include "Application.h"
#include "GL\glew.h"
#include "Map.h"

SceneTest::SceneTest():map(nullptr)
{
}

SceneTest::~SceneTest()
{
}

void SceneTest::Init()
{
	SceneBase::Init();

	//entitylist

	//Player player = new Player();
	//player.init();
	//entitylist.pushback(player);

	// Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	// Test map
	map = new Map();
	//map->ReadFile("Maps/TestMap.csv");
}

void SceneTest::Update(double dt)
{
	//for (int i = 0; i < entitylist.size(); i++)
	//{
	//	entitylist[i]->pos += go->vel * player.GetGameSpeed() * dt;
	//}

	SceneBase::Update(dt);
}

void SceneTest::Render()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//// Projection matrix : Orthographic Projection
	//Mtx44 projection;
	//projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	//projectionStack.LoadMatrix(projection);

	//// Camera matrix
	//viewStack.LoadIdentity();
	//viewStack.LookAt(
	//	camera.position.x, camera.position.y, camera.position.z,
	//	camera.target.x, camera.target.y, camera.target.z,
	//	camera.up.x, camera.up.y, camera.up.z
	//);

	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
	map->RenderMap("Maps/TestMap.csv");

	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);
}

void SceneTest::Exit()
{
	SceneBase::Exit();

	delete map;
}
