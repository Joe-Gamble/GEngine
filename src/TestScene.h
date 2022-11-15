#pragma once
#ifndef TEST_SCENE_H 
#define TEST_SCENE_H

#include "Scene.h"

namespace GEngine
{
	class TestScene : public Scene
	{
	public:
		void OnSceneLoad() override;
		void OnSceneDestroy() override;
	private:

	};
}

#endif
