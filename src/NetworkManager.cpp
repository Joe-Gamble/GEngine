#include "NetworkManager.h"
#include "NetworkUtils.h"

using namespace GEngine::Callbacks;

namespace GEngine::Networking
{
    bool NetworkManager::Initialise()
    {
        if (currentState == NetworkState::UNINITIALIZED || currentState == NetworkState::INITIALIZED)
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

    bool NetworkManager::MakeServer(ServerType type)
    {
        if (!IsServer() && !IsClient())
        {
            if (!Initialise())
            {
                return false;
            }
                
            if (type == HOSTED)
            {
                m_client = std::make_unique<GameClient>();

                std::cout << "Server machine client created" << std::endl;
            }

            m_server = std::make_unique<GameServer>(type);

            currentState = NetworkState::SESSION_ACTIVE;
            return true;
        }
        return false;
    }

    bool NetworkManager::JoinServer(const std::string& ip)
    {
        if (!IsClient() && !HasAuthority())
        {
            if (!m_initialised)
            {
                if (!Initialise())
                {
                    return false;
                }
            }
            m_client = std::make_unique<GameClient>();

            m_ipAddress = ip;
            currentState = NetworkState::CLIENT_CONNECTING;
            
            return true;

        }
        std::cout << "Client already exists or this was called on a dedicated server. " << std::endl;
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
                        nm->SetState(NetworkState::SESSION_ACTIVE);
                    }
                    else
                    {
                        EventDriver::Instance().CallEvent(Event::NETWORKING_CLIENT_CONNECT_UNSUCCESSFUL);

                        nm->ShutDown();
                    }
                    break;
                }

                case NetworkState::SESSION_ACTIVE:
                {

                    if (nm->m_server != nullptr && nm->m_server->IsRunning())
                    {
                        if (nm->m_server->HasConnectionsToClose())
                        {
                            for (auto& connection : nm->m_server->GetConnectionsToClose())
                            {
                                nm->m_server->SendKickPacketToClient(connection.first, connection.second, 5000);
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

                case NetworkState::SESSION_END:
                {
                    if (nm->m_client != nullptr)
                    {
                        if (!nm->HasAuthority())
                            nm->m_client->LeaveSession();

                        nm->m_client.reset();
                    }

                    if (nm->m_server != nullptr)
                    {
                        nm->m_server->LeaveSession();

                        nm->m_server.reset();
                    }

                    nm->SetState(NetworkState::INITIALIZED);
                    std::cout << "Session Ended." << std::endl << std::endl;;
                    return 0;
                }

                case NetworkState::SHUTDOWN:
                {
                    nm->m_shutdown = true;
                    nm->m_initialised = false;

                    nm->SetState(NetworkState::UNINITIALIZED);

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

    bool NetworkManager::VerifyNewConnection(const short& version, const int& playerCount, std::string& error)
    {
        if (version != Version)
        {
            error = "Invalid Version";
            return false;
        }
        else
            std::cout << "Version verified." << std::endl;
        

        if (playerCount > MaxPlayers)
        {
            error = "Connection Limit";
            return false;
        }

        return true;
    }

    bool NetworkManager::HasAuthority()
    {
        if (m_server != nullptr)
        {
            if (m_server->GetServerType() == ServerType::HOSTED)
            {
                return true;
            }
        }
        return false;
    }

    void NetworkManager::EndSession()
    {
        currentState = NetworkState::SESSION_END;
        SDL_DetachThread(networkThread);
        networkThread = nullptr;
    }

    void NetworkManager::ProcessLocalPacket(std::shared_ptr<GamePacket>& packet)
    {
        m_client->ProcessLocalPacket(packet);
    }

    void NetworkManager::ShutDown()
    {
        EndSession();
        SetState(NetworkState::SHUTDOWN);
    }

}


