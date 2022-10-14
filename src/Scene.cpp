#include "Scene.h"

void Scene::Update(double& dt)
{
	entityManager->Update(dt);
}
