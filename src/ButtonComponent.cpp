#include "ButtonComponent.h"
#include "Input.h"

using namespace GEngine;

GEngine::Button::Button(int x, int y, void(*callback)())
{
}

void GEngine::Button::Update(double& dt)
{
	if (IsEnabled())
	{
		Vector2 mousePos = Input::Instance().GetMousePosition();
		SDL_Point point = { mousePos.X(), mousePos.Y() };

		if (SDL_PointInRect(&point, &m_Rect))
		{

		}
	}
}

void GEngine::Button::Render()
{
}
