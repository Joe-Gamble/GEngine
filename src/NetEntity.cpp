#include "NetEntity.h"

NetEntity* NetEntity::Instantiate()
{
	NetEntity* go = new NetEntity();
	return go;
}
