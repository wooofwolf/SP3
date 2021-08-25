#include "HowtoScene.h"

#include <sstream>

#include "GL/glew.h"
#include "Application.h"
#include "MeshList.h"

#include "../includes/irrKlang.h"

HowtoScene::HowtoScene()
{
}

HowtoScene::~HowtoScene()
{
}

void HowtoScene::Init()
{
	SceneBase::Init();

	// Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth()
		/ Application::GetWindowHeight();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();
	// Bind window
	ImGui_ImplGlfw_InitForOpenGL(Application::GetInstance().GetWindow(), true);
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);

	windowFlags = 0;
	windowFlags |= ImGuiWindowFlags_NoBackground;
	windowFlags |= ImGuiWindowFlags_NoTitleBar;
	windowFlags |= ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoResize;
	windowFlags |= ImGuiWindowFlags_NoCollapse;
}

void HowtoScene::Update(const double dt)
{
	SceneBase::Update(dt);

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Create an invisible window which covers the entire OpenGL window
	ImGui::Begin("Play window", NULL, windowFlags);
	{
		ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::SetWindowSize(ImVec2((float)Application::GetInstance().GetWindowWidth(), (float)Application::GetInstance().GetWindowHeight()));
		ImGui::SetWindowFontScale(3.f);

		// Display the FPS
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "FPS: %f", fps);
		// Render which gun the player is holding
		ImGuiWindowFlags menubutton = ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoScrollbar;

		ImGui::Begin("Play", NULL, menubutton);
		{
			ImGui::SetWindowPos(ImVec2(0, 0));
			ImGui::SetWindowSize(ImVec2(50.0f, 50.0f));
			ImGui::SetWindowFontScale(3.f);
			if (ImGui::ImageButton("..//Image//RestartButton.png", ImVec2(100, 100), ImVec2(0, 0), ImVec2(0, 0), 1, ImVec4(1, 1, 1, 1), ImVec4(1, 0, 1, 1)))
			{
				SceneManager::GetInstance()->SetActiveScene("MenuScene");
				ImGuiIO& io = ImGui::GetIO(); (void)io;
				io.WantCaptureMouse = false;
				Player::GetInstance()->lose = false;
				goback = true;
			}
		}
		ImGui::End();
	}
	ImGui::End();


	static bool bEnterPressed = false;
	if (!bEnterPressed && Application::IsKeyPressed(VK_RETURN))
	{
		SceneManager::GetInstance()->SetActiveScene("Level1");
		//SceneManager::GetInstance()->SetActiveScene("Level1");
		bEnterPressed = true;
	}
	else if (bEnterPressed && !Application::IsKeyPressed(VK_RETURN))
		bEnterPressed = false;
}

void HowtoScene::Render(void)
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
	ss << "among us";
	RenderTextOnScreen(MeshList::GetInstance()->
		GetMesh(MeshList::GEO_TEXT), ss.str(), Color(0, 1, 0), 3, 8, 30);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	if (goback)
	{
		Destroy();
		goback = false;
	}
}

void HowtoScene::Destroy(void)
{
	SceneBase::Exit();
	SceneGUI::GetInstance()->Exit();

}
