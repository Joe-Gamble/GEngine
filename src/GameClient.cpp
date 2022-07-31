#include "GameClient.h"
#include "NetTransform.h"
#include "GamePacket.h"

GameClient::GameClient(const std::string ip)
{
	if (Connect(ip))
	{
		thread = std::make_unique<std::thread>(std::thread(&GameClient::Tick, this));
	}
}

GameClient::~GameClient()
{
	CloseConnection("Client Exited the application.");
	thread->detach();
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
	std::cout << "COnnected to the server" << std::endl;
}

void GameClient::Tick()
{
	while (IsConnected())
	{
		Frame();
	}
}
