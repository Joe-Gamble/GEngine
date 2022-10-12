#pragma once

#ifndef NET_ENTITY_H
#define NET_ENTITY_H
#endif // !NET_ENTITY_H

#include "Entity.h"
#include "GamePacket.h"

namespace GEngine
{
	namespace Networking
	{
		class NetEntity : public Entity
		{
		public:
			NetEntity(short& netID);
			static NetEntity* Instantiate(short& netID);
			void AddComponentDataToPacket(std::shared_ptr<GamePacket>& entityPacket);

			static std::shared_ptr<GamePacket> MakeEntityPacket(NetEntity* entity);

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

			 //std::shared_ptr<Component> GetNetComponents;

		private:
			const short netID;
			std::vector<std::shared_ptr<NetComponent>> netComponents;
		};
	}
}



