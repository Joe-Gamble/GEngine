#pragma once
#ifndef GAMEMODE_RULES_H
#define GAMEMODE_RULES_H

struct GameModeRules
{
	GameModeRules() = delete;

	GameModeRules(bool drawable)
	{
		CanDraw = drawable;
	}

	GameModeRules(bool drawable, int duration)
	{
		CanDraw = drawable;
		TimeOut = true;
		GameDuration = duration;
	}

	bool TimeOut = false;
	bool CanDraw;
	int GameDuration = -1;
};
#endif