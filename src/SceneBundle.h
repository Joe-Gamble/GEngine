#pragma once
#ifndef SCENE_BUNDLE_H
#define SCENE_BUNDLE_H

#include "SceneFactory.h"

namespace GEngine
{
	struct SceneBundle
	{
		virtual void RegisterScenes() 
		{
			SceneFactory::Instance().AddScene("Default Scene", new Scene());
		}
	};
}

#endif