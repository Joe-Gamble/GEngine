#include "NetworkManager.h"
#include "NetworkUtils.h"

using namespace GEngine::Callbacks;

namespace GEngine::Networking
{
    bool NetworkManager::Initialise()
    {
        if (currentState == NetworkState::UNINITIALIZED)
        {
            if (GNet::Network::Initialize())
            {
                if (InitialiseThread())
                {
                    m_initialised = true;
                    m_shutdown = false;

                    currentState = NetworkState::INITIALIZED;
                    return true;
                }
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
        NetworkManager* nm = &NetworkManager::Instance();

        while (!nm->m_shutdown)
        {
            switch (nm->currentState)
            {
                case NetworkState::UNINITIALIZED:
                case NetworkState::INITIALIZED:
                case NetworkState::FREE:
                    break;

                case NetworkState::CLIENT_CONNECTING:
                {
                    const std::string& ip = nm->m_ipAddress;
                    std::cout << "Attempting to connect to " + ip << std::endl;

                    if (nm->m_client->ConnectToIP(ip))
                    {
                        EventDriver::Instance().CallEvent(Event::NETWORKING_CLIENT_CONNECT_SUCCESSFUL);
                        nm->currentState = NetworkState::CLIENT_SERVER_TICK;
                    }
                    else
                    {
                        EventDriver::Instance().CallEvent(Event::NETWORKING_CLIENT_CONNECT_UNSUCCESSFUL);
                        
                        nm->currentState = NetworkState::INITIALIZED;
                        nm->m_ipAddress = "";
                    }
                    break;
                }
            
                case NetworkState::CLIENT_SERVER_TICK:
                {
                    if (nm->m_server != nullptr)
                    {
                        if (nm->m_server->IsRunning())
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
                    return 0;
                }

                default:
                    break;
            }
        }
        return 0;
    }

    // at some point we're gonna have to worry about mutiple game loops 
    // actually I think the first check covers this

    void NetworkManager::ShutDown()
    {
        currentState = NetworkState::SHUTDOWN;
        GNet::Network::Shutdown();

        if (m_client != nullptr)
            delete m_client;

        if (m_server != nullptr)
            delete m_server;

        m_initialised = false;
    }

}


