#include "GameClient.h"
#include "NetTransform.h"
#include "GamePacket.h"
#include "NetworkManager.h"

using namespace GEngine::Networking;

GameClient::GameClient()
{
}

bool GameClient::ConnectToIP(const std::string ip)
{
	if (Connect(ip))
	{
		std::shared_ptr<GamePacket> versionPacket = std::make_shared<GamePacket>(PacketType::PT_VERIFY);
		short version = NetworkManager::Version;
		*versionPacket << version;

		SendPacket(versionPacket);
		return true;
	}
	else
	{
		return false;
	}
}

void GameClient::LeaveSession()
{
	CloseConnection("Session left");
}

GameClient::~GameClient()
{
	if (!NetworkManager::Instance().HasAuthority())
		NetworkManager::Instance().EndSession();
}

bool GameClient::ProcessPacket(std::shared_ptr<Packet> packet)
{
	GamePacket& gamePacket = *reinterpret_cast<GamePacket*>(packet.get());
	switch (packet->GetPacketType())
	{
		/*case PacketType::PT_TRANSFORM:
		{
			NetTransform transform = NetTransform();
			gamePacket >> transform;

			int x = transform.GetPosition().X();
			int y = transform.GetPosition().Y();

			std::cout << "Recieved Transform: " << x << " " << y << std::endl;
			break;
		}*/
		case PacketType::PT_KICK:
		{
			std::string reason;
			*packet >> reason;

			CloseConnection(reason);
			NetworkManager::Instance().EndSession();

			break;
		}
		case PacketType::PT_SCENE_LOAD:
		{
			// Load the scene
			break;
		}
		case PacketType::PT_INVALID:
		{
			break;
		}
		case PacketType::PT_ENTITY_CHANGE:
		{
			// Eventually will grab from entityManager using netID

			NetEntity entity = NetEntity();
			gamePacket >> entity;

			return true;
			break;
		}
		default:
			return false;
	}
	return true;
}

void GameClient::OnConnect()
{
	
}

void GameClient::OnDisconnect(std::string reason)
{
	// Fallback disconnect
	// reason should be an enum I think
	std::cout << reason << std::endl;
	NetworkManager::Instance().EndSession();
}

void GameClient::SendPacket(std::shared_ptr<GamePacket> packet)
{
	if (NetworkManager::Instance().HasAuthority())
	{
		NetworkManager::Instance().GetServer().ProcessLocalPacket(packet);
	}
	else
	{
		connection.pm_outgoing.Append(packet);
	}
}

bool GameClient::ProcessLocalPacket(std::shared_ptr<Packet> packet)
{
	return ProcessPacket(packet);
}

void GameClient::Tick()
{
	if (IsConnected())
	{
		Frame();
	}
}
