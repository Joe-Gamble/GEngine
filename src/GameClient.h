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
			std::unique_ptr<NetEntity>* MakeEntity(std::shared_ptr<Scene> scene);
			void RequestEntityInstatiate(std::unique_ptr<NetEntity>* entity);
			void RequestEntityChange(std::unique_ptr<NetEntity>* netEntity);
			bool ProcessLocalPacket(std::shared_ptr<Packet> packet);

			void Tick();

			inline bool IsClientConnected() { return IsConnected(); }
		protected:
			bool ProcessPacket(std::shared_ptr<Packet> packet) override;
			void OnConnect() override;
			void OnDisconnect(std::string reason) override;
		private:
			std::unordered_map<std::shared_ptr<Scene>*, std::vector<std::unique_ptr<NetEntity>>> pendingEntities;
		};
	}
}

#endif // !GAME_CLIENT


