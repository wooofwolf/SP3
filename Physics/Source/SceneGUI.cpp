#include "SceneGUI.h"

#include "Application.h"
#include "Player.h"

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
		ImGui::SetWindowFontScale(3.f);

		// Display the FPS
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "FPS: %f", fps);

		// Render the Health
		ImGuiWindowFlags healthWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoScrollbar;
		ImGui::Begin("Health", NULL, healthWindowFlags);
			ImGui::SetWindowPos(ImVec2(10.0f, 750.0f));
			ImGui::SetWindowSize(ImVec2(600.0f, 60.0f));
			ImGui::SetWindowFontScale(3.f);
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "Health:");
			ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(32.f / 255.f, 246.f / 255.f, 0, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));
			ImGui::ProgressBar((float)(Player::GetInstance()->GetHealth()) / 100.f, ImVec2(600.0f, 60.0f));
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "Energy:");
			ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(104.f / 255.f, 194.f / 255.f, 245.f / 255.f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));
			ImGui::ProgressBar((float)(Player::GetInstance()->GetEnergy()) / 100.f, ImVec2(600.0f, 60.0f));
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
		ImGui::End();
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
