#include "GameServer.h"
#include "GamePacket.h"
#include "EventDriver.h"
#include "NetworkManager.h"

using namespace GEngine::Networking;
using namespace GEngine::Callbacks;

GameServer::GameServer(ServerType type) : serverType(type)
{
	NetworkManager::Instance().EndSession();
}

GameServer::~GameServer()
{
	if (InSession())
		EndSession();
}

bool GameServer::InitialiseServer()
{
	if (Initialise())
	{
		isRunning = true;
		inSession = true;

		entityID = 0;
		return true;
	}

	return false;
}

void GameServer::OnConnect(TCPConnection& newConnection) noexcept
{
	//NetTransform transform = NetTransform();
	//transform.SetPosition(Vector2(0, 12));

	//std::shared_ptr<GamePacket> transformPacket = std::make_shared<GamePacket>(PacketType::PT_TRANSFORM);
	//*transformPacket << transform;

	//newConnection.pm_outgoing.Append(transformPacket);
}

void GameServer::OnDisconnect(TCPConnection& lostConnection, const std::string& reason)
{
	lostConnection.SetActive(false);
}

void GameServer::SendPacket(std::shared_ptr<GamePacket> packet)
{
	if (NetworkManager::Instance().HasAuthority())
	{
		NetworkManager::Instance().GetClient().ProcessLocalPacket(packet);
	}

	for (auto& connection : connections)
	{
		if (connection.IsActive())
			connection.pm_outgoing.Append(packet);
	}
}

bool GameServer::ProcessPacket(std::shared_ptr<Packet> packet, int connectionIndex)
{
	GamePacket gamePacket = *reinterpret_cast<GamePacket*>(packet.get());
	switch (packet->GetPacketType())
	{
	case PacketType::PT_VERIFY:
	{
		short version;
		gamePacket >> version;

		ValidateClientVersion(version, connectionIndex);
		break;
	}
	case PacketType::PT_SCENE_LOAD:
	{
		// send all clients to new scene?
		// or
		// is this a confirmation that a client has loaded a scene?
		break;
	}
	case PacketType::PT_ENTITY_CHANGE:
	{
		// Entity entity = Entity();
		// Send all clients an entity to spawn
		// VALIDATE THESE CHANGES???
		break;
	}
	case PacketType::PT_ENTITY_INSTANTIATE:
	{
		// Send all clients an entity to spawn
		break;
	}
	case PacketType::PT_INVALID:
	{
		break;
	}
	default:
		return false;
	}
	return true;
}

void GameServer::ProcessLocalPacket(std::shared_ptr<GamePacket> packet)
{
	ProcessPacket(packet, -1);
}

void GameServer::ValidateClientVersion(const short& version, const int& connection)
{
	std::string errorString;

	if (!NetworkManager::VerifyNewConnection(version, connections.size() + 1, errorString))
	{
		connectionsToClose.emplace(&connections.at(connection), errorString);
	}
	else
	{
		// Create some kind of Identifier and send back to client? NetID? 

		// Server -> have a collection of
	}
}

void GameServer::SendKickPacketToClient(TCPConnection* connection, std::string& reason, long long timeout)
{
	std::shared_ptr<Packet> kickPacket = std::make_shared<Packet>(PacketType::PT_KICK);
	//Packet* packet = reinterpret_cast<Packet*>(kickPacket.get());

	*kickPacket << reason;
	connection->pm_outgoing.Append(kickPacket);

	CloseConnectionDelayedData* data = new CloseConnectionDelayedData(new TCPConnection(*connection), reason, timeout);

	SDL_Thread* thread = SDL_CreateThread(CloseConnectionDelayed, "serverThread", data);
	SDL_DetachThread(thread);
}

int GameServer::CloseConnectionDelayed(void* data)
{
	GameServer& server = NetworkManager::Instance().GetServer();

	CloseConnectionDelayedData* connectionData = (CloseConnectionDelayedData*)data;

	int timeout = connectionData->timeout;
	TCPConnection& connection = *connectionData->connection;
	std::string& reason = connectionData->reason;

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	while (connection.IsActive())
	{
		std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
		long long passed = std::chrono::duration_cast<std::chrono::milliseconds>(current - begin).count();

		if (passed > timeout)
		{
			connection.SetActive(false);
		}
	}

	if (NetworkManager::Instance().IsServer())
		server.CloseConnection(&connection, reason);

	delete connectionData;
	return 0;
}

void GameServer::ChangeScene(const std::string& filepath)
{
	// Load the scene from disk
	// Send this instruction to all connected clients
}

bool GameServer::ValidateComponent(Component* component)
{
	return true;
}

void GameServer::Tick()
{
	if (isRunning)
	{
		Frame();
	}
}

NetEntity& GameServer::MakeEntity()
{
	short netID = entityID++;
	NetEntity* entity = NetEntity::Instantiate(netID);

	return *entity;
}

void GameServer::SendEntityToClients(Entity& entity)
{
}

void GameServer::EndSession()
{ 
	inSession = false;

	for (TCPConnection& connection : connections)
	{
		std::string reason = "Session Ended";
		connectionsToClose.emplace(&connection, reason);
	}
}
