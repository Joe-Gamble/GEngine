#pragma once

#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include <stdio.h>
#include "EntityManager.h"

namespace Engine
{
	class Game
	{
	public:
		Game();
		~Game();

		void Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

		void handleEvents();
		void Update(double& dt);
		void Render();

		void Destroy();

		inline bool isRunning() { return running; }

	private:
		bool running = false;;
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;

		EntityManager entityManager;
	};
}

#endif 