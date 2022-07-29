#include "NetTransform.h"
#include <iostream>

void NetTransform::Update(double& dt)
{
    std::cout << "NET: " << position.X() << std::endl;
}

void NetTransform::ApplyData(NetComponent* transform)
{
    NetTransform* netTransform = reinterpret_cast<NetTransform*>(transform);
    if (netTransform != nullptr)
    {
        NetTransform(netTransform);
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
