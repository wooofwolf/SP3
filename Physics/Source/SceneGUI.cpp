#include "SceneGUI.h"

#include "Application.h"
#include "Player.h"
#include "Guns.h"
#include "Boss.h"

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

	player = Player::GetInstance();
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

		// Render the health and energy bars
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
			ImGui::ProgressBar((float)(player->GetHealth()) / 100.f, ImVec2(600.0f, 60.0f));
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "Energy:");
			ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(104.f / 255.f, 194.f / 255.f, 245.f / 255.f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));
			ImGui::ProgressBar((float)(player->GetEnergy()) / 100.f, ImVec2(600.0f, 60.0f));
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
		ImGui::End();

		// Render which gun the player is holding
		ImGuiWindowFlags gunWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoScrollbar;
		ImGui::Begin("Gun", NULL, gunWindowFlags);
			ImGui::SetWindowPos(ImVec2(1500.0f, 900.0f));
			ImGui::SetWindowSize(ImVec2(50.0f, 100.0f));
			ImGui::SetWindowFontScale(3.f);
			if (dynamic_cast<Revolver*>(Player::GetInstance()->GetActiveGun()) != nullptr)
				ImGui::TextColored(ImVec4(0, 1, 0, 1), "Holding: Revolver");
			else if (dynamic_cast<Shotgun*>(Player::GetInstance()->GetActiveGun()) != nullptr)
				ImGui::TextColored(ImVec4(0, 1, 0, 1), "Holding: Shotgun");
			else
				ImGui::TextColored(ImVec4(0, 1, 0, 1), "Holding: Minigun");
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "Reload:");
			ImGui::SameLine();
			if (player->power2active)
				ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(1.f, 0, 0, 1.0f));
			else
				ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(32.f / 255.f, 246.f / 255.f, 0, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));
			ImGui::ProgressBar((float)(player->GetActiveGun()->fireRate - player->GetActiveGun()->fireRateTimer), ImVec2(200.0f, 60.0f), "");
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
		ImGui::End();

		// Render the boss bar
		if (Boss::GetInstance()->GetFightActive())
		{
			ImGuiWindowFlags bossBarWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoBackground |
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoScrollbar;
			ImGui::Begin("Boss", NULL, bossBarWindowFlags);
				ImGui::SetWindowPos(ImVec2(450.0f, 10.0f));
				ImGui::SetWindowSize(ImVec2(1000.0f, 100.0f));
				ImGui::SetWindowFontScale(3.f);
				ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(229.f / 255.f, 101.f / 255.f, 46.f / 255.f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
				ImGui::ProgressBar(Boss::GetInstance()->GetHP() / 1000.f, ImVec2(1000.f, 80.f), "");
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
			ImGui::End();
		}
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
