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
			inline NetEntity* GetNetEntity(short id) { return netEntities[id]->get(); }

			void SendPacket(std::shared_ptr< GamePacket>& packet);

			void ShutDown();
			void EndSession();

		private:

			NetworkManager() = default;

			bool Initialise();
			bool InitialiseThread();

			static int Tick(void* data);

			std::unique_ptr<GameServer> m_server = nullptr;
			std::unique_ptr<GameClient> m_client = nullptr;
			
			// shutdown thread
			SDL_Thread* networkThread = nullptr;

			// netEntity instances
			std::map<short, std::unique_ptr<NetEntity>*> netEntities;
			
			bool m_shutdown = false;
			bool m_initialised = false;
			
			std::string m_ipAddress;

			SDL_mutex* networkStateMutex = SDL_CreateMutex();
			NetworkState currentState = NetworkState::UNINITIALIZED;
		};
	}
}

#endif



