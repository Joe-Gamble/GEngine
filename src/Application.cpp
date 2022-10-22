#include "Application.h"
#include <iostream>

#include "Input.h"
#include "EventDriver.h"
#include "NetworkManager.h"

using namespace GEngine;
using namespace GEngine::Networking;
using namespace GEngine::Callbacks;

Application::Application(AppSettings& settings)
{
	Init(settings);
}

void Application::Init(AppSettings& settings)
{
	int flags = 0;
	name = settings.name;

	if (settings.FullScreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS) == 0)
	{
		std::cout << "Subsystems Initialised!..." << std::endl;

		window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, settings.ScreenWidth, settings.ScreenHeight, flags);
		if (window)
		{
			std::cout << "Window created" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
		if (renderer)
		{
			std::cout << "Renderer created" << std::endl;
			SDL_SetRenderDrawColor(renderer, 32, 90, 48, 255);
		}

		EventDriver::Instance().RegisterCallback(std::bind((void(*)(Uint8, Uint8, Uint8)) &Application::SetBackgroundToDefault, 32, 90, 48), Event::NETWORKING_SESSION_ENDED);

		EventDriver::Instance().RegisterCallback(std::bind((void(*)(Uint8, Uint8, Uint8)) & Application::SetBackgroundToDefault, 90, 32, 48), Event::NETWORKING_SERVER_READY);

		EventDriver::Instance().RegisterCallback(std::bind((void(*)(Uint8, Uint8, Uint8)) & Application::SetBackgroundToDefault, 48, 32, 90), Event::NETWORKING_CLIENT_CONNECT_SUCCESSFUL);

		isRunning = true;
	}
}

void Application::Run()
{
	unsigned int now = SDL_GetTicks();
	unsigned int last = SDL_GetTicks();

	double timeBetweenFrames = 0;

	while (isRunning)
	{
		now = SDL_GetTicks();
		timeBetweenFrames = now - last;

		if (timeBetweenFrames > 1000 / 60.0)
		{
			double delta = timeBetweenFrames / 1000.0f;
			// std::cout << "FPS: " << delta << std::endl;

			HandleEvents();

			Tick();

			Draw();

			last = now;
		}
	}
}

void Application::HandleEvents()
{
	Input::Instance().Listen();
}

void Application::Tick()
{
	if (Input::Instance().GetKeyDown(SDL_SCANCODE_Q))
	{
		NetworkManager::Instance().MakeServer(ServerType::DEDICATED);
	}
	else if (Input::Instance().GetKeyDown(SDL_SCANCODE_W))
	{
		NetworkManager::Instance().JoinServer("192.168.0.203");
	}
	else if (Input::Instance().GetKeyDown(SDL_SCANCODE_E))
	{
		NetworkManager::Instance().EndSession();
	}
}

void Application::Draw()
{
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

void Application::Quit()
{
	isRunning = false;

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	// This needs to be called from GameManager as part of the Game deconstruction
	NetworkManager::Instance().ShutDown();

	std::cout << "Game Destroyed" << std::endl;
}
