#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include "EntityManager.h"
#include "SDL.h"

namespace GEngine
{
	enum SceneType :uint16_t
	{
		UNKNOWN = -1,
		GAME = 0,
		UI = 1
	};

	class Scene
	{
	public:
		void Update(double& dt);
		void Render();
		virtual void OnSceneLoad(std::shared_ptr<Scene>* scene) {};
		virtual void OnSceneDestroy() {};

	protected:		
		virtual void OnUpdate(double& dt) {}
		virtual void OnRender() {}

	public:
		std::unique_ptr<NetEntity>* AddNetEntity(NetEntity* entity);
		std::unique_ptr<Entity>* AddEntity(Entity* entity);

		inline void SetActive(bool active) { m_isActive = active; }
		inline void SetBlocking(bool blocking) { m_blocking = blocking; }
		inline void SetType(SceneType _type) { type = _type; }

		inline bool IsActive() { return m_isActive; }
		inline bool IsBlocking() { return m_blocking; }
		inline bool IsType(SceneType _type) { return type == _type; }
		inline SceneType GetType() { return type; }

		inline EntityManager& GetEntityManager() { return *m_entityManager.get(); }

	private:
		std::unique_ptr<EntityManager> m_entityManager = std::make_unique<EntityManager>();

		bool m_isActive = false;
		bool m_blocking = false;
		SceneType type = SceneType::UNKNOWN;
	};
}

#endif // !SCENE_H

