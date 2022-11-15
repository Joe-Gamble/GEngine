#include "TestScene.h"
#include "Transform.h"

using namespace GEngine;


// Check if using scene factory helps
void GEngine::TestScene::OnSceneLoad()
{
	Entity* testEntity = Entity::Instantiate(this);
	testEntity->AddComponent<Transform>();

	AddEntity(testEntity);}

void GEngine::TestScene::OnSceneDestroy()
{
}
