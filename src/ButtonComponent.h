#pragma once
#ifndef BUTTON_COMPONENT_H
#define BUTTON_COMPONENT_H

#include <vector>
#include "Component.h"
#include "SDL.h"

namespace GEngine
{
	enum ButtonState
	{
		DISABLED = -1,
		NORMAL = 0,
		HIGHLIGHTED = 1,
		PRESSED = 2
	};

	struct Button : public Component
	{
	public:
		Button(int x, int y, void (*callback)());
		void OnUpdate(double& dt) override;
		void Render() override;
	private:
		SDL_Rect m_Rect;
		bool m_Released;
		void (*m_Callback)();
	};
}

#endif // !BUTTON_COMPONENT_H

