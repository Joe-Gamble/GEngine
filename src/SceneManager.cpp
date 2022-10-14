#include "SceneManager.h"

void SceneManager::LoadScene(std::string& sceneName, bool isAddative)
{
	std::unique_ptr<Scene> scene;

	if (isAddative && scenes.size() > 0)
	{
		std::unique_ptr<Scene>* previousScene = &scenes.front();
		scene = std::make_unique<Scene>(previousScene);
	}
	else
	{
		if (scenes.size() > 0)
		{
			// cleanup scenes
			scene = std::make_unique<Scene>();
		}
	}
	scenes.push_back(scene);
}

void SceneManager::LoadScene(SceneMold& mold)
{
	LoadScene(mold.sceneName, mold.addative);
}

void SceneManager::Tick(double& dt)
{
	for (const auto& scene : scenes)
	{
		scene->Update();
		scene->Render();
	}
}
