#pragma once
#ifndef SCENE_H
#define SCENE_H
#endif // !SCENE_H

#include <vector>
#include "Component.h"

class Scene
{
public:
	Scene();
	~Scene();
private:
	std::vector<Entity*> components;
};