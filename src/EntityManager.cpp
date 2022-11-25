#include "EntityManager.h"
#include "Transform.h"
#include "NetworkManager.h"

using namespace GEngine;
using namespace GEngine::Networking;

void EntityManager::Update(double& dt)
{
	for (const auto& entity : entities)
	{
		entity->Update(dt);
	}

	for (const auto& netEntity : netEntities)
	{
		netEntity->Update(dt);
	}
}

void EntityManager::Render()
{
	for (const auto& entity : entities)
	{
		entity->Render();
	}

	for (const auto& netEntity : netEntities)
	{
		netEntity->Render();
	}
}

void EntityManager::Refresh()
{
	entities.erase(std::remove_if(std::begin(entities), std::end(entities), 
		[](const std::unique_ptr<Entity>& mEntity)
		{
			return !mEntity->IsAlive();
		}),
		std::end(entities));

	netEntities.erase(std::remove_if(std::begin(netEntities), std::end(netEntities),
		[](const std::unique_ptr<NetEntity>& mEntity)
		{
			return !mEntity->IsAlive();
		}),
		std::end(netEntities));
}

std::unique_ptr<Entity>& EntityManager::AddEntity(Entity* entity)
{
	std::unique_ptr<Entity> uPtr{ entity };
	entities.push_back(std::move(uPtr));

	return entities.front();
}	

std::unique_ptr<NetEntity>& EntityManager::AddNetEntity(NetEntity* entity)
{
	std::unique_ptr<NetEntity> uPtr{ entity };

	netEntities.push_back(std::move(uPtr));
	return netEntities.front();
}