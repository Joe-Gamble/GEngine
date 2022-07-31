#pragma once

#ifndef GAME_PACKET_H
#define GAME_PACKET_H
#endif // !GAME_PACKET

#include "IncludeMe.h"
#include "NetTransform.h"

using namespace GNet;

class GamePacket : public Packet
{
public:

	GamePacket(PacketType packetType) : Packet(packetType) {}
	GamePacket& operator << (const NetTransform& data); // NetTransform insertion
	GamePacket& operator >> (NetTransform& data); // NetTransform extraction

	GamePacket& operator << (const Vector2& data);
	GamePacket& operator >> (Vector2& data);
};

