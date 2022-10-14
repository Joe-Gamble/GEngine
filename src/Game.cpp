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
			SDL_SetRenderDrawColor(renderer, 32 , 90, 48, 255);
		}

		running = true;
	}

	// if game host machine

	EventDriver::Instance().RegisterCallback(std::bind(&Game::SetBackgroundToDefault, this, 32, 90, 48), Event::NETWORKING_SESSION_ENDED);

	EventDriver::Instance().RegisterCallback(std::bind(&Game::SetBackgroundToDefault, this, 90, 32, 48), Event::NETWORKING_SERVER_READY);

	EventDriver::Instance().RegisterCallback(std::bind(&Game::SetBackgroundToDefault, this, 48, 32, 90), Event::NETWORKING_CLIENT_CONNECT_SUCCESSFUL);

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

	short num = 1;
	

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
					NetworkManager::Instance().MakeServer(ServerType::DEDICATED);
					break;
				}

				case SDLK_w:
				{
					NetworkManager::Instance().JoinServer("192.168.0.203");
					break;
				}
				case SDLK_e:
				{
					NetworkManager::Instance().EndSession();
					break;
				}
				case SDLK_r:
				{
					if (NetworkManager::Instance().IsServer())
					{
						/*NetEntity& netEntity = NetworkManager::Instance().GetServer().MakeEntity();
						entityManager->AddNetEntity(&netEntity);

						netEntity.AddComponent<NetTransform>();
						NetTransform* transform = netEntity.TryGetComponent<NetTransform>();

						transform->SetPosition({ 1, 0 });

						 std::shared_ptr<GamePacket> entityPacket = std::make_shared<GamePacket>(PacketType::PT_ENTITY_CHANGE);
						 *entityPacket << netEntity;

						 std::cout << "Entity Packaged" << std::endl;

						 NetworkManager::Instance().GetServer().SendPacket(entityPacket);
						 std::cout << "Entity Sent" << std::endl;*/

						 // std::shared_ptr<GamePacket> packet = NetEntity::MakeEntityPacket(&entity);
					}
					
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