#pragma once
#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H
#endif // !SCENE_MANAGER_H


#include "Scene.h"
#include "SceneMold.h"
#include <vector>
#include <memory>

class SceneManager
{
public:
	static inline SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	void LoadScene(std::string& sceneName, bool isAddative, bool showPrevious);
	void LoadScene(SceneMold& mold);

	void Back();

	void Tick(double& dt);

private:
	std::vector<std::unique_ptr<Scene>> scenes;
};