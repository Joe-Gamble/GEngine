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

		connection.pm_outgoing.Append(versionPacket);
		return true;
	}
	else
	{
		return false;
	}
}

GameClient::~GameClient()
{
	if (IsConnected())
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

void GameClient::Tick()
{
	if (IsConnected())
	{
		Frame();
	}
}
