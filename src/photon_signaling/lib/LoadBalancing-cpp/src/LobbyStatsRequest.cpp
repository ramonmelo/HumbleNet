/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2017 by Exit Games GmbH. All rights reserved.
 * http://www.photonengine.com
 * mailto:developer@photonengine.com
 */

#include "LoadBalancing-cpp/inc/LobbyStatsRequest.h"

/** @file LobbyStatsRequest.h */

namespace ExitGames
{
	namespace LoadBalancing
	{
		using namespace Common;

		LobbyStatsRequest::LobbyStatsRequest(const JString& name, nByte type)
			: mName(name)
			, mType(type)
		{
		}

		JString LobbyStatsRequest::getName(void) const
		{
			return mName;
		}

		nByte LobbyStatsRequest::getType(void) const
		{
			return mType;
		}

		JString& LobbyStatsRequest::toString(JString& retStr, bool withTypes) const
		{
			return retStr += L"{name=" + mName + L", type=" + mType + L"}";
		}
	}
}