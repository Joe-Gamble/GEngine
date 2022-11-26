#include "TestScene.h"
#include "Transform.h"
#include "PlayerInput.h"
#include "NetworkManager.h"

using namespace GEngine;

// Check if using scene factory helps
void GEngine::TestScene::OnSceneLoad(std::shared_ptr<Scene>* scene)
{
	if (NetworkManager::Instance().HasAuthority())
	{
		NetEntity* testEntity = NetworkManager::Instance().CreateNewNetEntity(scene);
		NetTransform& transform = *testEntity->AddComponent<NetTransform>();
		transform.SetPosition({ 1, 1 });

		NetworkManager::Instance().SendNewEntity(testEntity);
	}
	else if (NetworkManager::Instance().IsClient())
	{
		NetEntity* testEntity = NetworkManager::Instance().CreateNewNetEntity(scene);
		NetTransform& transform = *testEntity->AddComponent<NetTransform>();		
		transform.SetPosition({ 2, 2 });

		NetworkManager::Instance().SendNewEntity(testEntity);
	}
}
		

void GEngine::TestScene::OnSceneDestroy()
{
}
