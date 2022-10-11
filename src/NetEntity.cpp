#include "NetEntity.h"

NetEntity::NetEntity(short& _netID) : netID(_netID)
{

}

NetEntity* NetEntity::Instantiate(short& netID)
{
	NetEntity* go = new NetEntity(netID);
	return go;
}
