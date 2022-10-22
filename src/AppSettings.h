#pragma once
#ifndef APP_SETTINGS_H
#define APP_SETTINGS_H

#include <string>

struct AppSettings
{
	std::string name;

	int ScreenWidth;
	int ScreenHeight;
	bool FullScreen = false;

	std::string StartingSceneName;
};

#endif // !GAME_SETTINGS_H

