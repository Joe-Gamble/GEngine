#include "GameServer.h"
#include <thread>

GameServer::GameServer()
{
	if (Initialise())
	{
		isRunning = true;
		std::thread(Tick());
	}
}

GameServer::~GameServer()
{
	
}

void GameServer::OnConnect(TCPConnection& newConnection)
{
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
	while (isRunning)
	{
		Frame();
	}
}
