#pragma once

#include "SceneBase.h"
#include "SceneGUI.h"
#include "SoundController.h"

#include "../includes/irrKlang.h"


class MenuScene : public SceneBase
{
private:
	// Variables
	float m_worldWidth;
	float m_worldHeight;

	ImGuiWindowFlags windowFlags;
	bool goback = false;


public:
	// Constructor
	MenuScene();
	// Destructor
	~MenuScene();

	virtual void Init();
	virtual void Update(const double dt);
	virtual void Render(void);
	virtual void Destroy(void);

	SoundController* soundController;


protected:
	SceneBase::ButtonData playButtonData;
	SceneBase::ButtonData howtoButtonData;
	SceneBase::ButtonData quitButtonData;

};

