#pragma once
#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H
#endif // !ENGINE_SERVER

#include "IncludeMe.h"
//#include <thread>


namespace GEngine
{
	namespace Networking
	{
		class GameClient : private GNet::Client
		{
		public:
			GameClient();
			bool ConnectToIP(const std::string ip);
			~GameClient();

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


