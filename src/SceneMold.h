#pragma once
#ifndef SCENE_MOLD
#define SCENE_MOLD


#include <string>
#include "nlohmann/json.hpp"

namespace GEngine
{
	namespace Data
	{
		struct SceneMold
		{
			std::string path; // scene name for json loading
			bool addative; // does the scene add to anything thats current present
			bool inclusive; // does this scene include previous scene entities
			bool blockInput; // block input to any previous panels
			SceneType type; // what layer should the scene be rendered to
		};


		inline void from_json(const nlohmann::json& j, SceneMold& s) {
			j.at("path").get_to(s.path);
			j.at("isAddative").get_to(s.addative);
			j.at("isInclusive").get_to(s.inclusive);
			j.at("blockInput").get_to(s.blockInput);
			j.at("SceneType").get_to(s.type);
		}
	}
}

#endif // !SCENE_MOLD



