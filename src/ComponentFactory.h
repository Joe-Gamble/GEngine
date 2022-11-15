#pragma once
#ifndef COMPONENT_FACTORY_H
#define COMPONENT_FACTORY_H

#include <string>
#include <unordered_map >
#include <functional>
#include "Component.h"
#include <memory>
#include "ECS.h"

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

		inline bool HasComponent(ComponentID type) { return components.count(type) > 0; }

		inline void AddComponent(Component* component)
		{
			std::unique_ptr<Component> uPtr{ component };

			ComponentID id = getComponentTypeID<std::remove_reference_t<decltype(*component)>>();
			components.emplace(id, std::move(uPtr));
		}

		inline ComponentID GetComponentID(Component* component )
		{
			return getComponentTypeID<std::remove_reference_t<decltype(*component)>>();
		}

		inline std::unique_ptr<Component>* GetComponent(ComponentID type)
		{
			auto it = components.find(type);
			if (it != components.end())
			{
				return &it->second;
			}

			return nullptr;
		}

	private:
		std::unordered_map<ComponentID, std::unique_ptr<Component>> components;
	};
}
#endif

