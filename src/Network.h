#pragma once

#ifndef NETWORK_H
#define NETWORK_H
#endif

#include <IncludeMe.h>

static class NetworkManager
{
public:
	bool Initialise();
	bool IsHost();
	int GetConnectedUsers();

private:
	Server* server = nullptr;
	Client* client = nullptr;
};

