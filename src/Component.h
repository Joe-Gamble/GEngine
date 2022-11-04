#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

#include <iostream>  
#include "ComponentType.h"
#include <memory>


namespace GEngine
{
	class Entity;

	struct Component
	{
		virtual ~Component() {}

		inline bool IsEnabled() { return m_isEnabled; }
		void SetEnabled(bool _enabled) { m_isEnabled = _enabled; }

		virtual const uint32_t GetMoldSize() { return 0; }

		virtual ComponentType GetType() { return ComponentType::TYPE_UNDEFINED; };
		virtual const void* Serialise() {
			std::cout << "ERROR: virtual const void* Serialise(): no implementation found. " << std::endl;
			return nullptr;
		}

		virtual void Deserialise() {};

		Entity* entity = nullptr;

		virtual void Init() {};
		virtual void Update(double& dt) {}
		virtual void Render() {}

		virtual void ApplyData(const void* data) {};
		
	private:
		bool m_isEnabled = true;
	};
}

#endif




