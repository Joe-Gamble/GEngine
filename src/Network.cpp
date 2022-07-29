#include "Network.h"

using namespace GNet;

bool NetworkManager::Initialise(bool isServer,  )
{
    if (Network::Initialize())
    {
        server = new Server();
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
