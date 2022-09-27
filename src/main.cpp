#include <SDL.h>
#include <stdio.h>
#include "Game.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

using namespace GEngine;

Game* game = new Game("Test Game");

int main(int argc, char* args[])
{
	Uint64 now = SDL_GetPerformanceCounter();
	Uint64 last = 0;

	double deltaTime = 0;

	game->Init(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);

	while (game->isRunning())
	{
		last = now;
		now = SDL_GetPerformanceCounter();

		deltaTime = ((now - last) * 1000 / (double)SDL_GetPerformanceFrequency());

		game->handleEvents();
		game->Update(deltaTime);
		game->Render();
	}

	game->Destroy();

	return 0;
}