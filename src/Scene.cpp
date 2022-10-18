#include "Scene.h"

using namespace GEngine;

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Init(std::unique_ptr<Scene>* ptr)
{
}

void Scene::Update(double& dt)
{
	if (IsActive())
		PollEvents();

	m_entityManager->Update(dt);
}

void Scene::Render()
{
	m_entityManager->Render();
}

void Scene::AddNetEntity(NetEntity* entity)
{
	m_entityManager->AddNetEntity(entity);
}

void Scene::PollEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
		/*
		if (NetworkManager::Instance().IsServer())
		{
			/*NetEntity& netEntity = NetworkManager::Instance().GetServer().MakeEntity();
			entityManager->AddNetEntity(&netEntity);

			netEntity.AddComponent<NetTransform>();
			NetTransform* transform = netEntity.TryGetComponent<NetTransform>();

			transform->SetPosition({ 1, 0 });

			 std::shared_ptr<GamePacket> entityPacket = std::make_shared<GamePacket>(PacketType::PT_ENTITY_CHANGE);
			 *entityPacket << netEntity;

			 std::cout << "Entity Packaged" << std::endl;

			 NetworkManager::Instance().GetServer().SendPacket(entityPacket);
			 std::cout << "Entity Sent" << std::endl;

			 // std::shared_ptr<GamePacket> packet = NetEntity::MakeEntityPacket(&entity);
		}
	*/
	}
}



