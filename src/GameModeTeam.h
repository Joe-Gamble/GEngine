#pragma once
#ifndef GAMEMODE_TEAM_SETTINGS_H
#define GAMEMODE_TEAM_SETTINGS_H

#include <stdint.h>

struct GameModeTeam
{
	explicit GameModeTeam(int max, int required, bool endOnRequiredLost, int pointsToWin, uint16_t identifier) : TeamIdentifier(identifier)
	{
		PointsToWin = pointsToWin;
		RequiredPlayers = required;
		MaxPlayers = max;
		EndSessionOnRequiredLost = endOnRequiredLost;
	}

	int PointsToWin;
	int RequiredPlayers;
	int MaxPlayers;
	bool EndSessionOnRequiredLost;
	uint16_t TeamIdentifier;
};

#endif
