#include "NetEntity.h"

using namespace GEngine::Networking;

NetEntity::NetEntity(short& _netID) : netID(_netID)
{

}

NetEntity* NetEntity::Instantiate(short& netID)
{
	NetEntity* go = new NetEntity(netID);
	return go;
}

//std::shared_ptr<GamePacket> NetEntity::MakeEntityPacket(NetEntity* entity)
//{
//	// Owner? 
//	std::shared_ptr<GamePacket> entityPacket = std::make_shared<GamePacket>(PacketType::PT_ENTITY_CHANGE);
//
//	short id = -1;
//	*entityPacket << entity->netID;
//
//	uint32_t componentCount = entity->GetComponents()->size();
//
//	*entityPacket << sizeof(uint32_t);
//	*entityPacket << componentCount;
//
//	entity->AddComponentDataToPacket(entityPacket);
//	return entityPacket;
//}
//
//void NetEntity::AddComponentDataToPacket(std::shared_ptr<GamePacket>& entityPacket)
//{
//	uint32_t componentCount = GetComponents()->size();
//
//	*entityPacket << sizeof(uint32_t);
//	*entityPacket << componentCount;
//
//	for (const auto& entry : *GetComponents())
//	{
//		GamePacket& gamePacket = *entityPacket;
//		Packet* packet = reinterpret_cast<Packet*>(&gamePacket);
//
//		gamePacket << entry->GetMoldSize();
//		packet->Append(entry->Serialise(), entry->GetMoldSize());
//	}
//}
