#include <SDL.h>
#include <stdio.h>
#include "Game.h"
#include "Application.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

using namespace GEngine;

int main(int argc, char* args[])
{
	GameSettings settings =
	{
		"Test Game",
		640,
		480,
		false,
		"MainMenu"
	};

	GEngine::Application TestApp = Application(settings);
	TestApp.Run();

	return 0;
}