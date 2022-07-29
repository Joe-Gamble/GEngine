#include "Network.h"

using namespace GNet;

bool NetworkManager::Initialise()
{
    if (Network::Initialize())
    {

    }
}

bool NetworkManager::IsHost()
{
    return false;
}

int NetworkManager::GetConnectedUsers()
{
    return 0;
}
