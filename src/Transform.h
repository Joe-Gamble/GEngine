#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H
#endif

#include "Vector.h"
#include "Component.h"

using namespace GUtility;

struct Transform : public Component
{

public:
	Transform();
	Transform(const Transform& transform);

	void Update(double& dt) override;

	Vector2* const GetPosition() { return &position; }
	void SetPosition(const Vector2& _position) { position = _position; }
private:
	Vector2 position;
	Vector2 scale;
	//Quaternion Rotaion
};



