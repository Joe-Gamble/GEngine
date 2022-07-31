#pragma once

#ifndef GAME_SERVER_H
#define GAME_SERVER_H
#endif // !ENGINE_SERVER

#include "IncludeMe.h"
#include <thread>

class GameServer : public GNet::Server
{
public:
	GameServer();
	~GameServer();

	void TestSend();
protected:
	virtual void OnConnect(TCPConnection& newConnection) noexcept override;
	virtual void OnDisconnect(TCPConnection& lostConnection, std::string reason) override;
	virtual bool ProcessPacket(std::shared_ptr<Packet> packet) override;
private:
	void Tick();
	bool isRunning = false;
	std::unique_ptr<std::thread> thread;
};
