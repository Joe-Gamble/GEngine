#pragma once
#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H
#endif // !ENGINE_SERVER

#include "IncludeMe.h"
#include <thread>

namespace GEngine
{
	namespace Networking
	{
		class GameClient : GNet::Client
		{
		public:
			GameClient(const std::string ip);
			~GameClient();

		protected:
			bool ProcessPacket(std::shared_ptr<Packet> packet) override;
			void OnConnect() override;
			//void OnConnectFail() override;
			//void OnDisconnect(std::string reason) override;

		private:
			void Tick();
			std::unique_ptr<std::thread> thread;
		};
	}
}


