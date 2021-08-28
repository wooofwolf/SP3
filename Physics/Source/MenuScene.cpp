#include "MenuScene.h"

#include <sstream>

#include "GL/glew.h"
#include "Application.h"
#include "MeshList.h"

#include "../includes/irrKlang.h"

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
	playButtonData.fileName = "Image\\playbutton.png";
	playButtonData.textureID = il->LoadTextureGetID(playButtonData.fileName.c_str(), false);
	
	CImageLoader* il2 = CImageLoader::GetInstance();
	howtoButtonData.fileName = "Image\\howtobutton.png";
	howtoButtonData.textureID = il->LoadTextureGetID(howtoButtonData.fileName.c_str(), false);

	CImageLoader* il3 = CImageLoader::GetInstance();
	quitButtonData.fileName = "Image\\quitbutton.png";
	quitButtonData.textureID = il->LoadTextureGetID(quitButtonData.fileName.c_str(), false);

	soundController = SoundController::GetInstance();
	soundController->PlaySoundFile("Sounds/Music_mainmenu.wav", true);
}

void MenuScene::Update(const double dt)
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
		ImGuiWindowFlags menubutton = ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoScrollbar;



		ImGui::SetWindowPos(ImVec2(m_worldWidth / 2, 300));

		if (ImGui::ImageButton((ImTextureID)playButtonData.textureID, ImVec2(600, 150), ImVec2(0, 0), ImVec2(1, 1), 1))
		{
			SceneManager::GetInstance()->SetActiveScene("Level1");
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.WantCaptureMouse = false;
			Player::GetInstance()->lose = false;
			goback = true;
		}

		ImGui::Dummy(ImVec2(0.0f, 20.f));

		if (ImGui::ImageButton((ImTextureID)howtoButtonData.textureID, ImVec2(600, 150), ImVec2(0, 0), ImVec2(1, 1), 1))
		{
			SceneManager::GetInstance()->SetActiveScene("HowtoScene");
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.WantCaptureMouse = false;
			Player::GetInstance()->lose = false;
			goback = true;
		}

		ImGui::Dummy(ImVec2(0.0f, 20.f));

		if (ImGui::ImageButton((ImTextureID)quitButtonData.textureID, ImVec2(600, 150), ImVec2(0, 0), ImVec2(1, 1), 1)
			|| Application::IsKeyPressed(VK_ESCAPE))
		{
			Application::GetInstance().quitgame = true;
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.WantCaptureMouse = false;
			Player::GetInstance()->lose = false;
			goback = true;
		}
	}
	ImGui::End();
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

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	if (goback)
	{
		Destroy();
		goback = false;
	}
}

void MenuScene::Destroy(void)
{
	soundController->StopAllSounds();
	SceneBase::Exit();
	SceneGUI::GetInstance()->Exit();
}
