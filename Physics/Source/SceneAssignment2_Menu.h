#ifndef SCENE_ASSIGNMENT_2_MENU_H
#define SCENE_ASSIGNMENT_2_MENU_H

#include "SceneBase.h"

class SceneAssignment2_Menu : public SceneBase
{
public:
	SceneAssignment2_Menu();
	~SceneAssignment2_Menu();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
protected:

	std::vector<GameObject*> m_goList;
	float m_worldWidth;
	float m_worldHeight;
};

#endif