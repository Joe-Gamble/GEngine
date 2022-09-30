#include "NetworkManager.h"
#include "NetworkUtils.h"

using namespace GEngine::Callbacks;

namespace GEngine::Networking
{
    bool NetworkManager::Initialise()
    {
        if (GNet::Network::Initialize())
        {
            initialised = true;
            shutdown = false;

            // EventDriver::Instance().RegisterCallback(std::bind(&NetworkManager::OnClientConnected, this), Event::NETWORKING_CLIENT_CONNECTED);

            return true;
        }
    }

    bool NetworkManager::InitialiseThread()
    {
        if (networkThread)
            return true;

        networkThread = SDL_CreateThread(Tick, "Network Thread", (void*)NULL);

        if (networkThread == NULL) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateThread failed: %s\n", SDL_GetError());
            return false;
        }

        return true;
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
        if (!IsClient())
        {
            if (!initialised)
            {
                if (!Initialise())
                {
                    return false;
                }
            }

            SDL_Thread* thread = SDL_CreateThread(NetworkUtils::ConnectToServer, "Network Thread", (void*)NULL);

            if (HasAuthority() || !IsServer())
            {
                bool connectionSuccessful = false;
                GameClient* newClient = new GameClient();

                if (newClient->ConnectToIP(ip))
                {
                    client = newClient;
                    EventDriver::Instance().CallEvent(Event::NETWORKING_CLIENT_CONNECTED);
                    return true;
                }

                SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "Client failed to connect to %s\n", ip.c_str());
                return connectionSuccessful;
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
        NetworkManager* nm = &NetworkManager::Instance();
        while (!nm->shutdown)
        {
            if (nm->server != nullptr)
            {
                if (nm->server->IsRunning())
                    nm->server->Tick();
            }

            if (nm->client != nullptr)
            {
                if (nm->client->IsClientConnected())
                    nm->client->Tick();
            }
        }
        return 0;
    }

    // at some point we're gonna have to worry about mutiple game loops 
    // actually I think the first check covers this

    void NetworkManager::ShutDown()
    {
        shutdown = true;

        if (client != nullptr)
            delete client;

        if (server != nullptr)
            delete server;

        initialised = false;

        GNet::Network::Shutdown();
    }

}


