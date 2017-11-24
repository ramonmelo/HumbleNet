/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2017 by Exit Games GmbH. All rights reserved.
 * http://www.photonengine.com
 * mailto:developer@photonengine.com
 */

#include "LoadBalancing-cpp/inc/LobbyStatsResponse.h"

/** @file LobbyStatsResponse.h */

namespace ExitGames
{
	namespace LoadBalancing
	{
		using namespace Common;

		LobbyStatsResponse::LobbyStatsResponse(const JString& name, nByte type, int peerCount, int roomCount)
			: mName(name)
			, mType(type)
			, mPeerCount(peerCount)
			, mRoomCount(roomCount) 
		{
		}

		JString LobbyStatsResponse::getName(void) const
		{
			return mName;
		}

		nByte LobbyStatsResponse::getType(void) const
		{
			return mType;
		}

		int LobbyStatsResponse::getPeerCount(void) const
		{
			return mPeerCount;
		}

		int LobbyStatsResponse::getRoomCount(void) const
		{
			return mRoomCount;
		}

		JString& LobbyStatsResponse::toString(JString& retStr, bool withTypes) const
		{
			return retStr += L"{name=" + mName.toString() + L", type=" + mType + L", peerCount=" + mPeerCount + L", roomCount=" + mRoomCount + L"}";
		}
	}
}