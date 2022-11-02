#pragma once
#ifndef GAMEMODE_H
#define GAMEMODE_H

#include <memory>
#include <vector>

#include <string>

#include "GameTeam.h"
#include "GameModeTeam.h"
#include "GameModeRules.h"

struct GameMode
{
	std::vector<GameModeTeam> Teams;
	GameModeRules Rules;
	std::string SceneName;

	bool IsMultiplayer;
	bool EndSessionOnPlayerDisconnect;

	virtual void OnValidate() = 0;
	virtual void OnPointsAdjusted(GameTeam* team) = 0;
};
#endif
