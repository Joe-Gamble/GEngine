#pragma once
#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H


#include "Scene.h"
#include "SceneMold.h"

#include <vector>
#include <memory>

using namespace GEngine::Data;

namespace GEngine
{
	using json = nlohmann::json;

	class SceneManager
	{
	public:
		static inline SceneManager& Instance()
		{
			static SceneManager instance;
			return instance;
		}

		void LoadScene(SceneMold& mold);
		void LoadScene(std::string& name);

		void ClearUIScenes();
		void ClearScenes();

		bool HasScene(std::shared_ptr<Scene> scene);

		void RemoveScene(Scene* scene);

		void Back();
		void Tick(double& dt);

	private:
		std::shared_ptr<Scene>* LoadUIScene(std::string& sceneName, bool isAddative, bool showPreviouse);
		std::shared_ptr<Scene>* LoadGameScene(std::string& sceneName, bool isAddative, bool showPreviouse);

		std::vector<std::shared_ptr<Scene>> gameScenes;
		std::vector<std::shared_ptr<Scene>> uiScenes;


	};
}

#endif // !SCENE_MANAGER_H

