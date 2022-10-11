#include "EntityManager.h"
#include "Transform.h"


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
}

Entity& EntityManager::AddEntity()
{
	Entity* e = Entity::Instantiate();
	std::unique_ptr<Entity> uPtr{ e };
	entities.emplace_back(std::move(uPtr));

	return *e;
}	

NetEntity& EntityManager::AddNetEntity(short& netID)
{
	NetEntity* e = NetEntity::Instantiate(netID);
	std::unique_ptr<Entity> uPtr{ e };
	entities.emplace_back(std::move(uPtr));

	return *e;
}