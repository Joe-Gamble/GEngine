#pragma once

#include <array>
#include <bitset>
#include <memory>
#include <vector>
#include <assert.h>
#include "Component.h"
#include "ECS.h"


using ComponentBitSet = std::bitset<MAX_COMPONENTS>;
using ComponentArray = std::array<Component*, MAX_COMPONENTS>;

class Scene;

class Entity
{
private:
	bool alive = true;
	std::vector<std::unique_ptr<Component>> components;
	Scene* scene = nullptr;

	ComponentArray componentArray;
	ComponentBitSet componentBitSet;

public:

	static Entity* Instantiate();
	static Entity* Instantiate(const Entity&);

	void Update(double& dt);
	void Render();

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
