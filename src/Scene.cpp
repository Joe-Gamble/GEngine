#include "Scene.h"

using namespace GEngine;

void Scene::Update(double& dt)
{
	m_entityManager->Refresh();
	m_entityManager->Update(dt);

	OnUpdate(dt);
}

void Scene::Render()
{
	m_entityManager->Render();

	OnRender();
}

std::unique_ptr<NetEntity>* Scene::AddNetEntity(NetEntity* entity)
{
	return &m_entityManager->AddNetEntity(entity);
}

std::unique_ptr<Entity>* Scene::AddEntity(Entity* entity)
{
	return &m_entityManager->AddEntity(entity);
}