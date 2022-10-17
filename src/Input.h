#pragma once
#ifndef INPUT_COMPONENT_H
#define INPUT_COMPONENT_H
#endif

#include "Component.h"
#include "Entity.h"

struct Input : public Component
{
	void Update(double& dt) override;
};
