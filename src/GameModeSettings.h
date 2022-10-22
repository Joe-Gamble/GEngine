#pragma once
#ifndef GAMEMODE_H
#define GAMEMODE_H

#include <memory>
#include <vector>

#include "GameModeTeam.h"
#include "GameModeRules.h"

struct GameMode
{
	std::vector<GameModeTeam> Teams;
	GameModeRules Rules;
	std::string SceneName;

	bool IsMultiplayer;
	bool EndSessionOnPlayerDisconnect;
};
#endif
