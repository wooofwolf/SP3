#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "SceneBase.h"
#include "SingletonPattern.h"
#include "Player.h"

class SceneGUI : public SceneBase, public SingletonPattern<SceneGUI>
{
	friend SingletonPattern<SceneGUI>;
public:
	void Init();
	void Update(double dt);
	void Render();
	void Exit();

protected:
	// Constructor
	SceneGUI();
	// Destructor
	virtual ~SceneGUI();

	// Flags for IMGUI
	ImGuiWindowFlags windowFlags;

	Player* player;
};

