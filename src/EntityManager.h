#pragma once

#include <vector>
#include <memory>
#include "Entity.h"


class EntityManager
{
private:
	std::vector<std::unique_ptr<Entity>> entities;

public:
	void Update(double& dt);
	void Render();

	void Refresh();
	Entity& AddEntity();
};
