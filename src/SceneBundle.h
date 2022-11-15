#pragma once
#include "Transform.h"
#ifndef SCENE_BUNDLE_H
#define SCENE_BUNDLE_H

#include "SceneFactory.h"
#include "ComponentFactory.h"
#include "ComponentType.h"
#include "Component.h"
#include "NetTransform.h"

#include "TestScene.h"

namespace GEngine
{
	struct SceneBundle
	{
		// Register scenes of the application
		virtual void RegisterScenes() 
		{
			SceneFactory::Instance().AddScene("Test Scene", std::make_unique<TestScene>());
		}

		// Register componnets for the application 
		virtual void RegisterComponents()
		{
			ComponentFactory::Instance().AddComponent(new NetTransform());
		}
	};
}

#endif