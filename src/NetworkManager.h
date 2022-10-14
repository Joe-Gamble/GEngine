#pragma once

#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H
#endif

#include "GameServer.h"
#include "GameClient.h"

#include "SDL_thread.h"
#include "SDL.h"

#include "EventDriver.h"

#include <functional>


namespace GEngine
{
	namespace Networking
	{
		enum NetworkState
		{
			UNINITIALIZED,
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
			inline void SetState(NetworkState state) { currentState = state; }

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
			
			SDL_Thread* networkThread = nullptr;

			// shutdown thread
			bool m_shutdown = false;
			bool m_initialised = false;
			NetworkState currentState = NetworkState::UNINITIALIZED;

			std::string m_ipAddress;
		};
	}
}



