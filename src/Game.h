#pragma once

#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include <stdio.h>
#include <string>

namespace GEngine
{
	class Game
	{
	public:
		Game();
		Game(const std::string& _name);
		~Game();

		void Init(int xpos, int ypos, int width, int height, bool fullscreen);

		inline void SetBackgroundToDefault(Uint8 r, Uint8 g, Uint8 b) { SDL_SetRenderDrawColor(renderer, r, g, b, 255); }

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
	};
}

#endif 