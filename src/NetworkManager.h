#pragma once

#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H
#endif

#include "GameServer.h"
#include "GameClient.h"

#include "SDL_thread.h"
#include "SDL.h"

namespace GEngine
{
	namespace Networking
	{
		class NetworkManager
		{
		public:
			bool MakeServer();
			bool JoinServer(const std::string& ip);

			bool IsServer();
			bool IsClient();

			static inline NetworkManager& Instance()
			{
				static NetworkManager instance;
				return instance;
			}

			static int Tick(void* data);

			void ShutDown();
			inline bool IsInitialised() { return initialised; }

			inline bool HasAuthority() { return (server != nullptr && client == nullptr); }

		private:

			NetworkManager() = default;

			bool Initialise();
			bool InitialiseThread();

			GameServer* server = nullptr;
			GameClient* client = nullptr;
			
			SDL_Thread* networkThread = nullptr;

			// shutdown thread
			bool shutdown = false;
			bool initialised = false;
		};
	}
}



