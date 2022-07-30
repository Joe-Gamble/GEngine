#pragma once

#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H
#endif

#include "GameServer.h"
#include "GameClient.h"

class NetworkManager
{
public:
	bool Initialise();
	bool IsHost();
	int GetConnectedUsers();

	void ShutDown();

private:
	GameServer* server = nullptr;
	GameClient* client = nullptr;
};

