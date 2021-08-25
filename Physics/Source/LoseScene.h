#pragma once

#include "SceneBase.h"
#include "SceneGUI.h"
#include "../includes/irrKlang.h"


class LoseScene : public SceneBase
{
private:
	// Variables
	float m_worldWidth;
	float m_worldHeight;
	// Flags for IMGUI
	ImGuiWindowFlags windowFlags;

	bool goback = false;
public:
	// Constructor
	LoseScene();
	// Destructor
	~LoseScene();

	virtual void Init();
	virtual void Update(const double dt);
	virtual void Render(void);
	virtual void Destroy(void);
};

