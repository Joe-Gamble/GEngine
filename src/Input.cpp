#include "Input.h"
#include "Application.h"

using namespace GEngine;

Input::Input()
{
	m_KeyStates = SDL_GetKeyboardState(&m_length);
	m_PrevKeyStates = new Uint8[m_length];
	memcpy(m_PrevKeyStates, m_KeyStates, m_length);

	m_MouseState = SDL_GetMouseState(nullptr, nullptr);
	m_PrevMouseState = m_MouseState;
}

Input::~Input()
{
	delete m_PrevKeyStates;
}

void Input::Listen()	
{
	memcpy(m_PrevKeyStates, m_KeyStates, m_length);
	m_PrevMouseState = m_MouseState;
	
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
			case SDL_KEYDOWN:
				break;
			default:
				break;
		}
	}
}

bool Input::GetKey(SDL_Scancode key)
{
	return m_KeyStates[key] && m_PrevKeyStates[key];
}

bool Input::GetKeyDown(SDL_Scancode key)
{
	return m_KeyStates[key] && !m_PrevKeyStates[key];
}

bool Input::GetKeyUp(SDL_Scancode key)
{
	return !m_KeyStates[key] && m_PrevKeyStates[key];
}

bool Input::GetMouseButton(int button)
{
	return m_MouseState & SDL_BUTTON(button);
}

bool Input::GetMouseButtonDown(int button)
{
	return (m_MouseState & SDL_BUTTON(button) && !(m_PrevMouseState & SDL_BUTTON(button)));
}

bool Input::GetMouseButtonUp(int button)
{
	return (!(m_MouseState & SDL_BUTTON(button)) && m_PrevMouseState & SDL_BUTTON(button));
}

Vector2 Input::GetMousePosition()
{
	if (SDL_GetMouseFocus() != nullptr)
	{
		return m_MousePos;
	}
	return Vector2();
}

void Input::ProcessMouseEvent(SDL_MouseButtonEvent& mouseButtonEvent)
{
	int x, y;
	m_MouseState = SDL_GetMouseState(&x, &y);

	m_MousePos.SetX(x);
	m_MousePos.SetY(y);
}

void Input::MouseUp()
{
}

void Input::MouseDown()
{
}
