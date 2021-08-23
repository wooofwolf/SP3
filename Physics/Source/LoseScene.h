#pragma once

#include "SceneBase.h"

#include "../includes/irrKlang.h"


class LoseScene : public SceneBase
{
private:
	// Variables
	float m_worldWidth;
	float m_worldHeight;

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

