/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2017 by Exit Games GmbH. All rights reserved.
 * http://www.photonengine.com
 * mailto:developer@photonengine.com
 */

#include "LoadBalancing-cpp/inc/Internal/RoomPropertiesCacher.h"
#include "LoadBalancing-cpp/inc/Room.h"

namespace ExitGames
{
	namespace LoadBalancing
	{
		namespace Internal
		{
			void RoomPropertiesCacher::cache(Room& room, const Common::Hashtable& properties)
			{
				room.cacheProperties(properties);
			}
		}
	}
}