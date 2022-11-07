#include "TestScene.h"
#include "Transform.h"

using namespace GEngine;

void GEngine::TestScene::OnSceneLoad()
{
	Entity& testEntity = *Entity::Instantiate(this);
	testEntity.AddComponent<Transform>();

	GetEntityManager()->AddEntity(&testEntity);
}

void GEngine::TestScene::OnSceneDestroy()
{
}
