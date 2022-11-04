#pragma once
#ifndef APP_SETTINGS_H
#define APP_SETTINGS_H

#include <string>
#include "SceneBundle.h"

namespace GEngine
{
	struct AppSettings
	{
		AppSettings(std::string _name, SceneBundle* bundle, int width, int height, bool fullscreen)
		{
			sceneBundle = bundle;
			name = _name;

			FullScreen = fullscreen;
			ScreenWidth = width;
			ScreenHeight = height;
		}

		SceneBundle* sceneBundle;
		std::string name;

		int ScreenWidth;
		int ScreenHeight;
		bool FullScreen = false;
	};
}

#endif // !GAME_SETTINGS_H

