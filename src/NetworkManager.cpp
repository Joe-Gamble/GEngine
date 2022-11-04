#include "NetworkManager.h"
#include <mutex>

using namespace GEngine::Networking;
using namespace GEngine::Callbacks;

bool NetworkManager::Initialise()
{
    if (currentState == NetworkState::UNINITIALIZED || currentState == NetworkState::INITIALIZED)
    {
        if (!m_initialised)
        {
            if (!GNet::Network::Initialize())
                return false;
        }

        if (!InitialiseMutex() || !InitialiseThread())
        {
            if (networkStateMutex)
                SDL_DestroyMutex(networkStateMutex);

            if (networkThread)
                SDL_DetachThread(networkThread);
        }

        m_initialised = true;
        m_shutdown = false;

        SetState(NetworkState::INITIALIZED);
        return true;

    }
    return false;
}

bool NetworkManager::InitialiseThread()
{
    if (networkThread)
        SDL_DetachThread(networkThread);

    networkThread = SDL_CreateThread(Tick, "Network Thread", (void*)NULL);

    if (networkThread == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateThread failed: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

bool NetworkManager::InitialiseMutex()
{
    if (networkStateMutex)
        SDL_DestroyMutex(networkStateMutex);

    networkStateMutex = SDL_CreateMutex();

    if (networkStateMutex == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateMutex failed: %s\n", SDL_GetError());
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

        m_server = std::make_unique<GameServer>(type);

        if (m_server->InitialiseServer())
        {
            if (type == HOSTED)
            {
                m_client = std::make_unique<GameClient>();

                std::cout << "Server machine client created" << std::endl;
            }

            SetState(NetworkState::SESSION_ACTIVE);
            EventDriver::Instance().CallEvent(Event::NETWORKING_SERVER_READY);

            return true;
        }
    }
    return false;
}

bool NetworkManager::JoinServer(const std::string& ip)
{
    if (!IsClient() && !HasAuthority())
    {
        if (!Initialise())
        {
            return false;
        }

        m_client = std::make_unique<GameClient>();

        m_ipAddress = ip;
        SetState(NetworkState::CLIENT_CONNECTING);

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

        if (timeBetweenFrames > 1000.f / 60.0f)
        {
            if (SDL_TryLockMutex(nm->networkStateMutex) == 0)
            {
                if (nm->IsServer())
                {
                    if (nm->m_server->HasConnectionsToClose())
                    {
                        for (auto& connection : nm->m_server->GetConnectionsToClose())
                        {
                            nm->m_server->SendKickPacketToClient(connection.first, connection.second, 5000);
                        }
                        nm->m_server->ClearConnectionsToClose();
                    }
                }

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
                            std::cout << "Connected to the server" << std::endl;

                            SDL_UnlockMutex(nm->networkStateMutex);
                            nm->SetState(NetworkState::SESSION_ACTIVE);
                        }
                        else
                        {
                            EventDriver::Instance().CallEvent(Event::NETWORKING_CLIENT_CONNECT_UNSUCCESSFUL);
                            std::cout << "Couldn't connect to the server" << std::endl;

                            SDL_UnlockMutex(nm->networkStateMutex);
                            nm->SetState(NetworkState::SESSION_END);

                            nm->EndSession();
                        }
                        break;
                    }

                    case NetworkState::SESSION_ACTIVE:
                    {

                        if (nm->m_server != nullptr && nm->m_server->IsRunning())
                        {
                            EventDriver::Instance().CallEvent(Event::NETWORKING_SERVER_TICK);
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
                            if (!nm->HasAuthority() && nm->GetClient().IsConnected())
                                nm->m_client->LeaveSession();

                            nm->m_client.reset();
                        }

                        if (nm->m_server != nullptr)
                        {
                            if (nm->GetServer().InSession())
                            {
                                nm->m_server->EndSession();
                            }

                            if (nm->GetServer().HasConnectionsToClose() || (!nm->GetServer().InSession() && nm->GetServer().GetConnections().size() > 0))
                            {
                                nm->m_server->Tick();
                                break;
                            }
                            else
                            {
                                nm->m_server.reset();
                            }
                        }

                        SDL_UnlockMutex(nm->networkStateMutex);
                        nm->SetState(NetworkState::INITIALIZED);

                        std::cout << "Session Ended." << std::endl << std::endl;
                        EventDriver::Instance().CallEvent(Event::NETWORKING_SESSION_ENDED);

                        return 0;
                    }

                    case NetworkState::SHUTDOWN:
                    {
                        nm->m_shutdown = true;
                        nm->m_initialised = false;

                        SDL_UnlockMutex(nm->networkStateMutex);
                        nm->SetState(NetworkState::UNINITIALIZED);

                        SDL_DestroyMutex(nm->networkStateMutex);

                        Network::Shutdown();
                        return 0;
                    }

                    default:
                        break;
                }
                SDL_UnlockMutex(nm->networkStateMutex);
            }
            last = now;
        }
    }
    return 0;
}

int GEngine::Networking::NetworkManager::SetEndSessionState(void* data)
{
    NetworkManager::Instance().SetState(NetworkState::SESSION_END);
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
    if (IsServer())
    {
        if (m_server->GetServerType() == ServerType::HOSTED && IsClient())
        {
            return true;
        }
    }
    return false;
}

void NetworkManager::SetState(NetworkState state)
{
    SDL_LockMutex(networkStateMutex);
    currentState = state;

    SDL_UnlockMutex(networkStateMutex);
}

void NetworkManager::EndSession()
{
    if (networkThread)
    {
        SDL_DetachThread(networkThread);
        networkThread = nullptr;

        SDL_Thread* endSessionThread = SDL_CreateThread(SetEndSessionState, "End Network Session Thread", (void*)NULL);
        SDL_DetachThread(endSessionThread);
    }
}

NetEntity* NetworkManager::GetNetEntity(short id)
{
    auto it = netEntities.find(id); 
    
    if (it != netEntities.end())
    {
        return it->second->get();
    }
    
    return nullptr;
}

void NetworkManager::SendPacket(std::shared_ptr<GamePacket>& packet)
{
    if (IsServer())
        m_server->SendPacket(packet);
    else
        m_client->SendPacket(packet);
}

void NetworkManager::ShutDown()
{
    EndSession();
    SetState(NetworkState::SHUTDOWN);
}