#pragma once

#ifndef NET_ENTITY_H
#define NET_ENTITY_H

#include "Entity.h"
#include "NetComponent.h"

namespace GEngine
{
	namespace Networking
	{
		class NetEntity : public Entity
		{
		public:
			NetEntity(short& netID, Scene* scene);
			static NetEntity* Instantiate(short& netID, Scene* scene);

			template<typename T, typename... TArgs>
			T* AddComponent(TArgs&&... mArgs)
			{
				T* component = TryGetComponent<T>();
				if (component == nullptr)
				{
					Entity* e = (Entity*)this;
					T* t = &e->AddComponent<T, mArgs...>();
					netComponents.emplace_back(t);

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




