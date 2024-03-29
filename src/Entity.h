#pragma once
#ifndef ENTITY_H
#define ENTITY_H


#include <array>
#include <bitset>
#include <memory>
#include <vector>
#include <assert.h>
#include "Component.h"
#include "ECS.h"

namespace GEngine
{
	class Scene;

	using ComponentBitSet = std::bitset<MAX_COMPONENTS>;
	using ComponentArray = std::array<Component*, MAX_COMPONENTS>;

	class Entity
	{
	private:
		bool alive = true;
		std::vector<std::unique_ptr<Component>> components;
		Scene* scene = nullptr;

		ComponentArray componentArray = {};
		ComponentBitSet componentBitSet = {};

	protected:
		Entity(Scene* _scene) : scene(_scene) {}

	public:

		static Entity* Instantiate(std::shared_ptr<Scene> scene);
		static Entity* Instantiate(Scene* scene);

		void Update(double& dt);
		void Render();

		Scene* GetScene() { return scene; };

		bool IsAlive() const;
		void Destroy();

		template<typename T>
		bool HasComponent() const
		{
			return componentBitSet[getComponentTypeID<T>()];
		}

		template<typename T, typename... TArgs>
		T& AddComponent(TArgs&&... mArgs)
		{
			assert(!HasComponent<T>());

			T* c(new T(std::forward<TArgs>(mArgs)...));
			c->entity = this;

			std::unique_ptr<Component> uPtr{ c };
			components.emplace_back(std::move(uPtr));

			componentArray[getComponentTypeID<T>()] = c;
			componentBitSet[getComponentTypeID<T>()] = true;

			c->Init();

			return *c;
		}

		template<typename T>
		T& GetComponent() const
		{
			assert(HasComponent<T>());

			auto ptr = componentArray[getComponentTypeID<T>()];
			return *static_cast<T*>(ptr);
		}

		template<typename T>
		T* TryGetComponent() const
		{
			if (!HasComponent<T>())
				return nullptr;

			auto ptr = componentArray[getComponentTypeID<T>()];
			return static_cast<T*>(ptr);
		}

		std::vector<std::unique_ptr<Component>>* GetComponents();
	};
}

#endif
