#pragma once

#ifndef INPUT_H
#define INPUT_H

#include "SDL.h"

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

private:
	Input();
	void KeyUp();
	void KeyDown();

	const Uint8* m_KeyStates;
};

#endif