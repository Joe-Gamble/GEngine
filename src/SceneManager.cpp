#include "SceneManager.h"
#include <fstream>
#include <iostream>
#include "SDL.h"

#include "nlohmann/json.hpp"
#include "SceneFactory.h"

namespace GEngine
{
	void SceneManager::LoadScene(const std::string& name)
	{
		std::unique_ptr<SceneMold> mold = GetSceneData(name);

		if (mold)
			LoadScene(*mold.get());
	}

	void SceneManager::Init()
	{
		path = SDL_GetBasePath();
	}

	void SceneManager::LoadScene(SceneMold& mold)
	{
		std::shared_ptr<Scene> scene = nullptr;

		if (mold.type == SceneType::GAME)
			scene = AddScene(mold, gameScenes);
		else if (mold.type == SceneType::UI)
			scene = AddScene(mold, uiScenes);

		if (scene)
		{
			std::cout << "Scene loaded successfully: " << mold.name << std::endl;
			scene->OnSceneLoad();

			scene->SetBlocking(mold.blockInput);
			scene->SetType(mold.type);
		}
	}

	std::unique_ptr<SceneMold> SceneManager::GetSceneData(const std::string& sceneName)
	{
		std::string stream = path += "Data\\Scenes\\SceneManifest.json";

		std::ifstream file(stream);
		json data;
		try
		{
			file >> data;
			std::unique_ptr<SceneMold> sceneMold = std::make_unique<SceneMold>(data[0][sceneName].get<SceneMold>());

			if (sceneMold)
			{
				sceneMold->name = sceneName;
				return sceneMold;
			}
			
		}
		catch (json::exception e)
		{
			std::cout << e.what() << std::endl;
		}

		return nullptr;
	}

	std::shared_ptr<Scene> SceneManager::AddScene(SceneMold& mold, std::vector<std::shared_ptr<Scene>>& sceneContainer)
	{
		std::shared_ptr<Scene> scene = SceneFactory::Instance().GetScene(mold.name);

		if (scene && !HasScene(scene))
		{
			if (sceneContainer.size() > 0)
			{
				// clean up existing scenes
				if (!mold.addative)
					sceneContainer.clear();
				else
					sceneContainer.front()->SetActive(mold.inclusive);
			}

			sceneContainer.push_back(scene);
			return sceneContainer.front();
		}
		return nullptr;
	}

	void SceneManager::ClearUIScenes()
	{
		uiScenes.clear();
	}

	void SceneManager::ClearScenes()
	{
		ClearUIScenes();
		gameScenes.clear();
	}

	bool SceneManager::HasScene(std::shared_ptr<Scene> scene)
	{
		if (scene->GetType() == SceneType::GAME)
		{
			for (const auto& gameScene : gameScenes)
			{
				if (scene == gameScene)
				{
					return true;
				}
			}
		}
		else if (scene->GetType() == SceneType::UI)
		{
			for (const auto& uiScene : uiScenes)
			{
				if (scene == uiScene)
				{
					return true;
				}
			}
		}

		return false;
	}

	void SceneManager::RemoveScene(Scene* scene)
	{
		if (scene)
		{
			if (scene->IsType(SceneType::UNKNOWN))
				return;

			std::vector<std::shared_ptr<Scene>>* scenes = nullptr;


			if (scene->IsType(SceneType::GAME))
				scenes = &gameScenes;
			else if (scene->IsType(SceneType::UI))
				scenes = &uiScenes;

			if (scenes)
			{
				for (int i = scenes->size() - 1; i >= 0; i--)
				{
					if (scene == scenes->at(i).get())
					{
						scene->OnSceneDestroy();
						scenes->erase(scenes->begin() + i);
					}
				}
			}
		}
	}

	void SceneManager::Back()
	{
		if (uiScenes.size() > 1)
		{
			uiScenes.pop_back();
			uiScenes.front()->SetActive(true);
		}
	}

	void SceneManager::Tick(double& dt)
	{
		for (const auto& scene : gameScenes)
		{
			scene->Update(dt);

			if (scene->IsActive())
				scene->Render();
		}

		for (const auto& scene : uiScenes)
		{
			scene->Update(dt);

			if (scene->IsActive())
				scene->Render();
		}
	}
}









