#pragma once
#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H


#include "Scene.h"
#include "SceneMold.h"
#include <vector>
#include <memory>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

using namespace GEngine::Data;

namespace GEngine
{
	class SceneManager
	{
	public:
		static inline SceneManager& Instance()
		{
			static SceneManager instance;
			return instance;
		}

		void LoadScene(SceneMold& mold);
		void LoadScene(std::string& path);

		void ClearUIScenes();
		void ClearScenes();

		void RemoveScene(Scene* scene);

		void Back();

		void ProcessInput();
		void Tick(double& dt);

	private:
		std::unique_ptr<Scene>* LoadUIScene(std::string& sceneName, bool isAddative, bool showPreviouse);
		std::unique_ptr<Scene>* LoadGameScene(std::string& sceneName, bool isAddative, bool showPreviouse);

		std::vector<std::unique_ptr<Scene>> gameScenes;
		std::vector<std::unique_ptr<Scene>> uiScenes;
	};
}

#endif // !SCENE_MANAGER_H
