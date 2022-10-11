#pragma once

#ifndef NET_COMPONENT_TYPE
#define NET_COMPONENT_TYPE
#endif // !NETCOMPONENTTYPE

#include <stdint.h>

enum class ComponentType : uint16_t // 2 bytes per type identifier
{
	TYPE_UNDEFINED,
	TYPE_TRANSFORM,
	TYPE_TEXT,
	TYPE_RIGIDBODY,
	TYPE_BACKGROUND_CHANGER
};

