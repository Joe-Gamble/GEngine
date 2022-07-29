#include "Game.h"
#include <iostream>
#include "Transform.h"
#include "NetTransform.h"

using namespace Engine;

SDL_Texture* testTexture;

Game::Game()
{
	
}

Game::~Game()
{}

void Game::Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;

	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems Initialised!..." << std::endl;

		SDL_Window* window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window)
		{
			std::cout << "Window created" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			std::cout << "Renderer created" << std::endl;
			SDL_SetRenderDrawColor(renderer, 15, 18, 186, 255);
		}

		running = true;
	}

	Entity& entity = entityManager.AddEntity();
	entity.AddComponent<NetTransform>();
	entity.GetComponent<NetTransform>().SetPosition({ 1, 0 });

	// SDL_Surface* tmpSurface = IMG_Load("Assets/test.png");
	// testTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);

	//SDL_FreeSurface(tmpSurface);
}
	
void Game::handleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
		case SDL_QUIT:
		{
			running = false;
			break;
		}
		default:
			break;
	}
}

void Game::Update(double& dt)
{
	entityManager.Update(dt);
}

void Game::Render()
{
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, testTexture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	std::cout << "Game Destroyed" << std::endl;
}