#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

#include "ComponentType.h"
#include <memory>

namespace GEngine
{
	class Entity;

	struct Component
	{
		virtual ComponentType GetType() = 0;
		virtual const void* Serialise() = 0;
		virtual const uint32_t GetMoldSize() = 0;

		virtual void Deserialise() = 0;

		Entity* entity = nullptr;

		virtual void Init() {}
		virtual void Update(double& dt) {}
		virtual void Render() {}

		virtual void ApplyData(const void* data) {};
		virtual ~Component() {}

	private:
		bool hasDataToBeSent = false;
	};
}

#endif




