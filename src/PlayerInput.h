#pragma once
#ifndef INPUT_COMPONENT_H
#define INPUT_COMPONENT_H

#include "Component.h"
#include "Entity.h"

namespace GEngine
{
	struct PlayerInput : public Component
	{
		void Update(double& dt) override;
	};
}

#endif

