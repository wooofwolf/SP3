#pragma once

#include "SceneBase.h"
#include "SceneGUI.h"
#include "../includes/irrKlang.h"


class WinScene : public SceneBase
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
	WinScene();
	// Destructor
	~WinScene();

	virtual void Init();
	virtual void Update(const double dt);
	virtual void Render(void);
	virtual void Destroy(void);
protected:
	SceneBase::ButtonData restartButtonData;
};

