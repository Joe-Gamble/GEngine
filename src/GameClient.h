#pragma once
#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H
#endif // !ENGINE_SERVER

#include "IncludeMe.h"

class GameClient : GNet::Client
{
public:
	GameClient(const std::string ip);
};
