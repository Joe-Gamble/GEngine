#include "Input.h"
#include "Application.h"

using namespace GEngine;

Input::Input()
{
	m_KeyStates = SDL_GetKeyboardState(nullptr);
}

void Input::Listen()
{
	SDL_PumpEvents();

	m_KeyStates = SDL_GetKeyboardState(nullptr);

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT: Application::Quit(); break;

			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEWHEEL:
			case SDL_MOUSEMOTION:
				ProcessMouseEvent(event.button);
				break;

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

bool GEngine::Input::GetMouseButtonDown(int button)
{
	if (m_mouseState)
	{

	}

	return 1;
}

Vector2 GEngine::Input::GetMousePosition()
{
	if (SDL_GetMouseFocus() != nullptr)
	{
		return m_mousePos;
	}
	return Vector2();
}

void Input::KeyUp()
{
	m_KeyStates = SDL_GetKeyboardState(nullptr);
}

void Input::KeyDown()
{
	m_KeyStates = SDL_GetKeyboardState(nullptr);
}

void GEngine::Input::ProcessMouseEvent(SDL_MouseButtonEvent& mouseButtonEvent)
{
	int x, y;
	m_mouseState = SDL_GetMouseState(&x, &y);

	m_mousePos.SetX(x);
	m_mousePos.SetY(y);
}

void GEngine::Input::MouseUp()
{
}

void GEngine::Input::MouseDown()
{
}
