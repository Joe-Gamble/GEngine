#pragma once

#include "ComponentType.h"
#include <memory>

class Entity;

struct Component
{
	virtual const void* Serialise() = 0;
	virtual std::unique_ptr<Component> Deserialise() = 0;

	Entity* entity = nullptr;

	virtual void Init() {}
	virtual void Update(double& dt) {}
	virtual void Render() {}

	virtual void ApplyData(const void* data) {};
	virtual ~Component() {}

private:
	bool hasDataToBeSent = false;
	ComponentType componentType = ComponentType::TYPE_UNDEFINED;
};
