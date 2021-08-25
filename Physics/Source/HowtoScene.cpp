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

	CImageLoader* il = CImageLoader::GetInstance();
	returnButtonData.fileName = "Image\\return.png";
	returnButtonData.textureID = il->LoadTextureGetID(returnButtonData.fileName.c_str(), false);

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

		// Render which gun the player is holding
		ImGuiWindowFlags howto = ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoScrollbar;

		ImGui::Begin("exit", NULL, howto);
		{
			ImGui::SetWindowPos(ImVec2(0, 0));
			ImGui::SetWindowSize(ImVec2(50.0f, 50.0f));
			ImGui::SetWindowFontScale(3.f);
			if (ImGui::ImageButton((ImTextureID)returnButtonData.textureID, ImVec2(100, 100), ImVec2(0, 0), ImVec2(1, 1), 1))
			{
				SceneManager::GetInstance()->SetActiveScene("MenuScene");
				ImGuiIO& io = ImGui::GetIO(); (void)io;
				io.WantCaptureMouse = false;
				Player::GetInstance()->lose = false;
				goback = true;
			}
		}
		ImGui::End();

		ImGui::Begin("howtotext", NULL, howto);
		{
			ImGui::SetWindowPos(ImVec2(0, 100));
			ImGui::SetWindowSize(ImVec2(300.0f, 100.0f));
			ImGui::SetWindowFontScale(10.f);
			ImGui::TextColored(ImVec4(1, 1, 1, 1), "Controls:");
			ImGui::SetWindowFontScale(3.f);
			ImGui::TextColored(ImVec4(1, 1, 1, 1), "WASD  -  Moving Up, Down, Left and Right.");
			ImGui::TextColored(ImVec4(1, 1, 1, 1), "E/Q  -  Cycling weapons forwards or backwards respectively.");
			ImGui::TextColored(ImVec4(1, 1, 1, 1), "SPACEBAR  -  Dash towards movement direction.");
			ImGui::TextColored(ImVec4(1, 1, 1, 1), "1  -  Slow time down to slow enemies at the cost of a constant drain.");
			ImGui::TextColored(ImVec4(1, 1, 1, 1), "2  -  Use 50 energy to boost your movement and firing speed for a burst of damage.");
			ImGui::TextColored(ImVec4(1, 1, 1, 1), "3  -  Use 90 energy to throw a bomb that detonates and freezes enemies.");
			ImGui::SetWindowFontScale(10.f);
			ImGui::TextColored(ImVec4(1, 1, 1, 1), "Tips & Tricks:");
			ImGui::SetWindowFontScale(3.f);
			ImGui::TextColored(ImVec4(1, 1, 1, 1), "Maintaining your energy throughout a room can help clear them faster.");
			ImGui::TextColored(ImVec4(1, 1, 1, 1), "Your energy regenerates at a slow rate so try not to spam it too much.");
			ImGui::TextColored(ImVec4(1, 1, 1, 1), "If you activate your first ability, you will not have any energy regen.");
			ImGui::TextColored(ImVec4(1, 1, 1, 1), "Switching weapons can provide a good damage boost if you are low on energy.");

		}
		ImGui::End();
	}
	ImGui::End();
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
