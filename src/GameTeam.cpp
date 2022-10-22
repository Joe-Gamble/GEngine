#include "GameTeam.h"
#include "GamePlayer.h"

GameTeam::GameTeam(const GameModeTeam& _settings) : settings(_settings)
{
	id = settings.TeamIdentifier;
	players.reserve(settings.MaxPlayers);
}

void GameTeam::AddPlayer(GamePlayer* player)
{
	if (!HasMaxPlayers())
	{
		std::unique_ptr<GamePlayer> newPlayer{ player };
		players.emplace_back(std::move(newPlayer));
	}
}

void GameTeam::AddPoints(int points)
{
	currentPoints += points;
}

bool GameTeam::HasPlayer(GamePlayer* player)
{
	if (player->GetTeam() == this)
		return true;

	return false;
}

GameModeOutcome GameTeam::Validate()
{
	if (currentPoints >= settings.PointsToWin)
	{
		return GameModeOutcome::GAME_WON;
	}
	else if (GetCurrentPlayerCount() <= 0 || (GetCurrentPlayerCount() < settings.RequiredPlayers && settings.EndSessionOnRequiredLost))
	{
		return GameModeOutcome::GAME_DISCONNECT;
	}
	else
	{
		return GameModeOutcome::GAME_IN_PLAY;
	}
	
}
