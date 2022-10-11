#include "NetTransform.h"
#include <iostream>

const void* NetTransform::Serialise()
{
    NetTransformMold mold;
    mold = NetTransform::CreateMold(*this);
    return reinterpret_cast<const void*>(&mold);
}

std::unique_ptr<Component> NetTransform::Deserialise()
{
    return std::unique_ptr<NetTransform>();
}

void NetTransform::Update(double& dt)
{
    // std::cout << "NET: " << position.X() << std::endl;
}

void NetTransform::ApplyData(const void* data)
{
    
    NetTransformMold* netTransformMold = const_cast<NetTransformMold*>(reinterpret_cast<const NetTransformMold*>(data));
    if (netTransformMold != nullptr)
    {
        *this = NetTransform(*netTransformMold);
    }
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
