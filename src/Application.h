#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "AppSettings.h"
#include "SDL.h"
#include "GameManager.h"
#include "SceneManager.h"

namespace GEngine
{
	static class Application
	{
	public:
		explicit Application(AppSettings& settings);

		static void Run();
		static void Quit();

		inline static SDL_Renderer* GetRenderer() { return renderer; }

	private:
		static void Init(AppSettings& settings);

		static void HandleEvents();
		static void Tick(double& dt);
		static void Draw();

		inline static void SetBackgroundToDefault(Uint8 r, Uint8 g, Uint8 b) { SDL_SetRenderDrawColor(renderer, r, g, b, 255); }

	private:
		inline static bool isRunning;
		inline static std::string name;

		static GameManager m_gameManager;

		inline static SDL_Window* window;
		inline static SDL_Renderer* renderer;
	};
}

#endif // !APPLICATION_H

