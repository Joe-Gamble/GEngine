#include "NetworkManager.h"

bool NetworkManager::Initialise()
{
    if (GNet::Network::Initialize())
    {
        server = new GameServer();
        return true;
    }
}

bool NetworkManager::IsHost()
{
    return server != nullptr;
}

int NetworkManager::GetConnectedUsers()
{
    return 0;
}

void NetworkManager::ShutDown()
{
    if (server != nullptr)
        delete server;

    if (client != nullptr)
        delete server;

    GNet::Network::Shutdown();
}
