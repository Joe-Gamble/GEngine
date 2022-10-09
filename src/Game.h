#pragma once

#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include <stdio.h>
#include "EntityManager.h"
#include "NetworkManager.h"

namespace GEngine
{
	class Game
	{
	public:
		Game();
		Game(const std::string& _name);
		~Game();

		void Init(int xpos, int ypos, int width, int height, bool fullscreen);

		void handleEvents();
		void Update(double& dt);
		void Render();

		void Destroy();

		inline bool isRunning() { return running; }

	private:
		std::string name;
		bool running = false;;
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;

		std::unique_ptr<EntityManager> entityManager = std::make_unique<EntityManager>();

	};
}

#endif 