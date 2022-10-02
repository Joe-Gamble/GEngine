#include "GameServer.h"
#include "GamePacket.h"
#include "EventDriver.h"

using namespace GEngine::Networking;
using namespace GEngine::Callbacks;

GameServer::GameServer()
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
	// Do we want clients to be able to reconnect to a session or do we quit the game if we lose our player?
}

bool GameServer::ProcessPacket(std::shared_ptr<Packet> packet)
{
	return false;
}

void GameServer::Tick()
{
	if (isRunning)
	{
		Frame();
	}
}
