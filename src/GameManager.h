#pragma once
#ifndef GAME_MANAGER
#define GAME_MANAGER

#include "NetworkManager.h"
#include "EntityManager.h"

#include "GameModeSettings.h";
#include "TeamIDDefaults.h"
#include "GameTeam.h"
// SceneManager


class GameManager
{
public:
	void LoadGame(GameMode& gameSettings);
	void StartGame();
	void ValidateGame();

	GameTeam* GetTeam(uint16_t teamID);

	void AddPlayer(uint16_t teamID);

	inline bool InGame() { return inGame; }
	inline std::vector<std::unique_ptr<GameTeam>>* GetTeams() { return &teams; }

	void EndGame(GameModeOutcome outcome, GameTeam* team);

private:
	GameMode gamemode;
	bool inGame = false;

	std::vector<std::unique_ptr<GameTeam>> teams;
};

#endif
