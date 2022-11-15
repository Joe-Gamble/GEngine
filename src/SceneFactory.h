#pragma once
#ifndef SCENE_FACTORY_H
#define SCENE_FACTORY_H

#include <string>
#include <unordered_map >
#include <functional>
#include "Scene.h"
#include <memory>

namespace GEngine
{
	static class SceneFactory
	{
	public:
		static SceneFactory& Instance()
		{
			static SceneFactory factoy;
			return factoy;
		}

		inline bool HasScene(std::string& name) { return scenes.count(name) > 0; }
		void AddScene(std::string name, std::shared_ptr<Scene> scenePtr)
		{
			std::function<std::shared_ptr<Scene>()> sceneFunc = ([scenePtr = std::move(scenePtr)] { return scenePtr; });
			scenes.emplace(name, sceneFunc);
		}
		
		std::shared_ptr<Scene> GetScene(std::string& name)
		{
			auto it = scenes.find(name);
			 if (it != scenes.end())
			 {
				 std::function<std::shared_ptr<Scene>()> func = it->second;

				 std::shared_ptr<Scene> scene = func();
				 return scene;
			 }

			return nullptr;
		}

	private:
		std::unordered_map<std::string, std::function<std::shared_ptr<Scene>()>> scenes;
	};
}

#endif

