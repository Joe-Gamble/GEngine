#pragma once
#ifndef VECTOR_H
#define VECTOR_H

namespace GUtility
{
	class Vector2
	{
	public:
		Vector2() = default;
		Vector2(const float& x, const float& y);
		Vector2(const Vector2& rhs);

		Vector2 operator=(const Vector2& rhs);
		bool operator==(const Vector2& rhs);

		Vector2 operator*(Vector2& rhs);

		Vector2 operator/(Vector2& rhs);
		Vector2 operator/(float& rhs);

		Vector2 operator/=(Vector2& rhs);
		Vector2 operator/=(float& rhs);

		Vector2 operator+(Vector2& rhs);
		Vector2 operator-(Vector2& rhs);

		static Vector2 Zero();

		static float Distance(const Vector2&, const Vector2&);
		static Vector2 Midpoint(const Vector2&, const Vector2&);

		float Magnitude();
		Vector2 Normalise();


		float& X() { return x; }
		float& Y() { return y; }

		void SetX(float& newX) { x = newX; }
		void SetY(float& newY) { y = newY; }

	private:
		float x = 0;
		float y = 0;
	};
}

#endif // !VECTOR_H



