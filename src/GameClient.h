#pragma once
#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H
#endif // !ENGINE_SERVER

#include "IncludeMe.h"
#include "Entity.h"
#include "GamePacket.h"


namespace GEngine
{
	namespace Networking
	{
		class GameClient : private GNet::Client
		{
		public:
			GameClient();
			bool ConnectToIP(const std::string ip);
			void LeaveSession();

			~GameClient();

			void SendServerRequest();
			void SendPacket(std::shared_ptr<GamePacket> packet);
			void ProcessLocalPacket(std::shared_ptr<GamePacket> packet);

			void Tick();

			inline bool IsClientConnected() { return IsConnected(); }
		protected:
			bool ProcessPacket(std::shared_ptr<Packet> packet) override;
			void OnConnect() override;
			//void OnConnectFail() override;
			//void OnDisconnect(std::string reason) override;
		};
	}
}


