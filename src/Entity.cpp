#include "Entity.h"
#include "Transform.h"

using namespace GEngine;

Entity* Entity::Instantiate(std::shared_ptr<Scene> _scene)
{
	Entity* go = new Entity(_scene);
	return go;
}

std::vector<std::unique_ptr<Component>>* Entity::GetComponents()
{
	return &components;
}

void Entity::Update(double& dt)
{
	for (auto& c : components)
	{
		c->Update(dt);
	}
}

void Entity::Render()
{
	for (auto& c : components)
	{
		c->Render();
	}
}

bool Entity::IsAlive() const
{
	return alive;
}

void Entity::Destroy()
{
	alive = false;
}


