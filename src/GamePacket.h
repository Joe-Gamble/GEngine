#pragma once

#ifndef GAME_PACKET_H
#define GAME_PACKET_H

#include "IncludeMe.h"
#include "NetTransform.h"
#include "NetEntity.h"

using namespace GNet;

namespace GEngine
{
	namespace Networking
	{
		class GamePacket : public Packet
		{
		public:

			GamePacket(PacketType packetType) : Packet(packetType) {}

			GamePacket& operator << (const NetTransform& data); // NetTransform insertion
			GamePacket& operator >> (NetTransform& data); // NetTransform extraction

			GamePacket& operator << (const Vector2& data);
			GamePacket& operator >> (Vector2& data);

			GamePacket& operator << (const short& shortData);
			GamePacket& operator >> (short& newShort);

			GamePacket& operator << (const std::string& string);
			GamePacket& operator >> (std::string& string);

			GamePacket& operator << (ComponentID componentType);
			GamePacket& operator >> (ComponentID& componentType);

			GamePacket& operator << (NetEntity& entity);
			GamePacket& operator >> (NetEntity& componentType);

		};
	}
}

#endif // !GAME_PACKET



