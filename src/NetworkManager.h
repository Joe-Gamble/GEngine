#pragma once

#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H
#endif

#include "GameServer.h"
#include "GameClient.h"

class NetworkManager
{
public:
	bool MakeServer();
	bool JoinServer(const std::string& ip);

	bool IsServer();
	bool IsClient();

	void ShutDown();
	inline bool isInitialised() { return initialised; }

private:
	bool Initialise();

	GameServer* server = nullptr;
	GameClient* client = nullptr;

	bool initialised = false;
};

