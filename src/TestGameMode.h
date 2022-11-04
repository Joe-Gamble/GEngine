#pragma once
#ifndef TEST_GAME_MODE_H
#define TEST_GAME_MODE_H

#include "GameMode.h"

namespace GEngine
{
	struct TestGameMode : public GameMode
	{
		inline void OnValidate() override
		{

		}

		inline void OnPointsAdjusted(GameTeam* team) override
		{

		}
	};
}


#endif
