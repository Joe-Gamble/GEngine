#pragma once

#ifndef NETCOMPONENT
#define NETCOMPONENT
#endif // !NETCOMPONENT

#include "Component.h"
#include "NetComponentType.h"

struct NetComponent : Component
{
public:
	virtual bool SendData() = 0;
	virtual void ApplyData(NetComponent* transform) = 0;
private:
	bool hasDataToBeSent = false;
	NetComponentType componentType = TYPE_UNDEFINED;
};