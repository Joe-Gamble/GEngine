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

Vector2 Vector2::operator=(const Vector2& rhs)
{
	Vector2 vector2 = Vector2(rhs);
	return vector2;
}

bool Vector2::operator==(const Vector2& rhs)
{
	return (x == rhs.x && y == rhs.y);
}

Vector2 Vector2::operator+(Vector2& rhs)
{
	Vector2 vector2 = Vector2();

	vector2.x = x + rhs.x;
	vector2.y = y + rhs.y;

	return vector2;
}

Vector2 Vector2::operator-(Vector2& rhs)
{
	Vector2 vector2 = Vector2();

	vector2.x = x - rhs.x;
	vector2.y = y - rhs.y;

	return vector2;
}

Vector2 Vector2::operator*(Vector2& rhs)
{
	Vector2 vector2 = Vector2();

	vector2.x = x * rhs.x;
	vector2.y = y * rhs.y;

	return vector2;
}

Vector2 Vector2::operator/(Vector2& rhs)
{
	Vector2 vector2 = Vector2();

	vector2.x = x / rhs.x;
	vector2.y = y / rhs.y;

	return vector2;
}

Vector2 Vector2::operator/(float& rhs)
{
	Vector2 vector2 = Vector2();

	vector2.x = x / rhs;
	vector2.y = y / rhs;

	return vector2;
}

Vector2 Vector2::operator/=(Vector2& rhs)
{
	Vector2 v2 = *this / rhs;
	*this = v2;
	return *this;
}

Vector2 Vector2::operator/=(float& rhs)
{
	Vector2 v2 = *this / rhs;
	*this = v2;
	return *this;
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