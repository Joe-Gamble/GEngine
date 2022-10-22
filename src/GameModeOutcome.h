#pragma once
#ifndef GAMEMODE_OUTCOME_H
#define GAMEMODE_OUTCOME_H

#include <stdint.h>

enum GameModeOutcome : uint16_t
{
	GAME_SHUTDOWN,
	GAME_DISCONNECT,
	GAME_WON,
	GAME_LOST,
	GAME_DRAW,
	GAME_IN_PLAY
};
#endif