#pragma once
#ifndef SCENE_H
#define SCENE_H
#endif // !SCENE_H

#include <vector>
#include "Component.h"

class Scene
{
public:
	Scene();
	~Scene();

	Scene(Scene* previousScene);
	bool Back();

	void Init();
private:
	std::vector<Entity*> m_components;
	bool m_isActive = false;
	Scene* m_previousScene = nullptr;
};