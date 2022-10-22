#pragma once
#ifndef GAME_TEAM_H
#define GAME_TEAM_H

#include "GameModeTeam.h"
#include "GamePlayer.h"
#include "GameModeOutcome.h"

#include <vector>
#include <memory>

class GameTeam
{
public:
	explicit GameTeam(const GameModeTeam& settings);

	void AddPlayer(GamePlayer* player);
	void AddPoints(int points);
	bool HasPlayer(GamePlayer* player);
	GameModeOutcome Validate();

	inline bool HasMaxPlayers() { return players.size() < settings.MaxPlayers; }
	inline bool HasRequiredPlayers() { return players.size() >= settings.RequiredPlayers; }
	inline GameModeTeam& GetSettings() { return settings; }
	inline uint16_t& GetID() { return id; }
	inline int GetCurrentPlayerCount() { return players.size(); }
private:
	std::vector<std::unique_ptr<GamePlayer>> players;
	GameModeTeam settings;
	int currentPoints = 0;
	uint16_t id;
};
#endif
