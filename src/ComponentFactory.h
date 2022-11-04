#pragma once
#ifndef COMPONENT_FACTORY_H
#define COMPONENT_FACTORY_H

#include <string>
#include <unordered_map >
#include <functional>
#include "Component.h"
#include <memory>

namespace GEngine
{
	static class ComponentFactory
	{
	public:
		static ComponentFactory& Instance()
		{
			static ComponentFactory factoy;
			return factoy;
		}

		inline bool HasComponent(ComponentType type) { return components.count(type) > 0; }

		inline void AddComponent(ComponentType type, Component* component)
		{
			std::unique_ptr<Component> uPtr{ component };
			components.emplace(type, std::move(uPtr));
		}

		inline std::unique_ptr<Component>* GetComponent(ComponentType type)
		{
			auto it = components.find(type);
			if (it != components.end())
			{
				return &it->second;
			}

			return nullptr;
		}

	private:
		std::unordered_map<ComponentType, std::unique_ptr<Component>> components;
	};
}
#endif

