#pragma once

#ifndef NETCOMPONENT
#define NETCOMPONENT
#endif // !NETCOMPONENT

#include "Component.h"
#include "NetComponentType.h"
#include <memory>
#include <map>
#include <IncludeMe.h>

struct NetComponent : Component
{
public:
	virtual bool SendData() = 0;
	virtual void Clear() = 0;

	virtual void ApplyData(NetComponent* transform) = 0;
protected:
	static const std::map<PacketType, NetComponent*> PacketPrototypes; // will need this eventually
private:
	bool hasDataToBeSent = false;
	NetComponentType componentType = TYPE_UNDEFINED;
};