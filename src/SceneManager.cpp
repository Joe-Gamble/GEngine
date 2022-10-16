#include "SceneManager.h"

void SceneManager::LoadScene(std::string& sceneName, bool isAddative, bool showPrevious)
{
	if (!isAddative && scenes.size() > 0)
	{
		// clean up existing scenes
		scenes.clear();
	}
	else
	{
		scenes.front()->SetActive(showPrevious);
	}

	scenes.push_back(std::make_unique<Scene>());
}

void SceneManager::LoadScene(SceneMold& mold)
{
	LoadScene(mold.sceneName, mold.addative, mold.inclusive);
}

void SceneManager::Back()
{
	if (scenes.size() > 1)
	{
		scenes.pop_back();
		scenes.front()->SetActive(true);
	}
}

void SceneManager::Tick(double& dt)
{
	for (const auto& scene : scenes)
	{
		scene->Update(dt);

		if (scene->IsActive())
			scene->Render();
	}
}
