#include "EntityManager.h"
#include "Transform.h"
#include "NetworkManager.h"

using namespace GEngine;
using namespace GEngine::Networking;

void EntityManager::Update(double& dt)
{
	for (auto& entity : entities)
	{
		entity->Update(dt);
	}
}

void EntityManager::Render()
{
	for (auto& entity : entities)
	{
		entity->Render();
	}
}

void EntityManager::Refresh()
{
	entities.erase(std::remove_if(std::begin(entities), std::end(entities), 
		[](const std::unique_ptr<Entity>& mEntity)
		{
			return mEntity->IsAlive();
		}),
		std::end(entities));

	netEntities.erase(std::remove_if(std::begin(netEntities), std::end(netEntities),
		[](const std::unique_ptr<NetEntity>& mEntity)
		{
			return mEntity->IsAlive();
		}),
		std::end(netEntities));
}

Entity& EntityManager::AddEntity()
{
	Entity* e = Entity::Instantiate(m_scene);
	std::unique_ptr<Entity> uPtr{ e };
	entities.emplace_back(std::move(uPtr));

	return *e;
}	

NetEntity& EntityManager::AddNetEntity(NetEntity* entity)
{
	// this should go in GameManager
	NetworkManager& networkManager = NetworkManager::Instance();
	
	if (networkManager.HasAuthority())
	{
		std::unique_ptr<NetEntity> uPtr{ entity };

		netEntities.emplace_back(std::move(uPtr));
		return *entity;
	}
}