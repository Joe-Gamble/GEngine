#include "GamePacket.h"
#include "NetworkManager.h"
#include "ComponentFactory.h"

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

GamePacket& GamePacket::operator<<(const std::string& string)
{
    Packet* packet = static_cast<Packet*>(this);
    *packet << string;

    return *this;
}

GamePacket& GamePacket::operator>>(std::string& string)
{
    Packet* packet = static_cast<Packet*>(this);
    *packet >> string;

    return *this;
}

GamePacket& GamePacket::operator<<(ComponentID componentType)
{
    Packet* packet = reinterpret_cast<Packet*>(this);

    *packet << sizeof(uint16_t);
    const void* data = reinterpret_cast<const void*>(&componentType);
    Append(data, sizeof(uint16_t));

    return *this;
}

GamePacket& GamePacket::operator>>(ComponentID& componentType)
{
    uint32_t componentTypeSize = 0;

    Packet* packet = reinterpret_cast<Packet*>(this);
    *packet >> componentTypeSize;

    if (extractionOffset + componentTypeSize > buffer.size())
        throw PacketException("[Packet::operator>>(Short& data)] - Extraction offset exceeded buffer size.");

    void* data = malloc(sizeof(size_t));

    if (data != nullptr && sizeof(data) > 0)
    {
        memcpy(data, &buffer[extractionOffset], sizeof(short));
        size_t* shortData = reinterpret_cast<size_t*>(data);

        if (shortData == nullptr)
            return *this;

        componentType = ComponentID(*shortData);

        extractionOffset += componentTypeSize;
        std::free(data);
    }

    return *this;
}

GamePacket& GamePacket::operator<<(NetEntity& entity)
{
    Packet* packet = reinterpret_cast<Packet*>(this);
    *packet << entity.GetID();

    uint32_t componentCount = entity.GetComponents()->size();
    *packet << componentCount;

    for (const auto& entry : *entity.GetNetComponents())
    {
        NetComponent& component = *entry;

        *this << ComponentFactory::Instance().GetComponentID(&component);

        *packet << component.GetMoldSize();
        packet->Append(component.Serialise(), component.GetMoldSize());
    }

    return *this;
}

GamePacket& GamePacket::operator>>(NetEntity& entity)
{
    Packet* packet = reinterpret_cast<Packet*>(this);
    uint32_t componentCount = 0;

    short id;
    *this >> id;

    if (entity.GetID() != id)
        return *this;

    *packet >> componentCount;

    if (componentCount <= 0)  
        return *this;

    for (int i = 0; i < componentCount; i++)
    {
        ComponentID componentID;
        *this >> componentID;

        uint32_t componentSize = 0;
        *packet >> componentSize;

        if (extractionOffset + componentSize > buffer.size())
            throw PacketException("[GamePacket::operator>>(NetEntity& entity)] - Extraction offset exceeded buffer size.");


        std::unique_ptr<Component>* componentInfo = ComponentFactory::Instance().GetComponent(componentID);

        if (componentInfo != nullptr)
        {
            Component* component = componentInfo->get();
            
            void* data = malloc(component->GetMoldSize());

            if (data != nullptr && sizeof(data) > 0)
            {
                using T = std::remove_reference_t<decltype(*component)>;
                Component* componentInstance = entity.AddComponent<T>();

                if (componentInstance != nullptr)
                {
                    memcpy(data, &buffer[extractionOffset], componentSize);
                    componentInstance->ApplyData(data);
                }
                
                extractionOffset += componentSize;
            }
            std::free(data);
        }
    }

    return *this;
}
