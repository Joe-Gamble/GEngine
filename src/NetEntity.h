#pragma once

#ifndef NET_ENTITY_H
#define NET_ENTITY_H
#endif // !NET_ENTITY_H

#include "Entity.h"

class NetEntity : public Entity
{
public:
	static NetEntity* Instantiate();
};

