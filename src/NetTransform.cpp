#include "NetTransform.h"
#include <iostream>

const void* NetTransform::Serialise(const NetTransform& transform)
{
    NetTransformMold mold;
    mold = NetTransform::CreateMold(transform);
    return reinterpret_cast<const void*>(&mold);
}

std::unique_ptr<NetTransform> NetTransform::Deserialise()
{
    return std::unique_ptr<NetTransform>();
}

void NetTransform::Update(double& dt)
{
    // std::cout << "NET: " << position.X() << std::endl;
}

void NetTransform::ApplyData(NetComponent* transform)
{
    NetTransform* netTransform = reinterpret_cast<NetTransform*>(transform);
    if (netTransform != nullptr)
    {
        NetTransform(netTransform);
    }
}

void NetTransform::Clear()
{
    entity = nullptr;

    position = Vector2();
    scale = Vector2();
    // rotation
}

bool NetTransform::SendData()
{
    return false;
}

NetTransform::NetTransform()
{
    position = Vector2();
    scale = Vector2(1, 1);
}

NetTransform::NetTransform(const NetTransform& transform)
{
    position = transform.position;
    scale = transform.scale;
}

NetTransform::NetTransform(NetTransformMold& mold)
{
    position = Vector2(mold.position);
    scale = Vector2(mold.scale);
}
