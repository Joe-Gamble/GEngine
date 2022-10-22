#include "Input.h"
#include "Application.h"

using namespace GEngine;

Input::Input()
{
	m_KeyStates = SDL_GetKeyboardState(nullptr);
}

void Input::Listen()
{
	m_KeyStates = SDL_GetKeyboardState(nullptr);

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT: Application::Quit(); break;
			case SDL_KEYDOWN: KeyDown(); break;
			case SDL_KEYUP: KeyUp(); break;

			default:
				break;
		}
	}
}

bool Input::GetKeyDown(SDL_Scancode key)
{
	return (m_KeyStates[key] == 1);
}

void Input::KeyUp()
{
	m_KeyStates = SDL_GetKeyboardState(nullptr);
}

void Input::KeyDown()
{
	m_KeyStates = SDL_GetKeyboardState(nullptr);
}
