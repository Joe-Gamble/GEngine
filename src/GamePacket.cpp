#include "GamePacket.h"

GamePacket& GamePacket::operator<<(const NetTransform& transform)
{
    Packet* packet = reinterpret_cast<Packet*>(this);

    *packet << sizeof(NetTransformMold);
    Append(NetTransform::Serialise(transform), sizeof(NetTransformMold));

    return *this;
}

GamePacket& GamePacket::operator>>(NetTransform& transform)
{
    transform.Clear();

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
    Append(Vector2::Serialise(vector), sizeof(Vector2Mold));

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

        vector = Vcetor2(*mold);

        extractionOffset += vectorSize;

        std::free(data);
    }

    return *this

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