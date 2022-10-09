#pragma once
#ifndef SCENE_H
#define SCENE_H
#endif // !SCENE_H

#include <memory>
#include "EntityManager.h"

class Scene
{
public:
	Scene();
	~Scene();

	Scene(std::shared_ptr<Scene> previousScene);
	bool Back();

	void Init();
private:
	std::unique_ptr<EntityManager> entityManager = std::make_unique<EntityManager>();

	/*
	Entity* entity = entityManager->AddEntity();
	entity->AddComponent<NetTransform>();
	entity->GetComponent<NetTransform>().SetPosition({ 1, 0 });
	*/

	bool m_isActive = false;
	std::shared_ptr<Scene> m_previousScene = nullptr;
};