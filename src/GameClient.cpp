#include "GameClient.h"
#include "NetTransform.h"
#include "GamePacket.h"

using namespace GEngine::Networking;

GameClient::GameClient(const std::string ip)
{
	if (Connect(ip))
	{
		
	}
}

GameClient::~GameClient()
{
	CloseConnection("Client exited the application.");
}

bool GameClient::ProcessPacket(std::shared_ptr<Packet> packet)
{
	GamePacket gamePacket = *reinterpret_cast<GamePacket*>(packet.get());
	switch (packet->GetPacketType())
	{
		case PacketType::PT_TRANSFORM:
			NetTransform transform = NetTransform();
			gamePacket >> transform;
			std::cout << "Recieved Transform:" << std::endl;
			break;
		}

	return true;
}

void GameClient::OnConnect()
{
	std::cout << "Connected to the server" << std::endl;
}

void GameClient::Tick()
{
	while (IsConnected())
	{
		Frame();
	}
}
