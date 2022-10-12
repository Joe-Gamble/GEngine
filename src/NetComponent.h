#pragma once
#ifndef NET_COMPONENT
#define NET_COMPONENT
#endif

#include "Component.h"

struct NetComponent : Component
{
public:
	inline bool HasPendingData() { return hasDataToSend; }
private:
	bool hasDataToSend = false;
};