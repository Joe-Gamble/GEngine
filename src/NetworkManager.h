#pragma once

#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include "GameServer.h"
#include "GameClient.h"

#include "SDL_thread.h"
#include "SDL.h"

#include "EventDriver.h"

#include <functional>
#include <map>
#include <mutex>

namespace GEngine
{
	namespace Networking
	{
		enum NetworkState
		{
			UNINITIALIZED = 0,
			INITIALIZED,
			CLIENT_CONNECTING,
			SESSION_ACTIVE,
			SESSION_END,
			SHUTDOWN,
			MAX_STATE
		};

		class NetworkManager
		{
			friend GameClient;
			friend GameServer;
		public:
			static const short Version = 12353;
			static const int MaxPlayers = 2;

		public:
			bool MakeServer(ServerType serverType);
			bool JoinServer(const std::string& ip);

			bool IsServer();
			bool IsClient();

			inline GameServer& GetServer() { return *m_server; }
			inline GameClient& GetClient() { return *m_client; }

			void OnClientReady() {}

			static inline NetworkManager& Instance()
			{
				static NetworkManager instance;
				return instance;
			}

			static bool VerifyNewConnection(const short& version, const int& playerCount, std::string& error);

			inline bool IsInitialised() { return m_initialised; }
			bool HasAuthority();
			void SetState(NetworkState state);

			inline bool HasNetEntity(short id) { return netEntities.count(id); }
			NetEntity* GetNetEntity(short id);
			inline std::unordered_map<short, NetEntity*>* GetNetEntities() { return &netEntities; }

			NetEntity* CreateNewNetEntity(std::shared_ptr<Scene>* scene);
			void SendNewEntity(NetEntity* entityPtr);
			void AddNetEntity(NetEntity* entityPtr);
			void SendNetEntityChanges(short id);

			void SendPacket(std::shared_ptr< GamePacket>& packet);

			void ShutDown();
			void EndSession();

		private:

			NetworkManager() = default;

			bool Initialise();
			bool InitialiseThread();
			bool InitialiseMutex();

			static int Tick(void* data);
			static int SetEndSessionState(void* data);

			std::unique_ptr<GameServer> m_server = nullptr;
			std::unique_ptr<GameClient> m_client = nullptr;
			
			// shutdown thread
			SDL_Thread* networkThread = nullptr;

			// netEntity instances
			std::unordered_map<short, NetEntity*> netEntities;
			
			bool m_shutdown = false;
			bool m_initialised = false;
			
			std::string m_ipAddress;

			SDL_mutex* networkStateMutex = nullptr;
			NetworkState currentState = NetworkState::UNINITIALIZED;
		};
	}
}

#endif



