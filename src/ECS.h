#pragma once

#include <array>
#include <bitset>
#include <iostream>


class Entity;
struct Component;

constexpr std::size_t MAX_COMPONENTS = 10;

using ComponentID = std::size_t;

namespace Internal
{
	inline ComponentID getUniqueComponentID() {
		static ComponentID lastID = 0u;
		return lastID++;
	}
}

template<typename T> inline ComponentID getComponentTypeID() noexcept {
	static_assert (std::is_base_of<Component, T> ::value, "Type not based on component");

	static ComponentID typeID{ Internal::getUniqueComponentID() };
	return typeID;
}

constexpr std::size_t MAX_ENTITIES = 5000;



	