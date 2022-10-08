#pragma once

#ifndef GAME_SERVER_H
#define GAME_SERVER_H
#endif // !ENGINE_SERVER

#include "IncludeMe.h"
#include "NetComponent.h"
#include "GamePacket.h"
#include <thread>
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

			inline std::map<int, std::string>* GetConnectionsToClose() { return &connectionsToClose; }
			inline bool HasConnectionsToClose() { return !connectionsToClose.empty(); }
			inline void ClearConnectionsToClose() { connectionsToClose.clear(); }
			void CloseConnectionWithClient(int clientID, const std::string& reason);

			void SendPacket(std::shared_ptr<GamePacket> packet);
			void ProcessLocalPacket(std::shared_ptr<GamePacket> packet);

		protected:
			virtual void OnConnect(TCPConnection& newConnection) noexcept override;
			virtual void OnDisconnect(TCPConnection& lostConnection, std::string reason) override;
			virtual bool ProcessPacket(std::shared_ptr<Packet> packet, int connectionIndex) override;
			//virtual void OnMaxConnectionsReached() override;
		private:
			bool ValidateComponent(NetComponent* component);
			void ValidateClientVersion(const short& version, const int& connection);

			bool isRunning = false;
			std::map<int, std::string> connectionsToClose;
			ServerType serverType = ServerType::UNKNOWN;
		};
	}
}

