#pragma once

#ifndef NET_TRANSFORM
#define NET_TRANSFORM
#endif // !NET_TRANSFORM

#include "NetComponent.h"
#include "Transform.h"

struct NetTransform : NetComponent
{
public:
	void Update(double& dt) override;
	void ApplyData(NetComponent* transform) override;
	bool SendData();
private:


public:
	NetTransform();
	NetTransform(const NetTransform& transform);

	Vector2* const GetPosition() { return &position; }
	void SetPosition(const Vector2& _position) { position = _position; }
private:
	Vector2 position;
	Vector2 scale;
	//Quaternion Rotaion

};

