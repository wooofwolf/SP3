#include "LoseScene.h"

#include <sstream>

#include "GL/glew.h"
#include "Application.h"
#include "MeshList.h"
#include "../includes/irrKlang.h"

LoseScene::LoseScene()
{
}

LoseScene::~LoseScene()
{
}

void LoseScene::Init()
{
	SceneBase::Init();

	// Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	GLFWwindow* pWindow;
	// Setup Platform/Renderer bindings

	ImGui::StyleColorsDark();

	windowFlags = 0;
	windowFlags |= ImGuiWindowFlags_NoBackground;
	windowFlags |= ImGuiWindowFlags_NoTitleBar;
	windowFlags |= ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoResize;
	windowFlags |= ImGuiWindowFlags_NoCollapse;

	CImageLoader* il = CImageLoader::GetInstance();
	restartButtonData.fileName = "Image\\retrybutton.png";
	restartButtonData.textureID = il->LoadTextureGetID(restartButtonData.fileName.c_str(), false);

}

void LoseScene::Update(const double dt)
{
	SceneBase::Update(dt);

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Create an invisible window which covers the entire OpenGL window
	ImGui::Begin("Invisible window", NULL, windowFlags);
		ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::SetWindowSize(ImVec2((float)Application::GetInstance().GetWindowWidth(),		(float)Application::GetInstance().GetWindowHeight()));
		ImGui::SetWindowFontScale(3.f);

	// Display the FPS
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "FPS: %f", fps);
			// Render which gun the player is holding
		ImGuiWindowFlags Playbutton = ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoScrollbar;
		
		ImGui::Begin("lose", NULL, Playbutton);
		ImGui::SetWindowPos(ImVec2(750, 600));
		ImGui::SetWindowSize(ImVec2(50.0f, 100.0f));
		ImGui::SetWindowFontScale(3.f);
		ImGuiStyle& style = ImGui::GetStyle();
		style.FrameRounding = 200.0f;
		if (ImGui::ImageButton((ImTextureID)restartButtonData.textureID, ImVec2(400, 200), ImVec2(0, 0), ImVec2(1, 1), 1))
		{
			SceneManager::GetInstance()->SetActiveScene(SceneManager::GetInstance()->GetPrevScene());
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.WantCaptureMouse = false;
			Player::GetInstance()->lose = false;
			goback = true;
		}
		/*ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));
		ImGui::PopStyleColor();*/
		ImGui::End();
		
	ImGui::End();
}

void LoseScene::Render(void)
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

	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth / 2, (m_worldHeight / 2) + 10, 0);
	modelStack.Rotate(-180, 0, 0, 1);
	modelStack.Scale(50, 6, 2);
	RenderMesh(MeshList::GetInstance()->GetMesh(MeshList::GEO_LOSE), true);
	modelStack.PopMatrix();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	if (goback)
	{
		Destroy();
		goback = false;
	}
}

void LoseScene::Destroy(void)
{
	SceneBase::Exit();
	// Cleanup
	SceneGUI::GetInstance()->Exit();
}
