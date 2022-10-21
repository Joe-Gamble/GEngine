#include "Game.h"

#include "EventDriver.h"
#include <iostream>
#include "SceneManager.h"
#include "NetworkManager.h"

using namespace GEngine;
using namespace GEngine::Callbacks;

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

	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS) == 0)
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
			SDL_SetRenderDrawColor(renderer, 32 , 90, 48, 255);
		}

		running = true;
	}

	std::string name = "MainMenu";

	// SceneManager::Instance().LoadScene(name);

	// if game host machine

	EventDriver::Instance().RegisterCallback(std::bind(&Game::SetBackgroundToDefault, this, 32, 90, 48), Event::NETWORKING_SESSION_ENDED);

	EventDriver::Instance().RegisterCallback(std::bind(&Game::SetBackgroundToDefault, this, 90, 32, 48), Event::NETWORKING_SERVER_READY);

	EventDriver::Instance().RegisterCallback(std::bind(&Game::SetBackgroundToDefault, this, 48, 32, 90), Event::NETWORKING_CLIENT_CONNECT_SUCCESSFUL);


	// if joining session
	// 
	// work 192.168.0.203
	// home 192.168.0.23

	short num = 1;
	

	//SDL_Surface* tmpSurface = IMG_Load("Assets/test.png");
	//testTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);

	//SDL_FreeSurface(tmpSurface);
}

//GEngine-- > Library
//
//GameModeSettings
//- MaxPlayers
//- IsDedicated
//
//GameSettings
//- Starting Scene name ?
//-isMultiplayer
	
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
			//Check the SDLKey values and move change the coords 
			switch (event.key.keysym.sym) 
			{
				// These calls are tests, 
				case SDLK_q:
				{
					NetworkManager::Instance().MakeServer(ServerType::DEDICATED);
					break;
				}

				case SDLK_w:
				{
					NetworkManager::Instance().JoinServer("192.168.0.23");
					break;
				}
				case SDLK_e:
				{
					NetworkManager::Instance().EndSession();
					break;
				}
				case SDLK_r:
				{
					break;
				}

				default:
					break;
			}
			break;
		}
		default:
		{
			break;
		}
		
	}
}

void Game::Update(double& dt)
{
	// Scene update here
}

void Game::Render()
{
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	// This needs to be called from GameManager as part of the Game deconstruction
	NetworkManager::Instance().ShutDown();
	
	std::cout << "Game Destroyed" << std::endl;
}