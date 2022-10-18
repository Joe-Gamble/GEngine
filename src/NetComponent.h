#pragma once
#ifndef NET_COMPONENT
#define NET_COMPONENT

#include "Component.h"

namespace GEngine
{
	struct NetComponent : Component
	{
	public:
		inline bool HasPendingData() { return hasDataToSend; }
	private:
		bool hasDataToSend = false;
	};
}

#endif

