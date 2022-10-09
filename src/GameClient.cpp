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
	if (IsConnected() && !NetworkManager::Instance().HasAuthority())
		CloseConnection("Client exited the application.");
}

bool GameClient::ProcessPacket(std::shared_ptr<Packet> packet)
{
	GamePacket gamePacket = *reinterpret_cast<GamePacket*>(packet.get());
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
		case PacketType::PT_SCENE_LOAD:
		{
			// Load the scene
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

void GameClient::OnConnect()
{
	std::cout << "Connected to the server" << std::endl;
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

void GameClient::ProcessLocalPacket(std::shared_ptr<GamePacket> packet)
{
	ProcessPacket(packet);
}

void GameClient::Tick()
{
	if (IsConnected())
	{
		Frame();
	}
}
