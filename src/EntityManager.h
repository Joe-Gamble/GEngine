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
	private:
		std::vector<std::unique_ptr<Entity>> entities;
		std::vector<std::unique_ptr<NetEntity>> netEntities;

	public:
		void Update(double& dt);
		void Render();

		void Refresh();

		std::unique_ptr<Entity>& AddEntity(Entity* entity);
		std::unique_ptr<NetEntity>& AddNetEntity(NetEntity* entity);
	};
}

#endif
 