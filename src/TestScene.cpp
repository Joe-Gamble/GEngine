#include "TestScene.h"
#include "Transform.h"
#include "PlayerInput.h"
#include "NetworkManager.h"

using namespace GEngine;

// Check if using scene factory helps
void GEngine::TestScene::OnSceneLoad()
{
	if (NetworkManager::Instance().HasAuthority())
	{
		std::unique_ptr<NetEntity>* testEntity = NetworkManager::Instance().CreateNewNetEntity( this );
		NetTransform& transform = *testEntity->get()->AddComponent<NetTransform>();
		transform.SetPosition({ 1, 1 });
	}
	else if (NetworkManager::Instance().IsClient())
	{
		NetEntity* testEntity = NetworkManager::Instance().CreateNewNetEntity(this )->get();
		NetTransform& transform = *testEntity->AddComponent<NetTransform>();
		transform.SetPosition({ 2, 2 });

		AddNetEntity(testEntity);
	}
}
		

void GEngine::TestScene::OnSceneDestroy()
{
}
