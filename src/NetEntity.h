#pragma once

#ifndef NET_ENTITY_H
#define NET_ENTITY_H

#include "Entity.h"
#include "NetComponent.h"

/// <summary>
/// Net Entity
/// Intended to be a networked object that can be updated between client instances
/// Mainly designed for player controlled objects, or maybe UI features like score?
/// </summary>

namespace GEngine
{
	namespace Networking
	{
		class NetEntity : public Entity
		{
		public:
			NetEntity();
			NetEntity(short& netID, std::shared_ptr<Scene> scene);
			static NetEntity* Instantiate(short& netID, std::shared_ptr<Scene> scene);
			static NetEntity* Instantiate(std::shared_ptr<Scene> scene);

			template<typename T, typename... TArgs>
			T* AddComponent(TArgs&&... mArgs)
			{
				T* component = TryGetComponent<T>();
				if (component == nullptr)
				{
					Entity* e = (Entity*)this;
					T* t = &e->AddComponent<T>(mArgs...);

					if (std::is_base_of<NetComponent, T>())
					{
						NetComponent* netComponent = static_cast<NetComponent*>(t);
						std::shared_ptr<NetComponent> ptr{ std::move(netComponent) };
						netComponents.emplace_back(ptr);
					}

					return t;
				}
				else
					return component;
			}

			const inline short GetID() { return netID; }
			inline std::vector <std::shared_ptr<NetComponent>>* GetNetComponents() { return &netComponents; }

		private:
			const short netID = -1;
			std::vector<std::shared_ptr<NetComponent>> netComponents;
		};
	}
}
#endif // !NET_ENTITY_H




