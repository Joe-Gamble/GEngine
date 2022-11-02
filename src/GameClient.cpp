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
		//case PacketType::PT_SCENE_CHANGE:
		//{
		//	// Load the scene
		//	break;
		//}
		case PacketType::PT_INVALID:
		{
			return false;
		}
		case PacketType::PT_ENTITY_INSTANTIATE:
		{
			short id = -1;
			gamePacket >> id;

			NetEntity* entity = NetworkManager::Instance().GetNetEntity(id);

			if (entity != nullptr)
			{
				gamePacket >> *entity;
			}
			break;

		}
		case PacketType::PT_ENTITY_CHANGE: // Entity exists, and we ar emaking changes to new/preexisting components
		{
			short id = -1;
			gamePacket >> id;

			NetEntity* entity = NetworkManager::Instance().GetNetEntity(id);

			if (entity != nullptr)
			{
				gamePacket >> *entity;
			}

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
