#pragma once

#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H
#endif

#include <string>
#include "GameClient.h"

namespace GEngine
{
	namespace Networking
	{
		static class NetworkUtils
		{
		public:
			static int ConnectToServer(void* data);
		};
	}
}
