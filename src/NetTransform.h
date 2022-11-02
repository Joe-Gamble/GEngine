#pragma once

#ifndef NET_TRANSFORM
#define NET_TRANSFORM

#include "NetComponent.h"
#include "Vector.h"
#include "NetTransformMold.h"

using namespace GEngine::GUtility;

namespace GEngine
{
	struct NetTransform : NetComponent
	{
	public:

		const void* Serialise() override;
		void Deserialise() override;

		void Update(double& dt) override;
		void ApplyData(const void* data) override;
		bool SendData();

		inline ComponentType GetType() override
		{
			return ComponentType::TYPE_NET_TRANSFORM;
		}

		inline const uint32_t GetMoldSize() override
		{
			return sizeof(NetTransformMold);
		}

	private:


	public:
		NetTransform();
		NetTransform(const NetTransform& transform);

		NetTransform(NetTransformMold& mold);

		static inline NetTransformMold CreateMold(const NetTransform& transform)
		{
			NetTransformMold mold;

			mold.position = Vector2::CreateMold(transform.position);
			mold.scale = Vector2::CreateMold(transform.scale);

			return mold;
		}

		inline Vector2& const GetPosition() { return position; }
		inline void SetPosition(const Vector2& _position) { position = _position; }

		inline Vector2& GetScale() { return scale; }
		inline void SetScale(const Vector2& scale) { position = scale; }
	private:
		Vector2 position;
		Vector2 scale;
		//Quaternion Rotaion
	};
}

#endif // !NET_TRANSFORM



