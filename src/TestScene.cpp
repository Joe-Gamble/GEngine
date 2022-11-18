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
		std::unique_ptr<NetEntity>* testEntity = NetworkManager::Instance().CreateNewNetEntity(scene);
		NetTransform& transform = *testEntity->get()->AddComponent<NetTransform>();
		transform.SetPosition({ 1, 1 });
	}
	else if (NetworkManager::Instance().IsClient())
	{
		std::unique_ptr<NetEntity>* testEntity = NetworkManager::Instance().CreateNewNetEntity(scene);		
		NetTransform& transform = *testEntity->get()->AddComponent<NetTransform>();		
		transform.SetPosition({ 2, 2 });
	}
}
		

void GEngine::TestScene::OnSceneDestroy()
{
}
