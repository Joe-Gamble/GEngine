#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "GameSettings.h"
#include "SDL.h"

namespace GEngine
{
	static class Application
	{
	public:
		explicit Application(GameSettings& settings);

		static void Run();
		static void Quit();

	private:
		static void Init(GameSettings& settings);

		static void HandleEvents();
		static void Tick();
		static void Draw();

		inline static void SetBackgroundToDefault(Uint8 r, Uint8 g, Uint8 b) { SDL_SetRenderDrawColor(renderer, r, g, b, 255); }

	private:
		inline static bool isRunning;
		inline static std::string name;

		inline static SDL_Window* window;
		inline static SDL_Renderer* renderer;
	};
}

#endif // !APPLICATION_H

