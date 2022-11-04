#pragma once
#include "Transform.h"
#ifndef SCENE_BUNDLE_H
#define SCENE_BUNDLE_H

#include "SceneFactory.h"
#include "ComponentFactory.h"
#include "ComponentType.h"
#include "Component.h"
#include "NetTransform.h"

namespace GEngine
{
	struct SceneBundle
	{
		virtual void RegisterScenes() 
		{
			SceneFactory::Instance().AddScene("Default Scene", new Scene());
		}

		virtual void RegisterComponents()
		{
			ComponentFactory::Instance().AddComponent(ComponentType::TYPE_NET_TRANSFORM, new NetTransform());
		}
	};
}

#endif