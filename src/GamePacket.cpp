#include "GamePacket.h"
#include "NetworkManager.h"

using namespace GEngine::Networking;

GamePacket& GamePacket::operator<<(const NetTransform& transform)
{
    Packet* packet = reinterpret_cast<Packet*>(this);
    NetTransform newTransform = transform;

    *packet << sizeof(NetTransformMold);
    Append(newTransform.Serialise(), sizeof(NetTransformMold));

    return *this;
}

GamePacket& GamePacket::operator>>(NetTransform& transform)
{
    uint32_t transformSize = 0;

    Packet* packet = reinterpret_cast<Packet*>(this);
    *packet >> transformSize;

    if (extractionOffset + transformSize > buffer.size())
        throw PacketException("[Packet::operator>>(NetTransform& data)] - Extraction offset exceeded buffer size.");

    void* data = malloc(sizeof(NetTransformMold));

    if (data != nullptr && sizeof(data) > 0)
    {
        memcpy(data, &buffer[extractionOffset],sizeof(NetTransformMold));
        NetTransformMold* mold = reinterpret_cast<NetTransformMold*>(data);

        if (mold == nullptr)
            return *this;

        transform = NetTransform(*mold);

        extractionOffset += transformSize;

        std::free(data);
    }
        
    return *this;
}


GamePacket& GamePacket::operator<<(const Vector2& vector)
{
    Packet* packet = reinterpret_cast<Packet*>(this);

    *packet << sizeof(Vector2Mold);
    // Append(Vector2::Serialise(vector), sizeof(Vector2Mold));

    return *this;
}



GamePacket& GamePacket::operator>>(Vector2& vector)
{

    // TO BE TESTED
    vector = Vector2();

    uint32_t vectorSize = 0;

    Packet* packet = reinterpret_cast<Packet*>(this);
    *packet >> vectorSize;

    if (extractionOffset + vectorSize > buffer.size())
        throw PacketException("[Packet::operator>>(NetTransform& data)] - Extraction offset exceeded buffer size.");

    void* data = malloc(sizeof(NetTransformMold));

    if (data != nullptr && sizeof(data) > 0)
    {
        memcpy(data, &buffer[extractionOffset], sizeof(Vector2Mold));
        Vector2Mold* mold = reinterpret_cast<Vector2Mold*>(data);

        if (mold == nullptr)
            return *this;

        vector = Vector2(*mold);

        extractionOffset += vectorSize;

        std::free(data);
    }

    return *this;

    /*

    uint32_t vectorSize;

    Packet packet = *reinterpret_cast<Packet*>(this);
    packet >> vectorSize;

    if ((extractionOffset + sizeof(uint32_t)) > buffer.size())
        throw PacketException("[Packet::operator>>(Vector2& data)] - Extraction offset exceeded buffer size.");

    // // O: insert return statement here
    return *this;
    */
}

GamePacket& GamePacket::operator << (const short& shortData)
{
    Packet* packet = reinterpret_cast<Packet*>(this);

    *packet << sizeof(short);
    const void* data = reinterpret_cast<const void*>(&shortData);
    Append(data, sizeof(short));

    return *this;
}

GamePacket& GamePacket::operator>>(short& newShort)
{
    uint32_t shortSize = 0;

    Packet* packet = reinterpret_cast<Packet*>(this);
    *packet >> shortSize;

    if (extractionOffset + shortSize > buffer.size())
        throw PacketException("[Packet::operator>>(Short& data)] - Extraction offset exceeded buffer size.");

    void* data = malloc(sizeof(short));

    if (data != nullptr && sizeof(data) > 0)
    {
        memcpy(data, &buffer[extractionOffset], sizeof(short));
        short* shortData = reinterpret_cast<short*>(data);

        if (shortData == nullptr)
            return *this;

        newShort = short(*shortData);

        extractionOffset += shortSize;
        std::free(data);
    }

    return *this;
}
