/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2017 by Exit Games GmbH. All rights reserved.
 * http://www.photonengine.com
 * mailto:developer@photonengine.com
 */

#include "LoadBalancing-cpp/inc/Internal/PlayerMovementInformant.h"

namespace ExitGames
{
	namespace LoadBalancing
	{
		namespace Internal
		{
			void PlayerMovementInformant::onEnterLocal(MutableRoom& room, Player& player)
			{
				room.addLocalPlayer(player);
			}

			void PlayerMovementInformant::onEnterRemote(MutableRoom& room, int number, const Common::Hashtable& properties)
			{
				if(room.getPlayerForNumber(number))
				{
					PlayerPropertiesUpdateInformant::setIsInactive(room, number, false);
					PlayerPropertiesUpdateInformant::onUpdate(room, number, properties);
				}
				else
					room.addPlayer(number, properties);
			}

			bool PlayerMovementInformant::onLeaveRemote(MutableRoom& room, int number)
			{
				return room.removePlayer(number);
			}

			bool PlayerMovementInformant::onLeaveLocal(MutableRoom& room, int number)
			{
				room.removeAllPlayers();
				return true;
			}
		}
	}
}