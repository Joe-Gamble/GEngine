#pragma once

#include <vector>
#include <memory>
#include "Entity.h"
#include "NetEntity.h"


class EntityManager
{
private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::vector<std::unique_ptr<NetEntity>> netEntities;

public:
	void Update(double& dt);
	void Render();

	void Refresh();
	Entity& AddEntity();
	NetEntity& AddNetEntity(short netID);
};
