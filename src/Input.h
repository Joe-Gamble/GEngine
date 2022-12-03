#pragma once

#ifndef INPUT_H
#define INPUT_H

#include "SDL.h"
#include "Vector.h"
#include <vector>

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

		bool GetKey(SDL_Scancode key);
		bool GetKeyDown(SDL_Scancode key);
		bool GetKeyUp(SDL_Scancode key);

		bool GetMouseButton(int button);
		bool GetMouseButtonDown(int button);
		bool GetMouseButtonUp(int button);

		Vector2 GetMousePosition();

	private:
		Input();
		~Input();

		void ProcessMouseEvent(SDL_MouseButtonEvent& event);

		void MouseUp();
		void MouseDown();

		const Uint8* m_KeyStates;
		Uint8* m_PrevKeyStates;

		Uint32 m_MouseState;
		Uint32 m_PrevMouseState;

		Vector2 m_MousePos;

		int m_KeysLength = 0;
	};
}

#endif