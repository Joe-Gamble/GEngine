#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include "EntityManager.h"
#include "SDL.h"

namespace GEngine
{
	enum SceneType
	{
		UNKNOWN = -1,
		GAME = 0,
		UI = 1
	};

	class Scene
	{
	public:
		Scene();
		~Scene();

		virtual void Update(double& dt);
		virtual void Render();

		virtual void OnSceneLoad() {};
		virtual void OnSceneDestroy() {};


		std::unique_ptr<NetEntity>* AddNetEntity(NetEntity* entity);

		inline void SetActive(bool active) { m_isActive = active; }
		inline void SetBlocking(bool blocking) { m_blocking = blocking; }
		inline void SetType(SceneType _type) { type = _type; }

		inline bool IsActive() { return m_isActive; }
		inline bool IsBlocking() { return m_blocking; }
		inline bool IsType(SceneType _type) { return type == _type; }
		inline SceneType GetType() { return type; }
		

		inline std::unique_ptr<EntityManager>* GetEntityManager() { return &m_entityManager; }
	private:
		std::unique_ptr<EntityManager> m_entityManager = std::make_unique<EntityManager>(this);

		bool m_isActive = false;
		bool m_blocking = false;

		SceneType type = SceneType::UNKNOWN;
	};
}

#endif // !SCENE_H

