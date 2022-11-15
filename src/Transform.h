#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Vector.h"
#include "Component.h"

namespace GEngine
{
	using namespace GUtility;

	struct Transform : public Component
	{

	public:
		Transform();
		Transform(const Transform& transform);

		void OnUpdate(double& dt) override;

		inline const Vector2* GetPosition() { return &position; }
		inline void SetPosition(const Vector2& _position) { position = _position; }
	private:
		Vector2 position;
		Vector2 scale;
		//Quaternion Rotaion
	};
}

#endif



