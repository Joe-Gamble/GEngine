#pragma once

class Entity;

struct Component
{
	Entity* entity = nullptr;

	virtual void Init() {}
	virtual void Update(double& dt) {}
	virtual void Render() {}

	virtual ~Component() {}
};
