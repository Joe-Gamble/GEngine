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
	unsigned int now = SDL_GetTicks();
	unsigned int last = SDL_GetTicks();

	double timeBetweenFrames = 0;

	game->Init(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);

	while (game->isRunning())
	{
		now = SDL_GetTicks();
		timeBetweenFrames = now - last;

		if (timeBetweenFrames > 1000 / 60.0)
		{
			double delta = timeBetweenFrames / 1000.0f;
			// std::cout << "FPS: " << delta << std::endl;

			last = now;

			game->handleEvents();
			game->Update(delta);
			game->Render();
		}
	}

	game->Destroy();
	return 0;
}