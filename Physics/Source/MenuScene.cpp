#include "MenuScene.h"

#include <sstream>

#include "GL/glew.h"
#include "Application.h"
#include "MeshList.h"

MenuScene::MenuScene()
{
}

MenuScene::~MenuScene()
{
}

void MenuScene::Init()
{
	SceneBase::Init();

	// Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
}

void MenuScene::Update(const double dt)
{
	SceneBase::Update(dt);

	static bool bEnterPressed = false;
	if (!bEnterPressed && Application::IsKeyPressed(VK_RETURN))
	{
		SceneManager::GetInstance()->SetActiveScene("TestScene");
		bEnterPressed = true;
	}
	else if (bEnterPressed && !Application::IsKeyPressed(VK_RETURN))
		bEnterPressed = false;
}

void MenuScene::Render(void)
{
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

	std::ostringstream ss;
	ss << "Press 'Enter' to start";
	RenderTextOnScreen(MeshList::GetInstance()->GetMesh(MeshList::GEO_TEXT), ss.str(), Color(0, 1, 0), 3, 8, 30);
}

void MenuScene::Destroy(void)
{
	SceneBase::Exit();
}
