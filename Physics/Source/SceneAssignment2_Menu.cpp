#include "SceneAssignment2_Menu.h"

#include "GL\glew.h"
#include "Application.h"
#include "SceneBase.h"
#include "Application.h"
#include "SceneAssignment2.h"

SceneAssignment2_Menu::SceneAssignment2_Menu()
{
}

SceneAssignment2_Menu::~SceneAssignment2_Menu()
{
}

void SceneAssignment2_Menu::Init()
{
	SceneBase::Init();

	// Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
}

void SceneAssignment2_Menu::Update(double dt)
{
	SceneBase::Update(dt);

	//Mouse Section
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;

		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = x / 10; //convert (0,800) to (0,80)
		float posY = y / 10; //convert (600,0) to (0,60)

		if (posX >= 55 && posX <= 135 && posY >= 64 && posY <= 88)
		{
			Application::sceneNo = 2;
			SceneAssignment2::gameStarted = true;
		}
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
	}
}

void SceneAssignment2_Menu::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, 1);
		modelStack.Scale(1.f, .5f, .5f);
		RenderMesh(meshList[GEO_BACKGROUND], true);
	modelStack.PopMatrix();
}

void SceneAssignment2_Menu::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while (m_goList.size() > 0)
	{
		GameObject* go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
}
