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
		// Register scenes of the application
		virtual void RegisterScenes() 
		{
			SceneFactory::Instance().AddScene("Default Scene", new Scene());
		}

		// Register componnets for the application 
		virtual void RegisterComponents()
		{
			ComponentFactory::Instance().AddComponent(ComponentType::TYPE_NET_TRANSFORM, new NetTransform());
		}
	};
}

#endif