#pragma once

#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <memory>
#include "Entity.h"
#include "NetEntity.h"

using namespace GEngine::Networking;

namespace GEngine
{
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

		Entity& AddEntity(Entity* entity);
		NetEntity& AddNetEntity(NetEntity* entity);
	};
}

#endif
 