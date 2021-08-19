#include "SceneGUI.h"

#include "Application.h"

void SceneGUI::Init()
{
	SceneBase::Init();

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

void SceneGUI::Update(double dt)
{
	SceneBase::Update(dt);

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Create an invisible window which covers the entire OpenGL window
	ImGui::Begin("Invisible window", NULL, windowFlags);
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetWindowSize(ImVec2((float)Application::GetInstance().GetWindowWidth(), (float)Application::GetInstance().GetWindowHeight()));
	ImGui::SetWindowFontScale(1.5f);

	// Display the FPS
	ImGui::TextColored(ImVec4(0, 1, 0, 1), "FPS: %f", fps);

	ImGui::End();
}

void SceneGUI::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void SceneGUI::Exit()
{
	SceneBase::Exit();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

SceneGUI::SceneGUI()
{
}

SceneGUI::~SceneGUI()
{
}
