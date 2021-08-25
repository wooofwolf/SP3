#pragma once

#include "SceneBase.h"
#include "SceneGUI.h"

#include "../includes/irrKlang.h"


class HowtoScene : public SceneBase
{
private:
	// Variables
	float m_worldWidth;
	float m_worldHeight;

	ImGuiWindowFlags windowFlags;
	bool goback = false;


public:
	// Constructor
	HowtoScene();
	// Destructor
	~HowtoScene();

	virtual void Init();
	virtual void Update(const double dt);
	virtual void Render(void);
	virtual void Destroy(void);
};

