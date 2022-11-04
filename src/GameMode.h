#pragma once
#ifndef GAMEMODE_H
#define GAMEMODE_H

#include <memory>
#include <vector>

#include <string>

#include "GameTeam.h"
#include "GameModeTeam.h"
#include "GameModeRules.h"

#include "SceneFactory.h"

namespace GEngine
{
	struct GameMode
	{
		GameMode() = default;
		~GameMode() = default;

		std::vector<GameModeTeam> Teams;
		GameModeRules Rules;
		std::string SceneName;

		bool IsMultiplayer;
		bool EndSessionOnPlayerDisconnect;

		inline virtual void RegisterScenes() = 0;
		inline virtual void OnValidate() = 0;
		inline virtual void OnPointsAdjusted(GameTeam* team) = 0;
	};
}

#endif
