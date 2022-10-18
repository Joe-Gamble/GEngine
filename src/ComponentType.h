#pragma once

#ifndef NET_COMPONENT_TYPE
#define NET_COMPONENT_TYPE

#include <stdint.h>

namespace GEngine
{
	enum class ComponentType : uint16_t // 2 bytes per type identifier
	{
		TYPE_UNDEFINED,

		TYPE_TRANSFORM,
		TYPE_TEXT,
		TYPE_RIGIDBODY,
		TYPE_BACKGROUND_CHANGER,

		TYPE_NET_TRANSFORM,

		MAX_DEFINE
	};

}

#endif // !NETCOMPONENTTYPE


