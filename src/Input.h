#pragma once

#ifndef INPUT_H
#define INPUT_H

#include "SDL.h"
#include "Vector.h"

using namespace GEngine::GUtility;

namespace GEngine
{
	class Input
	{
	public:
		static inline Input& Instance()
		{
			static Input instance;
			return instance;
		}

		void Listen();
		bool GetKeyDown(SDL_Scancode key);
		bool GetKeyUp(SDL_Scancode key);

		bool GetMouseButtonDown(int button);

		Vector2 GetMousePosition();

	private:
		Input();
		void KeyUp();
		void KeyDown();

		void ProcessMouseEvent(SDL_MouseButtonEvent& event);

		void MouseUp();
		void MouseDown();

		const Uint8* m_KeyStates;
		Uint32 m_mouseState;

		Vector2 m_mousePos;
	};
}

#endif