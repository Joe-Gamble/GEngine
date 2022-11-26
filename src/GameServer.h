#pragma once

#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include "IncludeMe.h"
#include "NetEntity.h"
#include "GamePacket.h"

#include "SDL_thread.h"
#include "SDL.h"

#include <map>
#include <chrono>

namespace GEngine
{
	namespace Networking
	{
		enum ServerType
		{
			UNKNOWN,
			DEDICATED,
			HOSTED
		};
		class GameServer : public GNet::Server
		{
		public:
			GameServer(ServerType type);
			~GameServer();

			bool InitialiseServer();

			void Tick();
			void EndSession();

			inline std::vector<TCPConnection>& GetConnections() { return connections; }

			inline bool IsRunning() { return isRunning; }
			inline bool InSession() { return inSession; }

			inline ServerType GetServerType() { return serverType; }

			inline std::map<TCPConnection*, std::string>& GetConnectionsToClose() { return connectionsToClose; }
			inline bool HasConnectionsToClose() { return !connectionsToClose.empty(); }
			inline void ClearConnectionsToClose() { connectionsToClose.clear(); }

			void SendKickPacketToClient(TCPConnection* connection, std::string& reason, long long timeout);

			struct CloseConnectionDelayedData
			{
				CloseConnectionDelayedData(TCPConnection* _connection, std::string& _reason, long long _timeout) 
					: connection(_connection), reason(_reason), timeout(_timeout)
				{}

				TCPConnection* connection;
				std::string reason;
				int timeout;
			};

			static int CloseConnectionDelayed(void* data);

			void SendPacket(std::shared_ptr<GamePacket> packet);
			void ProcessLocalPacket(std::shared_ptr<GamePacket> packet);

			NetEntity* MakeEntity(std::shared_ptr<Scene> scene);

			void SendEntityToClients(NetEntity* entity, bool isNew);
			void SendClientstoNewScene(const std::string& scene);

		protected:
			virtual void OnConnect(TCPConnection& newConnection) noexcept override;
			virtual void OnDisconnect(TCPConnection& lostConnection, const std::string& reason) override;
			virtual bool ProcessPacket(std::shared_ptr<Packet> packet, int connectionIndex) override;
		private:
			bool ValidateComponent(Component* component);
			void ValidateClientVersion(const short& version, const int& connection);

			bool isRunning = false;
			bool inSession = false;
			
			std::map<TCPConnection*, std::string> connectionsToClose;

			ServerType serverType = ServerType::UNKNOWN;
			short entityID = 0;
			short clientID = 0;
		};
	}
}

#endif // !GAME_SERVER

