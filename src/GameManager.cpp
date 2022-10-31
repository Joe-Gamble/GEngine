#include "GameManager.h"

void GameManager::LoadGame(GameMode& _gamemode)
{
	gamemode = _gamemode;

	for (const auto& teamSettings : gamemode.Teams)
	{
		teams.push_back(std::make_unique<GameTeam>(GameTeam(teamSettings)));
	}
}

void GameManager::StartGame()
{
	inGame = true;

	
}

void GameManager::ValidateGame()
{
	for (const auto& team : teams)
	{
		GameModeOutcome outcome = team->Validate();
		if (outcome != GameModeOutcome::GAME_IN_PLAY)
		{
			EndGame(outcome, team.get());
		}
	}
}

GameTeam* GameManager::GetTeam(uint16_t teamID)
{
	for (auto& team : teams)
	{
		if (team->GetID() == teamID)
			return team.get();
	}
	return nullptr;
}

void GameManager::AddPlayer(uint16_t teamID = TeamIDDefaults::INVALID)
{
	if (gamemode.IsMultiplayer && !NetworkManager::Instance().HasAuthority())
		return;

	GameTeam* teamToAdd = nullptr;

	// we have no preference for team, so lets get the best team to add a player to
	if (teamID == TeamIDDefaults::INVALID)
	{
		int mostMissing = 0;

		for (const auto& team : teams)
		{
			GameModeTeam& teamSettings = team->GetSettings();
			int playersMissing = teamSettings.MaxPlayers - team->GetCurrentPlayerCount();

			if (playersMissing > mostMissing)
			{
				mostMissing = playersMissing;
				teamToAdd = team.get();
			}
		}
	}
	else // we have a particular team we want to add a player to
	{
		teamToAdd = GetTeam(teamID);
	}

	if (teamToAdd != nullptr)
	{
		GamePlayer* player = new GamePlayer(*teamToAdd);
		teamToAdd->AddPlayer(player);
	}
}

void GameManager::EndGame(GameModeOutcome outcome, GameTeam* team = nullptr)
{
	switch (outcome)
	{
		case GameModeOutcome::GAME_SHUTDOWN:
		{
			// Send a kick packet to all clients notifying the server shut down
			break;
		}
		case GameModeOutcome::GAME_DISCONNECT:
		{
			// Send a kick packet to all clients notifying a player disconnected
			break;
		}
		case GameModeOutcome::GAME_WON:
		{
			// Send clients an endgame scene, we can determine what teams have won by validating them again,we are going to have to determine draws, not sure how I'm going to do that yet
			break;
		}
		default:
			return;
	}
}
