#include "Entity.h"
#include "Transform.h"

Entity* Entity::Instantiate()
{
	Entity* go = new Entity();
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


