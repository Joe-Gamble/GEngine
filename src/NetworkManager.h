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
			CLIENT_SERVER_TICK,
			SHUTDOWN,
			MAX_STATE
		};

		class NetworkManager
		{
		public:
			static const short Version = 12346;

		public:
			bool MakeServer();
			bool JoinServer(const std::string& ip);

			bool IsServer();
			bool IsClient();

			void OnClientReady() {}

			static inline NetworkManager& Instance()
			{
				static NetworkManager instance;
				return instance;
			}

			static int Tick(void* data);

			void ShutDown();
			inline bool IsInitialised() { return m_initialised; }

			inline bool HasAuthority() { return (m_server != nullptr && m_client == nullptr); }

			inline void SetState(NetworkState state) { currentState = state; }

		private:

			NetworkManager() = default;

			bool Initialise();
			bool InitialiseThread();

			GameServer* m_server = nullptr;
			GameClient* m_client = nullptr;
			
			SDL_Thread* networkThread = nullptr;

			// shutdown thread
			bool m_shutdown = false;
			bool m_initialised = false;
			NetworkState currentState = NetworkState::UNINITIALIZED;

			std::string m_ipAddress;
		};
	}
}



