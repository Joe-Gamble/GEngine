#include "Transform.h"
#include <iostream>

Transform::Transform()
{
	position = Vector2();
	scale = Vector2(1, 1);
}

Transform::Transform(const Transform& transform)
{
	position = transform.position;
	scale = transform.scale;
}

void Transform::Update(double& dt)
{
	std::cout << position.X() << std::endl;
}



