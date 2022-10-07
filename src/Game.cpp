#include "Game.h"
#include <iostream>
#include "Transform.h"
#include "NetTransform.h"
#include "NetworkManager.h"
#include "EventDriver.h";

using namespace GEngine;
using namespace Networking;
using namespace GEngine::Callbacks;

SDL_Texture* testTexture;

Game::Game()
{
	name = "My Game Window!";
}

GEngine::Game::Game(const std::string& _name)
{
	name = _name;
}

Game::~Game()
{}

void Game::Init(int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;

	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems Initialised!..." << std::endl;

		SDL_Window* window = SDL_CreateWindow(name.c_str(), xpos, ypos, width, height, flags);
		if (window)
		{
			std::cout << "Window created" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
		if (renderer)
		{
			std::cout << "Renderer created" << std::endl;
			SDL_SetRenderDrawColor(renderer, 15, 18, 186, 255);
		}

		running = true;
	}

	// if game host machine

	EventDriver::Instance().RegisterCallback([]() 
		{ 
			std::cout << "Client Connected" << std::endl; 
		}, 
		Event::NETWORKING_CLIENT_CONNECT_SUCCESSFUL
	);

	EventDriver::Instance().RegisterCallback([]()
		{
			std::cout << "Client couldn't connect." << std::endl;
		},
		Event::NETWORKING_CLIENT_CONNECT_UNSUCCESSFUL
			);

	// if joining session
	// 
	// work 192.168.0.203
	// home 192.168.0.23

	Entity& entity = entityManager->AddEntity();
	entity.AddComponent<NetTransform>();
	entity.GetComponent<NetTransform>().SetPosition({ 1, 0 });

	//SDL_Surface* tmpSurface = IMG_Load("Assets/test.png");
	//testTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);

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
			EventDriver::Instance().CallEvent(Event::GAME_QUIT);
			running = false;
			break;
		}
		case SDL_KEYDOWN:
		{
			/* Check the SDLKey values and move change the coords */
			switch (event.key.keysym.sym) 
			{
				case SDLK_q:
				{
					NetworkManager::Instance().MakeServer();
					break;
				}

				case SDLK_w:
				{
					NetworkManager::Instance().JoinServer("192.168.0.23");
					break;
				}
				default:
					break;
			}
		}
		default:
		{
			break;
		}
	}
}

void Game::Update(double& dt)
{
	entityManager->Update(dt);
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

	NetworkManager::Instance().ShutDown();
	
	std::cout << "Game Destroyed" << std::endl;
}