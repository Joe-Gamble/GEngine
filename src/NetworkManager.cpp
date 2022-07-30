#include "NetworkManager.h"

bool NetworkManager::Initialise()
{
    if (GNet::Network::Initialize())
    {
        initialised = true;
        return true;
    }
}

bool NetworkManager::MakeServer()
{
    if (!IsServer() && !IsClient())
    {
        if (!initialised)
        {
            if (!Initialise())
            {
                return false;
            }
        }

        server = new GameServer();
        return true;
    }
    return false;
}

bool NetworkManager::JoinServer(const std::string& ip)
{
    if (!IsServer() && !IsClient())
    {
        if (!initialised)
        {
            if (!Initialise())
            {
                return false;
            }
        }

        client = new GameClient(ip);
        return true;
    }
    return false;
}

bool NetworkManager::IsServer()
{
    return server != nullptr;
}

bool NetworkManager::IsClient()
{
    return client != nullptr;
}

void NetworkManager::ShutDown()
{
    if (server != nullptr)
        delete server;

    if (client != nullptr)
        delete server;

    initialised = false;

    GNet::Network::Shutdown();
}
