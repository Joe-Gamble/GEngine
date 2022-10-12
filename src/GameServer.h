#pragma once

#ifndef GAME_SERVER_H
#define GAME_SERVER_H
#endif // !ENGINE_SERVER

#include "IncludeMe.h"
#include "NetEntity.h"
#include "GamePacket.h"
#include "SDL_thread.h"
#include "SDL.h"
#include <map>

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

			void ChangeScene(const std::string& filepath);
			void Tick();
			void LeaveSession();

			inline bool IsRunning() { return isRunning; }
			inline ServerType GetServerType() { return serverType; }

			inline std::map<int, std::string>& GetConnectionsToClose() { return connectionsToClose; }
			inline bool HasConnectionsToClose() { return !connectionsToClose.empty(); }
			inline void ClearConnectionsToClose() { connectionsToClose.clear(); }

			void SendKickPacketToClient(int clientID, std::string& reason, int timeout);

			struct CloseConnectionDelayedData{
				CloseConnectionDelayedData(TCPConnection* _connection, std::string& _reason, int _timeout)
				{
					connection = _connection;
					reason = _reason;
					timeout = _timeout;
				}

				TCPConnection* connection;
				std::string reason;
				int timeout;
			};

			static int CloseConnectionDelayed(void* data);

			void SendPacket(std::shared_ptr<GamePacket> packet);
			void ProcessLocalPacket(std::shared_ptr<GamePacket> packet);

			NetEntity& MakeEntity();
			void SendEntityToClients(Entity& entity);

			short entityID = 0;

		protected:
			virtual void OnConnect(TCPConnection& newConnection) noexcept override;
			virtual void OnDisconnect(TCPConnection& lostConnection, const std::string& reason) override;
			virtual bool ProcessPacket(std::shared_ptr<Packet> packet, int connectionIndex) override;
		private:
			bool ValidateComponent(Component* component);
			void ValidateClientVersion(const short& version, const int& connection);

			bool isRunning = false;
			
			
			std::map<int, std::string> connectionsToClose;
			ServerType serverType = ServerType::UNKNOWN;
		};
	}
}

