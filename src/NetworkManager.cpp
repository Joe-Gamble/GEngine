#include "NetworkManager.h"
#include "NetworkUtils.h"

using namespace GEngine::Callbacks;

namespace GEngine::Networking
{
    bool NetworkManager::Initialise()
    {
        if (currentState == NetworkState::UNINITIALIZED)
        {
            if (!m_initialised)
            {
                if (!GNet::Network::Initialize())
                    return false;
            }
            
            if (InitialiseThread())
            {
                m_initialised = true;
                m_shutdown = false;

                currentState = NetworkState::INITIALIZED;
                return true;
            }
            
        }

        return false;
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
            if (!m_initialised)
            {
                if (!Initialise())
                {
                    return false;
                }
            }

            m_server = new GameServer();
            currentState = NetworkState::CLIENT_SERVER_TICK;
            return true;
        }
        return false;
    }

    bool NetworkManager::JoinServer(const std::string& ip)
    {
        if (!IsClient())
        {
            if (!m_initialised)
            {
                if (!Initialise())
                {
                    return false;
                }
            }

            if (HasAuthority() || !IsServer())
            {
                m_client = new GameClient();
                m_ipAddress = ip;
                currentState = NetworkState::CLIENT_CONNECTING;

                return true;
            }
        }
        return false;
    }

    bool NetworkManager::IsServer()
    {
        return m_server != nullptr;
    }

    bool NetworkManager::IsClient()
    {
        return m_client != nullptr;
    }

    int NetworkManager::Tick(void* data)
    {
        unsigned int now = SDL_GetTicks();
        unsigned int last = SDL_GetTicks();

        double timeBetweenFrames = 0;

        NetworkManager* nm = &NetworkManager::Instance();

        while (!nm->m_shutdown)
        {
            now = SDL_GetTicks();
            timeBetweenFrames = now - last;

            if (timeBetweenFrames > 1000 / 60.0)
            {
                switch (nm->currentState)
                {
                case NetworkState::UNINITIALIZED:
                case NetworkState::INITIALIZED:
                    break;

                case NetworkState::CLIENT_CONNECTING:
                {
                    const std::string& ip = nm->m_ipAddress;

                    if (nm->m_client->ConnectToIP(ip))
                    {
                        EventDriver::Instance().CallEvent(Event::NETWORKING_CLIENT_CONNECT_SUCCESSFUL);
                        nm->SetState(NetworkState::CLIENT_SERVER_TICK);
                    }
                    else
                    {
                        EventDriver::Instance().CallEvent(Event::NETWORKING_CLIENT_CONNECT_UNSUCCESSFUL);

                        nm->ShutDown();
                    }
                    break;
                }

                case NetworkState::CLIENT_SERVER_TICK:
                {

                    if (nm->m_server != nullptr && nm->m_server->IsRunning())
                    {
                        if (nm->m_server->HasConnectionsToClose())
                        {
                            for (auto const& connection : *nm->m_server->GetConnectionsToClose())
                            {
                                nm->m_server->CloseConnectionWithClient(connection.first, connection.second);
                            }

                            nm->m_server->ClearConnectionsToClose();
                        }

                        nm->m_server->Tick();
                    }

                    if (nm->m_client != nullptr)
                    {
                        if (nm->m_client->IsClientConnected())
                            nm->m_client->Tick();
                    }

                    break;
                }

                case NetworkState::SHUTDOWN:
                {
                    nm->m_shutdown = true;
                    nm->SetState(NetworkState::UNINITIALIZED);

                    if (nm->m_client != nullptr)
                    {
                        delete nm->m_client;
                        nm->m_client = nullptr;
                    }

                    if (nm->m_server != nullptr)
                    {
                        delete nm->m_server;
                        nm->m_server = nullptr;
                    }

                    nm->m_initialised = false;

                    Network::Shutdown();
                    return 0;
                }

                default:
                    break;
                }
            } 
        }
        return 0;
    }

    void NetworkManager::ShutDown()
    {
        currentState = NetworkState::SHUTDOWN;
        SDL_DetachThread(networkThread);
        networkThread = nullptr;
    }

}


