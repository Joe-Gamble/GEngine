#include "NetworkManager.h"

using namespace GEngine::Networking;

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
        networkThread = SDL_CreateThread(Tick, "Network Thread", (void*)NULL);
        
        return true;
    }
    return false;
}

bool NetworkManager::JoinServer(const std::string& ip)
{
    client = new GameClient(ip);
    return true;

    if (!IsServer() && !IsClient())
    {
        if (!initialised)
        {
            if (!Initialise())
            {
                return false;
            }
        }
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

int NetworkManager::Tick(void* data)
{
    NetworkManager nm = NetworkManager::Instance();

    if (nm.server != nullptr)
    {
        while (nm.server->IsRunning())
            nm.server->Tick();
    }
    else if (nm.client != nullptr)
    {
        while (nm.client->IsConnected())
            nm.client->Tick();
    }
    return 0;
}

void NetworkManager::ShutDown()
{
    if (client != nullptr)
        delete client;
    
    if (server != nullptr)
        delete server;

    initialised = false;

    GNet::Network::Shutdown();
}
