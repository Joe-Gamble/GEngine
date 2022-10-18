#pragma once
#ifndef VECTOR_H
#define VECTOR_H

#include "Vector2Mold.h"

namespace GEngine
{
	namespace GUtility
	{
		class Vector2
		{
		public:
			Vector2() = default;
			Vector2(const float& x, const float& y);
			Vector2(const Vector2& rhs);

			Vector2(Vector2Mold& mold);

			// static const void* Serialise(const Vector2& transform);

			static inline Vector2Mold& CreateMold(const Vector2& transform)
			{
				Vector2Mold mold;

				mold.x = transform.x;
				mold.y = transform.y;
				return mold;
			}

			//ASSINGMENT AND EQUALITY OPERATIONS
			inline Vector2& operator = (const Vector2& v) { x = v.x; y = v.y; return *this; }
			inline Vector2& operator = (const float& f) { x = f; y = f; return *this; }
			inline Vector2& operator - (void) { x = -x; y = -y; return *this; }
			inline bool operator == (const Vector2& v) const { return (x == v.x) && (y == v.y); }
			inline bool operator != (const Vector2& v) const { return (x != v.x) || (y != v.y); }

			//VECTOR2 TO VECTOR2 OPERATIONS
			inline const Vector2 operator + (const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
			inline const Vector2 operator - (const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
			inline const Vector2 operator * (const Vector2& v) const { return Vector2(x * v.x, y * v.y); }
			inline const Vector2 operator / (const Vector2& v) const { return Vector2(x / v.x, y / v.y); }

			//VECTOR2 TO THIS OPERATIONS
			inline Vector2& operator += (const Vector2& v) { x += v.x; y += v.y; return *this; }
			inline Vector2& operator -= (const Vector2& v) { x -= v.x; y -= v.y; return *this; }
			inline Vector2& operator *= (const Vector2& v) { x *= v.x; y *= v.y; return *this; }
			inline Vector2& operator /= (const Vector2& v) { x /= v.x; y /= v.y; return *this; }

			//SCALER TO VECTOR2 OPERATIONS
			inline const Vector2 operator + (float v) const { return Vector2(x + v, y + v); }
			inline const Vector2 operator - (float v) const { return Vector2(x - v, y - v); }
			inline const Vector2 operator * (float v) const { return Vector2(x * v, y * v); }
			inline const Vector2 operator / (float v) const { return Vector2(x / v, y / v); }

			//SCALER TO THIS OPERATIONS
			inline Vector2& operator += (float v) { x += v; y += v; return *this; }
			inline Vector2& operator -= (float v) { x -= v; y -= v; return *this; }
			inline Vector2& operator *= (float v) { x *= v; y *= v; return *this; }
			inline Vector2& operator /= (float v) { x /= v; y /= v; return *this; }

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
}

#endif // !VECTOR_H



