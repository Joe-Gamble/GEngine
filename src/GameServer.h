#pragma once

#ifndef GAME_SERVER_H
#define GAME_SERVER_H
#endif // !ENGINE_SERVER

#include "IncludeMe.h"

class GameServer : GNet::Server
{
public:
	GameServer();
	~GameServer();
protected:
	virtual void OnConnect(TCPConnection& newConnection) override;
	virtual void OnDisconnect(TCPConnection& lostConnection, std::string reason) override;
	virtual bool ProcessPacket(std::shared_ptr<Packet> packet) override;
private:
	void Tick();
	bool isRunning = false;
};
