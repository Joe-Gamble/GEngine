#pragma once

#include <vector>
#include <memory>
#include "Entity.h"
#include "NetEntity.h"

using namespace GEngine::Networking;

class Scene;

class EntityManager
{
public:
	EntityManager(Scene* scene) : m_scene(scene) {}
private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::vector<std::unique_ptr<NetEntity>> netEntities;

	Scene* m_scene = nullptr;

public:
	void Update(double& dt);
	void Render();

	void Refresh();
	Entity& AddEntity();
	NetEntity& AddNetEntity(NetEntity* entity);
};
 