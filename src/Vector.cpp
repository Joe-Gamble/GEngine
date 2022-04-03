#include "Vector.h"
#include <cmath>


using namespace GUtility;

Vector2::Vector2(const float& _x, const float& _y)
{
	x = _x;
	y = _y;
}

Vector2::Vector2(const Vector2& rhs)
{
	x = rhs.x;
	y = rhs.y;
}

Vector2 Vector2::Zero()
{
	return Vector2(0, 0);
}

float Vector2::Distance(const Vector2& v1, const Vector2& v2)
{
	float distance = sqrtf(powf((v2.x - v1.x), 2) + (powf((v2.y - v1.y), 2)));
	return distance;
}

Vector2 Vector2::Midpoint(const Vector2& v1, const Vector2& v2)
{
	float x = (v1.x + v2.x) * 0.5f;
	float y = (v1.y + v2.y) * 0.5f;

	return Vector2(x, y);
}

float Vector2::Magnitude()
{
	return sqrtf(x * x + y * y);
}

Vector2 Vector2::Normalise()
{
	float mag = Magnitude();

	if (mag > 0)
	{
		return *this / mag;
	}

	return Vector2::Zero();
}