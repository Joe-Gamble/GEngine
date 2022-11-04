#pragma once
#ifndef NET_COMPONENT
#define NET_COMPONENT

#include "Component.h"

namespace GEngine
{
	struct NetComponent : Component
	{
	public:
		inline bool HasPendingData() { return m_hasDataToBeSent; }
	private:
		bool m_hasDataToBeSent = false;
	};
}

#endif

