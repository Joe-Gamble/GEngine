#include "Scene.h"

using namespace GEngine;

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Update(double& dt)
{
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


