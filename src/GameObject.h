#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "SDL.h"
#include "SDL_image.h"
#include <stdio.h>
#include <string>
#include "Transform.h"
#include "Entity.h"

class GameObject : Entity
{
public:
	static GameObject* Instantiate();
	static GameObject* Instantiate(const GameObject&);

private:

	GameObject();
	GameObject(const std::string & name);
	GameObject(const GameObject&);

	std::string name = "GameObject";
	Transform* transform = nullptr;
};

#endif

