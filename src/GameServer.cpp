#include "GameServer.h"
#include "GamePacket.h"

GameServer::GameServer()
{
	if (Initialise())
	{
		isRunning = true;
		thread = std::make_unique<std::thread>(std::thread(&GameServer::Tick, this));
	}
}

GameServer::~GameServer()
{
	thread->detach();
}

void GameServer::TestSend()
{
	NetTransform transform = NetTransform();

	std::shared_ptr<GamePacket> transformPacket = std::make_shared<GamePacket>(PacketType::PT_TRANSFORM);
	*transformPacket << transform;
}

void GameServer::OnConnect(TCPConnection& newConnection) noexcept
{
	NetTransform transform = NetTransform();
	transform.SetPosition(Vector2(0, 12));

	std::shared_ptr<GamePacket> transformPacket = std::make_shared<GamePacket>(PacketType::PT_TRANSFORM);
	*transformPacket << transform;

	newConnection.pm_outgoing.Append(transformPacket);
}

void GameServer::OnDisconnect(TCPConnection& lostConnection, std::string reason)
{
}

bool GameServer::ProcessPacket(std::shared_ptr<Packet> packet)
{
	return false;
}

void GameServer::Tick()
{
	std::cout << "listening" << std::endl;
	while (isRunning)
	{
		Frame();
	}
}
