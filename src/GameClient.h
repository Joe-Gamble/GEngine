#pragma once
#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include "IncludeMe.h"
#include "NetEntity.h"
#include "GamePacket.h"


namespace GEngine
{
	namespace Networking
	{
		class GameClient : public GNet::Client
		{
		public:
			GameClient();
			bool ConnectToIP(const std::string ip);
			void LeaveSession();

			~GameClient();

			void SendPacket(std::shared_ptr<GamePacket> packet);
			bool ProcessLocalPacket(std::shared_ptr<Packet> packet);

			void Tick();

			inline bool IsClientConnected() { return IsConnected(); }
		protected:
			bool ProcessPacket(std::shared_ptr<Packet> packet) override;
			void OnConnect() override;
			void OnDisconnect(std::string reason) override;

			//void OnConnectFail() override;
		};
	}
}

#endif // !GAME_CLIENT


