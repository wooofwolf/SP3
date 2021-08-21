#include "SceneManager.h"

void SceneManager::Destroy()
{
	activeScene = nextScene = prevScene = nullptr;

	// Delete all scenes stored in the map
	std::map<std::string, SceneBase*>::iterator it, end;
	end = sceneMap.end();
	for (it = sceneMap.begin(); it != end; ++it)
	{
		delete it->second;
	}
	// Clear the map
	sceneMap.clear();
}

void SceneManager::Update(double dt)
{
	// Check for change of scene
	if (nextScene != activeScene)
	{
		if (activeScene)
		{
			// Scene is valid, need to call appropriate function to exit
			activeScene->Exit();
		}
		//
		// Set the previous CScene
		prevScene = activeScene;
		// Set the new active CScene
		activeScene = nextScene;
		// Init the new active CScene
		activeScene->Init();
	}

	// Update the active CScene
	if (activeScene)
	{
		activeScene->Update(dt);
	}

}

void SceneManager::Render()
{
	if (activeScene)
		activeScene->Render();
}

bool SceneManager::AddScene(const std::string& sceneName, SceneBase* scene)
{
	// Check if this _name already exists in the map...
	if (CheckSceneExists(sceneName))
	{
		// If it does not exist, then unable to proceed
		return false;
	}

	// Check if the _scene is valid
	if (scene == nullptr)
	{
		return false;
	}

	// If nothing is wrong, then add the scene to our map
	sceneMap[sceneName] = scene;

	return true;
}

bool SceneManager::RemoveScene(const std::string& sceneName)
{
	// Check if this _name does not exists in the map...
	if (!CheckSceneExists(sceneName))
	{
		// If it does not exist, then unable to proceed
		return false;
	}

	// Find the CGameState*
	SceneBase* target = sceneMap[sceneName];
	if (target == activeScene || target == nextScene)
	{
		return false;
	}

	// Delete and remove from our map
	delete target;
	sceneMap.erase(sceneName);

	return true;
}

bool SceneManager::SetActiveScene(const std::string& sceneName)
{
	// Check if this _name does not exists in the map...
	if (!CheckSceneExists(sceneName))
	{
		// If it does not exist, then unable to proceed
		return false;
	}

	// Scene exist, set the next scene pointer to that scene
	nextScene = sceneMap[sceneName];

	return true;
}

bool SceneManager::CheckSceneExists(const std::string& sceneName)
{
	return sceneMap.count(sceneName) != 0;
}

SceneBase* SceneManager::GetActiveScene()
{
	return activeScene;
}

SceneManager::SceneManager()
	: activeScene(nullptr)
	, nextScene(nullptr)
	, prevScene(nullptr)
{
}

SceneManager::~SceneManager()
{
}
