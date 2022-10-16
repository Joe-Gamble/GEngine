#pragma once
#ifndef SCENE_H
#define SCENE_H
#endif // !SCENE_H

#include <memory>
#include "EntityManager.h"
#include "SceneMold.h"
#include "SDL.h"

class Scene
{
public:
	Scene();
	~Scene();

	void Init(std::unique_ptr<Scene>* ptr);

	void Update(double& dt);
	void Render();

	void PollEvents();

	inline void SetActive(bool active) { m_isActive = active; }
	inline bool IsActive() { return m_isActive; }

	inline std::unique_ptr<EntityManager>* GetEntityManager() { return &m_entityManager; }
private:
	std::unique_ptr<EntityManager> m_entityManager = std::make_unique<EntityManager>(this);

	/*
	Entity* entity = entityManager->AddEntity();
	entity->AddComponent<NetTransform>();
	entity->GetComponent<NetTransform>().SetPosition({ 1, 0 });
	*/

	bool m_isActive = false;
	std::shared_ptr<Scene> m_previousScene = nullptr;
};