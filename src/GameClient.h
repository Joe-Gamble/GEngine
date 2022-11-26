#pragma once
#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include "IncludeMe.h"
#include "NetEntity.h"
#include "GamePacket.h"

#include <unordered_map>


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

			NetEntity* MakeTempEntity(std::shared_ptr<Scene> scene);
			NetEntity* MakeEntity(short id, std::shared_ptr<Scene> scene);

			void RequestEntityInstatiate(NetEntity* entity);
			void RequestEntityChange(NetEntity* netEntity);

			bool ProcessLocalPacket(std::shared_ptr<Packet> packet);

			void Tick();

			inline bool IsClientConnected() { return IsConnected(); }
		protected:
			bool ProcessPacket(std::shared_ptr<Packet> packet) override;
			void OnConnect() override;
			void OnDisconnect(std::string reason) override;
		private:
			std::unordered_map<short, std::unique_ptr<NetEntity>> pendingEntities;
			short pendingEntityID = 0;
		};
	}
}

#endif // !GAME_CLIENT


