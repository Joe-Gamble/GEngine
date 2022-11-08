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
		void Init();

		void LoadScene(SceneMold& mold);
		void LoadScene(const std::string& name);

		void ClearUIScenes();
		void ClearScenes();

		bool HasScene(std::shared_ptr<Scene> scene);

		void RemoveScene(Scene* scene);

		void Back();
		void Tick(double& dt);

	private:
		std::unique_ptr<SceneMold> GetSceneData(const std::string& sceneName);
		std::shared_ptr<Scene> AddScene(SceneMold& mold, std::vector<std::shared_ptr<Scene>>& sceneContainer);

		std::vector<std::shared_ptr<Scene>> gameScenes;
		std::vector<std::shared_ptr<Scene>> uiScenes;

		std::string path;
	};
}

#endif // !SCENE_MANAGER_H

