#pragma once

#include <map>
#include <string>

#include "SingletonPattern.h"
#include "SceneBase.h"

class SceneManager : public SingletonPattern<SceneManager>
{
	friend SingletonPattern<SceneManager>;
public:
	void Destroy();
	void Update(double dt);
	void Render();

	// User Interface
	bool AddScene(const std::string& sceneName, SceneBase* scene);
	bool RemoveScene(const std::string& sceneName);
	bool SetActiveScene(const std::string& sceneName);
	bool SetActiveScene(SceneBase* scene);
	bool CheckSceneExists(const std::string& sceneName);
	void ResetActiveScene();
	SceneBase* GetActiveScene();
	SceneBase* GetPrevScene();

protected:
	// Constructor
	SceneManager();
	~SceneManager();

	// Map of GameStates
	std::map<std::string, SceneBase*> sceneMap;
	// The handlers containing the active and next scene
	SceneBase* activeScene, * nextScene, * prevScene;
};

