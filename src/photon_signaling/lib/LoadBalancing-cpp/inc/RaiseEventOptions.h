/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2017 by Exit Games GmbH. All rights reserved.
 * http://www.photonengine.com
 * mailto:developer@photonengine.com
 */

#pragma once

#include "Photon-cpp/inc/PhotonPeer.h"
#include "Photon-cpp/inc/Enums/EventCode.h"
#include "Photon-cpp/inc/Enums/EventCache.h"
#include "Photon-cpp/inc/Enums/EventKey.h"
#include "Photon-cpp/inc/Enums/OperationCode.h"
#include "Photon-cpp/inc/Enums/ParameterCode.h"
#include "Photon-cpp/inc/Enums/ReceiverGroup.h"
#include "LoadBalancing-cpp/inc/Enums/ErrorCode.h"
#include "LoadBalancing-cpp/inc/Enums/PeerStates.h"
#include "LoadBalancing-cpp/inc/Enums/LobbyType.h"
#include "LoadBalancing-cpp/inc/Enums/MatchmakingMode.h"
#include "LoadBalancing-cpp/inc/AuthenticationValues.h"
#include "LoadBalancing-cpp/inc/LobbyStatsRequest.h"
#include "LoadBalancing-cpp/inc/LobbyStatsResponse.h"
#include "LoadBalancing-cpp/inc/WebFlags.h"

namespace ExitGames
{
	namespace LoadBalancing
	{
		class RaiseEventOptions : public Common::Base
		{
		public:
			RaiseEventOptions(nByte channelID=0, nByte eventCaching=Lite::EventCache::DO_NOT_CACHE, const int* targetPlayers=NULL, short numTargetPlayers=0, nByte receiverGroup=Lite::ReceiverGroup::OTHERS, nByte interestGroup=0, const WebFlags& webFlags=WebFlags(), int cacheSliceIndex=0);
			~RaiseEventOptions(void);

			RaiseEventOptions(const RaiseEventOptions& toCopy);
			RaiseEventOptions& operator=(const RaiseEventOptions& toCopy);

			nByte getChannelID(void) const;
			RaiseEventOptions& setChannelID(nByte channelID);
			nByte getEventCaching(void) const;
			RaiseEventOptions& setEventCaching(nByte eventCaching);
			const int* getTargetPlayers(void) const;
			RaiseEventOptions& setTargetPlayers(const int* targetPlayers);
			short getNumTargetPlayers(void) const;
			RaiseEventOptions& setNumTargetPlayers(short numTargetPlayers);
			nByte getReceiverGroup(void) const;
			RaiseEventOptions& setReceiverGroup(nByte receiverGroup);
			nByte getInterestGroup(void) const;
			RaiseEventOptions& setInterestGroup(nByte interestGroup);
			const WebFlags& getWebFlags(void) const;
			RaiseEventOptions& setWebFlags(const WebFlags& webFlags);
			int getCacheSliceIndex(void) const;
			RaiseEventOptions& setCacheSliceIndex(int cacheSliceIndex);

			virtual Common::JString& toString(Common::JString& retStr, bool withTypes=false) const;
		private:
			nByte mChannelID;
			nByte mEventCaching;
			const int* mTargetPlayers;
			short mNumTargetPlayers;
			nByte mReceiverGroup;
			nByte mInterestGroup;
			WebFlags mWebFlags;
			int mCacheSliceIndex;
		};
	}
}