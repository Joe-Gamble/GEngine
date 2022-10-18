#include "SceneManager.h"
#include <fstream>
#include <iostream>
#include "SDL.h"

#include "nlohmann/json.hpp"

namespace GEngine
{
	void SceneManager::LoadScene(std::string& name)
	{
		std::string stream = SDL_GetBasePath();
		stream += "Data\\Scenes\\SceneManifest.json";

		std::ifstream file(stream);
		json data;
		file >> data;
		try
		{
			SceneMold sceneMold = data[0][name].get<SceneMold>();
			LoadScene(sceneMold);
		}
		catch (json::exception e)
		{
			std::cout << e.what() << std::endl;
		}
		
		
	}

	void SceneManager::LoadScene(SceneMold& mold)
	{
		if (mold.type == SceneType::UNKNOWN)
			return;

		std::unique_ptr<Scene>* scene = nullptr;

		if (mold.type == SceneType::GAME)
			scene = LoadGameScene(mold.path, mold.addative, mold.inclusive);
		else if (mold.type == SceneType::UI)
			scene = LoadUIScene(mold.path, mold.addative, mold.inclusive);

		if (scene)
		{
			scene->get()->SetBlocking(mold.blockInput);
			scene->get()->SetType(mold.type);
		}
	}

	std::unique_ptr<Scene>* SceneManager::LoadUIScene(std::string& sceneName, bool isAddative, bool showPrevious)
	{
		if (uiScenes.size() > 0)
		{
			// clean up existing scenes
			if (!isAddative)
				uiScenes.clear();
			else
				uiScenes.front()->SetActive(showPrevious);
		}

		uiScenes.push_back(std::make_unique<Scene>());
		return &uiScenes.front();
	}

	std::unique_ptr<Scene>* SceneManager::LoadGameScene(std::string& sceneName, bool isAddative, bool showPrevious)
	{
		if (gameScenes.size() > 0)
		{
			// clean up existing scenes
			if (!isAddative)
				gameScenes.clear();
			else
				gameScenes.front()->SetActive(showPrevious);
		}

		gameScenes.push_back(std::make_unique<Scene>());
		return &gameScenes.front();
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

	void SceneManager::RemoveScene(Scene* scene)
	{
		if (scene)
		{
			if (scene->IsType(SceneType::UNKNOWN))
				return;

			std::vector<std::unique_ptr<Scene>>* scenes = nullptr;


			if (scene->IsType(SceneType::GAME))
				scenes = &gameScenes;
			else if (scene->IsType(SceneType::GAME))
				scenes = &uiScenes;

			if (scenes)
			{
				for (int i = scenes->size() - 1; i >= 0; i--)
				{
					if (scene == scenes->at(i).get())
						scenes->erase(scenes->begin() + i);
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

	void SceneManager::ProcessInput()
	{
		for (int i = uiScenes.size() - 1; i >= 0; i--)
		{
			Scene* scene = uiScenes[i].get();
			scene->PollEvents();

			if (scene->IsBlocking())
				return;
		}

		for (int i = gameScenes.size() - 1; i >= 0; i--)
		{
			Scene* scene = gameScenes[i].get();
			scene->PollEvents();

			if (scene->IsBlocking())
				return;
		}
	}

	void SceneManager::Tick(double& dt)
	{
		ProcessInput();

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









