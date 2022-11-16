#include "NetComponent.h"
#include "NetworkManager.h"

using namespace GEngine;
using namespace Networking;

bool NetComponent::HasReachedCycles()
{	
	if (m_cyclesPassed >= m_requiredCycles)
	{
		m_cyclesPassed = 0;
		return true;
	}
	return false;
}

void NetComponent::Update(double& dt)
{
	OnUpdate(dt);

	if (HasPendingData() && HasReachedCycles())
	{
		NetEntity* netEntity = reinterpret_cast<NetEntity*>(entity);

		if (netEntity != nullptr)
		{
			NetworkManager::Instance().SendNetEntityChanges(netEntity->GetID());
		}
	}
}
