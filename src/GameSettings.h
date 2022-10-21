#pragma once
#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H

#include <string>

struct GameSettings
{
	std::string name;

	int screenWidth;
	int screenHeight;
	bool fullscreen = false;

	std::string startingScene;
};

#endif // !GAME_SETTINGS_H

