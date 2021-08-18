#pragma once

#include "SceneBase.h"

#include "../includes/irrKlang.h"


class MenuScene : public SceneBase
{
private:
	// Variables
	float m_worldWidth;
	float m_worldHeight;

public:
	// Constructor
	MenuScene();
	// Destructor
	~MenuScene();

	virtual void Init();
	virtual void Update(const double dt);
	virtual void Render(void);
	virtual void Destroy(void);
};

