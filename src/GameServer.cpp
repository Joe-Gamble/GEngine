#include "GameServer.h"
#include "GamePacket.h"
#include "EventDriver.h"
#include "NetworkManager.h"

using namespace GEngine::Networking;
using namespace GEngine::Callbacks;

GameServer::GameServer(ServerType type) : serverType(type)
{
	if (Initialise())
	{
		isRunning = true;
		EventDriver::Instance().CallEvent(Event::NETWORKING_SERVER_READY);
	}
}

GameServer::~GameServer()
{
}

void GameServer::CloseConnectionWithClient(int clientID, const std::string& reason)
{
	CloseConnection(clientID, reason);
}

void GameServer::SendPacket(std::shared_ptr<GamePacket> packet)
{
	if (NetworkManager::Instance().HasAuthority())
	{
		NetworkManager::Instance().GetClient()->ProcessLocalPacket(packet);
	}

	for (auto& connection : connections)
	{
		connection.pm_outgoing.Append(packet);
	}
}

void GameServer::ProcessLocalPacket(std::shared_ptr<GamePacket> packet)
{
	ProcessPacket(packet, -1);
}

void GameServer::OnConnect(TCPConnection& newConnection) noexcept
{
	//NetTransform transform = NetTransform();
	//transform.SetPosition(Vector2(0, 12));

	//std::shared_ptr<GamePacket> transformPacket = std::make_shared<GamePacket>(PacketType::PT_TRANSFORM);
	//*transformPacket << transform;

	//newConnection.pm_outgoing.Append(transformPacket);
}

void GameServer::OnDisconnect(TCPConnection& lostConnection, std::string reason)
{
	std::cout << "[" << reason << "] Connection lost: " << lostConnection.ToString() << "." << std::endl;
}

void GameServer::ChangeScene(const std::string& filepath)
{
	// Load the scene from disk
	// Send this instruction to all connected clients
}

bool GameServer::ValidateComponent(NetComponent* component)
{
	return true;
}

void GameServer::ValidateClientVersion(const short& version, const int& connection)
{
	std::string errorString;

	if (!NetworkManager::VerifyNewConnection(version, connections.size() + 1, errorString))
	{
		connectionsToClose.emplace(connection, errorString);
	}
	else
	{
		// Create some kind of Identifier and send back to client? NetID? 

		// Server -> have a collection of
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

void GameServer::Tick()
{
	if (isRunning)
	{
		Frame();
	}
}

void GameServer::LeaveSession()
{
	for (auto& connection : connections)
	{
		// send a disconnect packet?
	}
}
